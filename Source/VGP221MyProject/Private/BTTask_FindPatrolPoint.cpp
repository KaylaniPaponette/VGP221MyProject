// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPoint.h"
#include "AIController.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    AAICharacter* AICharacter = Cast<AAICharacter>(AIController->GetPawn());
    if (!AICharacter) return EBTNodeResult::Failed;

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComponent) return EBTNodeResult::Failed;

    // Get the current patrol point index
    int32 PatrolPointIndex = BlackboardComponent->GetValueAsInt("PatrolPointIndex");

    // Set the next patrol point as the target location
    if (AICharacter->PatrolPoints.Num() > 0)
    {
        BlackboardComponent->SetValueAsVector("TargetLocation", AICharacter->PatrolPoints[PatrolPointIndex]->GetActorLocation());

        // Increment and loop the patrol point index
        PatrolPointIndex = (PatrolPointIndex + 1) % AICharacter->PatrolPoints.Num();
        BlackboardComponent->SetValueAsInt("PatrolPointIndex", PatrolPointIndex);

        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}