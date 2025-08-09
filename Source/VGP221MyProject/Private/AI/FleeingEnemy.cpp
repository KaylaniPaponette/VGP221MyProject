// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FleeingEnemy.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Player/FPSCharacter.h"
#include "Perception/AISenseConfig_Sight.h"

AFleeingEnemy::AFleeingEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 3000.0f;
	SightConfig->LoseSightRadius = 1200.0f;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;
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
	{
		AFPSCharacter* Player = Cast<AFPSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Player)
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
	break;
	default:
		break;
	}
}

void AFleeingEnemy::OnStateUpdate(EAIState State, float DeltaTime)
{
	Super::OnStateUpdate(State, DeltaTime);
}

void AFleeingEnemy::OnStateExit(EAIState State)
{
	Super::OnStateExit(State);
}
