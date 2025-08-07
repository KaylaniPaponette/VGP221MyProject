// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "AICharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"

AMyAIController::AMyAIController()
{
    // Create the perception component
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    // Create and configure the sight sense
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 2000.0f;
    SightConfig->LoseSightRadius = 1500.0f;
    SightConfig->PeripheralVisionAngleDegrees = 90.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    // Configure and dominate the perception component with the sight sense
    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

    // Bind the perception update function
    AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AMyAIController::OnPerceptionUpdated);
}

void AMyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Start the behavior tree
    if (BehaviorTree)
    {
        RunBehaviorTree(BehaviorTree);
    }
}

//void AMyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
//{
//    for (AActor* Actor : UpdatedActors)
//    {
//        // Check if the perceived actor has the "Player" tag
//        if (Actor->ActorHasTag(TEXT("Player")))
//        {
//            // Get the blackboard component
//            UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
//            if (BlackboardComponent)
//            {
//                // Set the "Player" key to the perceived player actor
//                BlackboardComponent->SetValueAsObject(TEXT("Player"), Actor);
//                return; // Exit the function since we found the player
//            }
//        }
//    }
//
//    // If the loop completes and no actor with the "Player" tag was found,
//    // clear the "Player" key. This happens when the player goes out of sight.
//    UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
//    if (BlackboardComponent)
//    {
//        BlackboardComponent->ClearValue(TEXT("Player"));
//    }
//}

void AMyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    for (AActor* Actor : UpdatedActors)
    {
        // Check if the perceived actor is the player
        if (Actor->IsA(ACharacter::StaticClass()) && Actor != GetPawn())
        {
            // Get the blackboard component
            UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
            if (BlackboardComponent)
            {
                // Set the "Player" key to the perceived player actor
                BlackboardComponent->SetValueAsObject(TEXT("Player"), Actor);
                return;
            }
        }
    }

    // If no player is found, clear the "Player" key
    UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
    if (BlackboardComponent)
    {
        BlackboardComponent->ClearValue(TEXT("Player"));
    }
}


