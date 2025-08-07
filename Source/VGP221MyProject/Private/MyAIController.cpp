// -----------------------------------------------------------------------------
// MyAIController.cpp
// This is the implementation for the AI controller.
// -----------------------------------------------------------------------------
#include "MyAIController.h"
#include "AICharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h" // Required for FAIStimulus
#include "GenericTeamAgentInterface.h" // Required for GetTeamAttitudeTowards

AMyAIController::AMyAIController()
{
    // Create the perception component
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    // Create and configure the sight sense
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 2500.0f;
    SightConfig->LoseSightRadius = 2000.0f;
    SightConfig->PeripheralVisionAngleDegrees = 180.0f;

    // IMPORTANT: Configure the sight to detect different teams.
    // The AI will now see actors as Enemies, Neutrals, or Friendlies based on their Team ID.
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    // Configure and dominate the perception component with the sight sense
    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

    // Bind the perception update function. Note we use OnTargetPerceptionUpdated now.
    AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMyAIController::OnTargetPerceptionUpdated);
}

void AMyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // When this controller possesses a pawn, adopt that pawn's team ID.
    // This makes the controller and the character act as one unit.
    if (IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(InPawn))
    {
        SetGenericTeamId(TeamAgent->GetGenericTeamId());
    }

    // Start the behavior tree
    if (BehaviorTree)
    {
        RunBehaviorTree(BehaviorTree);
    }
}

FGenericTeamId AMyAIController::GetGenericTeamId() const
{
    // Return the team ID of the pawn this controller is possessing.
    if (const IGenericTeamAgentInterface* TeamAgent = Cast<const IGenericTeamAgentInterface>(GetPawn()))
    {
        return TeamAgent->GetGenericTeamId();
    }
    // Return an invalid ID if no pawn or team is found.
    return FGenericTeamId::NoTeam;
}

// THIS IS THE MISSING FUNCTION DEFINITION
ETeamAttitude::Type AMyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
    const APawn* OtherPawn = Cast<APawn>(&Other);
    if (!OtherPawn)
    {
        // If the other actor isn't a pawn, treat it as neutral.
        return ETeamAttitude::Neutral;
    }

    // Get the team agent interface for the other pawn
    const IGenericTeamAgentInterface* TeamAgent = Cast<const IGenericTeamAgentInterface>(OtherPawn);
    if (!TeamAgent)
    {
        // If the other pawn doesn't have a team, treat it as neutral.
        return ETeamAttitude::Neutral;
    }

    // Get the team IDs for both this controller and the other actor
    FGenericTeamId MyTeamId = GetGenericTeamId();
    FGenericTeamId OtherTeamId = TeamAgent->GetGenericTeamId();

    // If either team is invalid, they are neutral.
    if (OtherTeamId == FGenericTeamId::NoTeam || MyTeamId == FGenericTeamId::NoTeam)
    {
        return ETeamAttitude::Neutral;
    }

    // Main logic: if teams are different, they are hostile. If they are the same, they are friendly.
    if (OtherTeamId == MyTeamId)
    {
        return ETeamAttitude::Friendly;
    }
    else
    {
        return ETeamAttitude::Hostile;
    }
}


void AMyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    // Check if the stimulus was successfully sensed (i.e., we saw an enemy)
    if (Stimulus.WasSuccessfullySensed())
    {
        // We have seen an ENEMY. Set them as the "Player" in the blackboard.
        GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), Actor);
    }
    else
    {
        // We have LOST sight of an actor. Check if it's the one we were targeting.
        UObject* CurrentTarget = GetBlackboardComponent()->GetValueAsObject(TEXT("Player"));
        if (CurrentTarget == Actor)
        {
            // It is the one we were targeting, so clear the value.
            GetBlackboardComponent()->ClearValue(TEXT("Player"));
        }
    }
}



//// OLD CODE
//
//
//#include "MyAIController.h"
//#include "AICharacter.h"
//#include "BehaviorTree/BehaviorTree.h"
//#include "BehaviorTree/BlackboardComponent.h"
//#include "BehaviorTree/BehaviorTreeComponent.h"
//#include "Kismet/GameplayStatics.h"
//
//AMyAIController::AMyAIController()
//{
//    // Create the perception component
//    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
//
//    // Create and configure the sight sense
//    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
//    SightConfig->SightRadius = 2000.0f;
//    SightConfig->LoseSightRadius = 1500.0f;
//    SightConfig->PeripheralVisionAngleDegrees = 90.0f;
//    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
//    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
//    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
//
//    // Configure and dominate the perception component with the sight sense
//    AIPerceptionComponent->ConfigureSense(*SightConfig);
//    AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
//
//    // Bind the perception update function
//    AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AMyAIController::OnPerceptionUpdated);
//}
//
//void AMyAIController::OnPossess(APawn* InPawn)
//{
//    Super::OnPossess(InPawn);
//
//    // Start the behavior tree
//    if (BehaviorTree)
//    {
//        RunBehaviorTree(BehaviorTree);
//    }
//}
//
////void AMyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
////{
////    for (AActor* Actor : UpdatedActors)
////    {
////        // Check if the perceived actor has the "Player" tag
////        if (Actor->ActorHasTag(TEXT("Player")))
////        {
////            // Get the blackboard component
////            UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
////            if (BlackboardComponent)
////            {
////                // Set the "Player" key to the perceived player actor
////                BlackboardComponent->SetValueAsObject(TEXT("Player"), Actor);
////                return; // Exit the function since we found the player
////            }
////        }
////    }
////
////    // If the loop completes and no actor with the "Player" tag was found,
////    // clear the "Player" key. This happens when the player goes out of sight.
////    UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
////    if (BlackboardComponent)
////    {
////        BlackboardComponent->ClearValue(TEXT("Player"));
////    }
////}
//
//void AMyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
//{
//    for (AActor* Actor : UpdatedActors)
//    {
//        // Check if the perceived actor is the player
//        if (Actor->IsA(ACharacter::StaticClass()) && Actor != GetPawn())
//        {
//            // Get the blackboard component
//            UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
//            if (BlackboardComponent)
//            {
//                // Set the "Player" key to the perceived player actor
//                BlackboardComponent->SetValueAsObject(TEXT("Player"), Actor);
//                return;
//            }
//        }
//    }
//
//    // If no player is found, clear the "Player" key
//    UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
//    if (BlackboardComponent)
//    {
//        BlackboardComponent->ClearValue(TEXT("Player"));
//    }
//}


