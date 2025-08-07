// -----------------------------------------------------------------------------
// MyAIController.h
// This header defines the AI's "brain" or controller.
// Now implements the IGenericTeamAgentInterface and defines team attitudes.
// -----------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h" // Include the team interface
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MyAIController.generated.h"

UCLASS()
class VGP221MYPROJECT_API AMyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AMyAIController();

    // Overridden from AAIController, called when this controller possesses a pawn.
    virtual void OnPossess(APawn* InPawn) override;

    // The Behavior Tree asset to use
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    class UBehaviorTree* BehaviorTree;

    // The perception component for sensing other actors
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAIPerceptionComponent* AIPerceptionComponent;

    // -- Team Interface Implementation --
    // We make the controller have a team so it can identify itself to the perception system.
    virtual FGenericTeamId GetGenericTeamId() const override;

    // Determines the attitude towards another actor based on team affiliation.
    virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;


private:
    // Configuration for the sight sense
    UAISenseConfig_Sight* SightConfig;

    // Function called when perception is updated with new stimuli.
    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};



//// OLD CODE
//
//#pragma once
//
//
//#include "CoreMinimal.h"
//#include "AIController.h"
//#include "Perception/AIPerceptionComponent.h"
//#include "Perception/AISenseConfig_Sight.h"
//#include "MyAIController.generated.h"
//
///**
// * 
// */
//UCLASS()
//class VGP221MYPROJECT_API AMyAIController : public AAIController
//{
//    GENERATED_BODY()
//
//public:
//    AMyAIController();
//
//    virtual void OnPossess(APawn* InPawn) override;
//
//    // The Behavior Tree asset to use
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
//    class UBehaviorTree* BehaviorTree;
//
//    // The perception component for sensing other actors
//    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
//    UAIPerceptionComponent* AIPerceptionComponent;
//
//private:
//    // Configuration for the sight sense
//    UAISenseConfig_Sight* SightConfig;
//
//    // Function called when perception is updated
//    UFUNCTION()
//    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
//};
