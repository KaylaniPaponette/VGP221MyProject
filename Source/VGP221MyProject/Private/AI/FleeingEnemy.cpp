// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FleeingEnemy.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Player/FPSCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Navigation/PathFollowingComponent.h"


AFleeingEnemy::AFleeingEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2000.0f;
	SightConfig->LoseSightRadius = 1200.0f;
	SightConfig->PeripheralVisionAngleDegrees = 120.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AFleeingEnemy::BeginPlay()
{
	Super::BeginPlay();
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AFleeingEnemy::OnPawnDetected);
	SetState(EAIState::Idle);
}

void AFleeingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFleeingEnemy::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (AActor* Actor : DetectedPawns)
	{
		if (Cast<AFPSCharacter>(Actor))
		{
			SetState(EAIState::Flee);
			return;
		}
	}
	SetState(EAIState::Idle);
}

void AFleeingEnemy::FindNewFleeLocation()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	AFPSCharacter* Player = Cast<AFPSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (AIController && Player)
	{
		FVector FleeDirection = GetActorLocation() - Player->GetActorLocation();
		FleeDirection.Normalize();
		FVector FleeLocation = GetActorLocation() + FleeDirection * 1000.0f;

		UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (NavSystem)
		{
			FNavLocation RandomLocation;
			if (NavSystem->GetRandomPointInNavigableRadius(FleeLocation, 500.0f, RandomLocation))
			{
				AIController->MoveToLocation(RandomLocation.Location);
			}
		}
	}
}


void AFleeingEnemy::OnStateEnter(EAIState State)
{
	Super::OnStateEnter(State);
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return;

	switch (State)
	{
	case EAIState::Idle:
		AIController->StopMovement();
		break;
	case EAIState::Flee:
		// Find the first location to run to
		FindNewFleeLocation();
		break;
	default:
		break;
	}
}

void AFleeingEnemy::OnStateUpdate(EAIState State, float DeltaTime)
{
	Super::OnStateUpdate(State, DeltaTime);
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return;

	switch (State)
	{
	case EAIState::Flee:
		// If we've reached our destination but are still in the Flee state, find a new place to run!
		if (AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
		{
			FindNewFleeLocation();
		}
		break;
	default:
		break;
	}
}

void AFleeingEnemy::OnStateExit(EAIState State)
{
	Super::OnStateExit(State);
}
