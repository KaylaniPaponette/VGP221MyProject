// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PatrollingEnemy.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Player/FPSCharacter.h"
#include "Perception/AISenseConfig_Sight.h"


APatrollingEnemy::APatrollingEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 3000.0f;
	SightConfig->LoseSightRadius = 1500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void APatrollingEnemy::BeginPlay()
{
	Super::BeginPlay();
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &APatrollingEnemy::OnPawnDetected);
	SetState(EAIState::Patrol);
}

void APatrollingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APatrollingEnemy::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (AActor* Actor : DetectedPawns)
	{
		if (Cast<AFPSCharacter>(Actor))
		{
			SetState(EAIState::Chase);
			return;
		}
	}
	SetState(EAIState::Patrol);
}

void APatrollingEnemy::OnStateEnter(EAIState State)
{
	Super::OnStateEnter(State);
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return;

	switch (State)
	{
	case EAIState::Patrol:
		if (PatrolPoints.Num() > 0)
		{
			AIController->MoveToActor(PatrolPoints[CurrentPatrolPointIndex]);
		}
		break;
	case EAIState::Chase:
		// Chase logic is handled in the update
		break;
	case EAIState::Attack:
		// Attack logic is handled in the update
		break;
	default:
		break;
	}
}

void APatrollingEnemy::OnStateUpdate(EAIState State, float DeltaTime)
{
	Super::OnStateUpdate(State, DeltaTime);
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return;

	switch (State)
	{
	case EAIState::Patrol:
		if (AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
		{
			CurrentPatrolPointIndex = (CurrentPatrolPointIndex + 1) % PatrolPoints.Num();
			if (PatrolPoints.Num() > 0)
			{
				AIController->MoveToActor(PatrolPoints[CurrentPatrolPointIndex]);
			}
		}
		break;
	case EAIState::Chase:
	{
		AFPSCharacter* Player = Cast<AFPSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Player)
		{
			AIController->MoveToActor(Player);
			if (FVector::Dist(GetActorLocation(), Player->GetActorLocation()) < 200.0f)
			{
				SetState(EAIState::Attack);
			}
		}
	}
	break;
	case EAIState::Attack:
	{
		AFPSCharacter* Player = Cast<AFPSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Player)
		{
			if (FVector::Dist(GetActorLocation(), Player->GetActorLocation()) > 250.0f)
			{
				SetState(EAIState::Chase);
			}
			// Add attack logic here (e.g., melee attack)
		}
	}
	break;
	default:
		break;
	}
}

void APatrollingEnemy::OnStateExit(EAIState State)
{
	Super::OnStateExit(State);
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return;

	switch (State)
	{
	case EAIState::Patrol:
		AIController->StopMovement();
		break;
	case EAIState::Chase:
		AIController->StopMovement();
		break;
	case EAIState::Attack:
		break;
	default:
		break;
	}
}
