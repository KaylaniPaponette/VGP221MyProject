// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BossEnemy.h"
#include "Player/FPSCharacter.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Animation/AnimInstance.h"
#include "Projectile/FPSProjectile.h"

ABossEnemy::ABossEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 4000.0f;
	SightConfig->LoseSightRadius = 4500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	bIsBoss = true;
	Health = MaxHealth;
}

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ABossEnemy::OnPawnDetected);
	SetState(EAIState::Idle);
}

void ABossEnemy::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	// If the boss is idle, start the fight
	if (CurrentState == EAIState::Idle)
	{
		for (AActor* Actor : DetectedPawns)
		{
			if (Cast<AFPSCharacter>(Actor))
			{
				UpdateStage(); // This will set the state to Boss_Melee
				return;
			}
		}
	}
}

float ABossEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (Health > 0)
	{
		// Check if we need to change stage after taking damage
		UpdateStage();
	}
	return ActualDamage;
}

void ABossEnemy::UpdateStage()
{
	float HealthPercentage = Health / MaxHealth;

	if (HealthPercentage > 0.5f && Stage != 1)
	{
		Stage = 1;
		SetState(EAIState::Boss_Melee);
	}
	else if (HealthPercentage <= 0.5f && Stage != 2)
	{
		Stage = 2;
		SetState(EAIState::Boss_Ranged);
	}
	// If the boss was idle, this will start stage 1
	else if (CurrentState == EAIState::Idle)
	{
		Stage = 1;
		SetState(EAIState::Boss_Melee);
	}
}

void ABossEnemy::OnStateEnter(EAIState State)
{
	Super::OnStateEnter(State);
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return;

	switch (State)
	{
	case EAIState::Boss_Melee:
		// Behavior handled in Update
		break;
	case EAIState::Boss_Ranged:
		// Stop moving for the ranged phase
		AIController->StopMovement();
		break;
	default:
		break;
	}
}

void ABossEnemy::OnStateUpdate(EAIState State, float DeltaTime)
{
	Super::OnStateUpdate(State, DeltaTime);
	AAIController* AIController = Cast<AAIController>(GetController());
	AFPSCharacter* Player = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (!AIController || !Player || CurrentState == EAIState::Idle) return;

	float DistanceToPlayer = FVector::Dist(GetActorLocation(), Player->GetActorLocation());

	switch (State)
	{
	case EAIState::Boss_Melee:
		AIController->MoveToActor(Player); // Chase player
		if (DistanceToPlayer < 250.0f && bCanAttack) // Check if close enough to attack
		{
			MeleeAttack();
		}
		break;
	case EAIState::Boss_Ranged:
		// Always face the player
		FVector DirectionToPlayer = Player->GetActorLocation() - GetActorLocation();
		FRotator NewRotation = DirectionToPlayer.Rotation();
		SetActorRotation(NewRotation);

		if (bCanAttack)
		{
			RangedAttack();
		}
		break;
	default:
		break;
	}
}

void ABossEnemy::OnStateExit(EAIState State)
{
	Super::OnStateExit(State);
	// Clear any attack timers when changing state
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	bCanAttack = true; // Ensure we can attack in the new state
}

void ABossEnemy::MeleeAttack()
{
	if (!bCanAttack) return;
	bCanAttack = false;

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController) AIController->StopMovement();


	if (AttackMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance) AnimInstance->Montage_Play(AttackMontage);
	}

	// Apply damage to player
	AFPSCharacter* Player = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player)
	{
		UGameplayStatics::ApplyDamage(Player, MeleeDamage, GetController(), this, UDamageType::StaticClass());
	}

	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ABossEnemy::ResetAttack, MeleeAttackRate, false);
}

void ABossEnemy::RangedAttack()
{
	if (!bCanAttack || !ProjectileClass) return;
	bCanAttack = false;

	if (AttackMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance) AnimInstance->Montage_Play(AttackMontage);
	}

	// Spawn and fire projectile
	AFPSCharacter* Player = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player)
	{
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 150.0f;
		FRotator SpawnRotation = GetActorRotation();

		AFPSProjectile* Projectile = GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		if (Projectile)
		{
			FVector LaunchDirection = (Player->GetActorLocation() - SpawnLocation).GetSafeNormal();
			Projectile->FireInDirection(LaunchDirection);
		}
	}

	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ABossEnemy::ResetAttack, RangedAttackRate, false);
}

void ABossEnemy::ResetAttack()
{
	bCanAttack = true;
}
