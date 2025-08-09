// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TurretEnemy.h"
#include "Player/FPSCharacter.h"
#include "Projectile/FPSProjectile.h"
#include "Perception/AISenseConfig_Sight.h"

ATurretEnemy::ATurretEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2000.0f;
	SightConfig->LoseSightRadius = 2500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ATurretEnemy::BeginPlay()
{
	Super::BeginPlay();
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ATurretEnemy::OnPawnDetected);
	SetState(EAIState::Idle);
}

void ATurretEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurretEnemy::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (AActor* Actor : DetectedPawns)
	{
		if (Cast<AFPSCharacter>(Actor))
		{
			SetState(EAIState::Attack);
			return;
		}
	}
	SetState(EAIState::Idle);
}

void ATurretEnemy::OnStateEnter(EAIState State)
{
	Super::OnStateEnter(State);
	switch (State)
	{
	case EAIState::Idle:
		GetWorldTimerManager().ClearTimer(FireRateTimerHandle);
		break;
	case EAIState::Attack:
		GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurretEnemy::Fire, FireRate, true, 0.0f);
		break;
	default:
		break;
	}
}

void ATurretEnemy::OnStateUpdate(EAIState State, float DeltaTime)
{
	Super::OnStateUpdate(State, DeltaTime);
	if (State == EAIState::Attack)
	{
		AFPSCharacter* Player = Cast<AFPSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Player)
		{
			FVector DirectionToPlayer = Player->GetActorLocation() - GetActorLocation();
			FRotator NewRotation = DirectionToPlayer.Rotation();
			SetActorRotation(NewRotation);
		}
	}
}

void ATurretEnemy::OnStateExit(EAIState State)
{
	Super::OnStateExit(State);
}

void ATurretEnemy::Fire()
{
	if (!ProjectileClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	AFPSCharacter* Player = Cast<AFPSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
		FRotator SpawnRotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (Projectile)
		{
			FVector LaunchDirection = (Player->GetActorLocation() - SpawnLocation).GetSafeNormal();
			Projectile->FireInDirection(LaunchDirection);
		}
	}
}
