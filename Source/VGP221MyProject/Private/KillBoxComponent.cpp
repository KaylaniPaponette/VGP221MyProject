// Fill out your copyright notice in the Description page of Project Settings.

#include "KillBoxComponent.h"
#include "Player/FPSCharacter.h" // Your player character header
#include "VGP221GameModeBase.h"   // Your game mode header
#include "GameFramework/Actor.h"  // Required for GetOwner()

// Sets default values for this component's properties
UKillBoxComponent::UKillBoxComponent()
{
    // We don't need this to tick
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UKillBoxComponent::BeginPlay()
{
    Super::BeginPlay();

    // 1. Get the actor that this component is attached to (our plane)
    AActor* Owner = GetOwner();
    if (Owner)
    {
        // 2. Bind our OnOwnerOverlap function to the owner's OnActorBeginOverlap event.
        // Now, whenever the plane is overlapped, our function will be called.
        Owner->OnActorBeginOverlap.AddDynamic(this, &UKillBoxComponent::OnOwnerOverlap);
    }
}

void UKillBoxComponent::OnOwnerOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    // Check if the overlapping actor is not null
    if (OtherActor)
    {
        // 1. Try to cast the actor to your FPSCharacter
        AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(OtherActor);

        // 2. If it's the player, then proceed
        if (PlayerCharacter)
        {
            // 3. Get the Game Mode
            AVGP221GameModeBase* GameMode = GetWorld()->GetAuthGameMode<AVGP221GameModeBase>();

            // 4. Call the PlayerDied function
            if (GameMode)
            {
                GameMode->PlayerDied(PlayerCharacter->GetController());
            }
        }
    }
}