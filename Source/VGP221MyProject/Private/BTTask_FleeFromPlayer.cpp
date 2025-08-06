// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FleeFromPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h" // Required for finding a point on the NavMesh

UBTTask_FleeFromPlayer::UBTTask_FleeFromPlayer()
{
    // Set a descriptive name for this node in the Behavior Tree editor.
    NodeName = "Flee From Player";

    // Set default blackboard key names. These can be changed in the editor.
    PlayerKey.SelectedKeyName = "Player";
    FleeLocationKey.SelectedKeyName = "TargetLocation";
}

EBTNodeResult::Type UBTTask_FleeFromPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Get the AI Controller and the Pawn it controls.
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* AIPawn = AIController ? AIController->GetPawn() : nullptr;
    if (!AIPawn)
    {
        return EBTNodeResult::Failed;
    }

    // Get the Blackboard component.
    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComponent)
    {
        return EBTNodeResult::Failed;
    }

    // Get the Player character from the Blackboard.
    ACharacter* PlayerCharacter = Cast<ACharacter>(BlackboardComponent->GetValueAsObject(PlayerKey.SelectedKeyName));
    if (!PlayerCharacter)
    {
        // If the player isn't set (e.g., they went out of sight), the condition to flee is no longer met.
        // We can fail here, and the Behavior Tree will switch to another branch (like patrolling).
        return EBTNodeResult::Failed;
    }

    // --- Fleeing Logic ---

    // 1. Get locations of the AI and the Player.
    const FVector AILocation = AIPawn->GetActorLocation();
    const FVector PlayerLocation = PlayerCharacter->GetActorLocation();

    // 2. Calculate the direction vector away from the player.
    FVector FleeDirection = AILocation - PlayerLocation;
    FleeDirection.Normalize(); // We only need the direction, so we normalize the vector.

    // 3. Calculate a potential flee destination.
    const FVector PotentialFleeLocation = AILocation + FleeDirection * FleeDistance;

    // 4. Find an actual valid point on the navigation mesh near our potential location.
    // This is crucial to prevent the AI from trying to run into walls or off cliffs.
    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    FNavLocation NavLocation;
    if (NavSystem && NavSystem->GetRandomPointInNavigableRadius(PotentialFleeLocation, 100.0f, NavLocation))
    {
        // 5. If a valid point was found, set it in the blackboard.
        BlackboardComponent->SetValueAsVector(FleeLocationKey.SelectedKeyName, NavLocation.Location);
        return EBTNodeResult::Succeeded; // The task succeeded in finding a flee location.
    }

    // If we couldn't find a valid navigation point, the task fails.
    return EBTNodeResult::Failed;
}
