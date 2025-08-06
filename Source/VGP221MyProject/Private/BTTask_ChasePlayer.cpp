// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComponent) return EBTNodeResult::Failed;

    // Get the player from the blackboard
    ACharacter* Player = Cast<ACharacter>(BlackboardComponent->GetValueAsObject("Player"));
    if (Player)
    {
        // Move to the player's location
        AIController->MoveToActor(Player, 50.0f);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
