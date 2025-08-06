// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FleeFromPlayer.generated.h"

/**
 * 
 */
UCLASS()
class VGP221MYPROJECT_API UBTTask_FleeFromPlayer : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_FleeFromPlayer();

    // The main execution function for this task.
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    // The distance the AI will try to flee from the player.
    UPROPERTY(EditAnywhere, Category = "AI")
    float FleeDistance = 500.0f;

    // The Blackboard key where the Player Actor is stored.
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector PlayerKey;

    // The Blackboard key where the calculated flee location will be stored.
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector FleeLocationKey;
};

