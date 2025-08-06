// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class VGP221MYPROJECT_API AMyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AMyAIController();

    virtual void OnPossess(APawn* InPawn) override;

    // The Behavior Tree asset to use
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    class UBehaviorTree* BehaviorTree;

    // The perception component for sensing other actors
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAIPerceptionComponent* AIPerceptionComponent;

private:
    // Configuration for the sight sense
    UAISenseConfig_Sight* SightConfig;

    // Function called when perception is updated
    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
};
