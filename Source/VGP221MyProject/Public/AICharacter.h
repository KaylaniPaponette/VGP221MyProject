// -----------------------------------------------------------------------------
// AICharacter.h
// This header defines the AI character that will exist in the world.
// Now implements the IGenericTeamAgentInterface for team affiliation.
// -----------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h" // Include the team interface
#include "AICharacter.generated.h"

UCLASS()
class VGP221MYPROJECT_API AAICharacter : public ACharacter, public IGenericTeamAgentInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AAICharacter();

    // An array of target points for the AI to patrol between
    UPROPERTY(EditAnywhere, Category = "AI")
    TArray<AActor*> PatrolPoints;

    // -- Team Interface Implementation --
    /**
     * Assigns a team ID to this character.
     * @param TeamID The new team ID to assign.
     */
    virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override { TeamId = TeamID; }

    /**
     * Retrieves the team ID for this character.
     * @return The team ID.
     */
    virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    // The team this AI belongs to.
    UPROPERTY(EditAnywhere, Category = "AI")
    FGenericTeamId TeamId;
};

////OLD CODE
// 
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
//#include "AICharacter.generated.h"
//
//UCLASS()
//class VGP221MYPROJECT_API AAICharacter : public ACharacter
//{
//    GENERATED_BODY()
//
//public:
//    // Sets default values for this character's properties
//    AAICharacter();
//
//    // An array of target points for the AI to patrol between
//    UPROPERTY(EditAnywhere, Category = "AI")
//    TArray<AActor*> PatrolPoints;
//
//protected:
//    // Called when the game starts or when spawned
//    virtual void BeginPlay() override;
//
//public:
//    // Called every frame
//    virtual void Tick(float DeltaTime) override;
//
//    // Called to bind functionality to input
//    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//};
//
