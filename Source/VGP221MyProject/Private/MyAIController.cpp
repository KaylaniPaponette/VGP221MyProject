// -----------------------------------------------------------------------------
// MyAIController.cpp
// -----------------------------------------------------------------------------
#include "MyAIController.h"
#include "AICharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h" 
#include "GenericTeamAgentInterface.h" 

AMyAIController::AMyAIController()
{
    
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    // Configure the sight sense
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 2500.0f;
    SightConfig->LoseSightRadius = 2000.0f;
    SightConfig->PeripheralVisionAngleDegrees = 180.0f;

    
    // Configure AI to see actors as Enemies, Neutrals, or Friendlies based on their Team ID.
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    // Configure and dominate the perception component with the sight sense
    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

    // Bind the perception update function.
    AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMyAIController::OnTargetPerceptionUpdated);
}

void AMyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // When this controller possesses a pawn, adopt that pawn's team ID.
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
    // Return an invalid ID if no pawn is found.
    return FGenericTeamId::NoTeam;
}

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

    // If teams are different, they are hostile. If they are the same, they are friendly.
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
	// Check if enemy perception is updated
    if (Stimulus.WasSuccessfullySensed())
    {
		// We have SIGHT of an actor. Check if it's the one we want to track. set it in the blackboard.
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

