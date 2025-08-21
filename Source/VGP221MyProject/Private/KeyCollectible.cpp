// KeyCollectible.cpp

#include "KeyCollectible.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Player/FPSCharacter.h" // We need this to check if the overlapping actor is the player
#include "Barrier.h" // We need this to call the DisableBarrier function

AKeyCollectible::AKeyCollectible()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create the components
    KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
    RootComponent = KeyMesh;

    TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
    TriggerSphere->SetupAttachment(RootComponent);
    // Set up collision to only detect the player (Pawn)
    TriggerSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AKeyCollectible::BeginPlay()
{
    Super::BeginPlay();

    // Bind our overlap function to the trigger's event
    TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AKeyCollectible::OnOverlapBegin);

    // Hide the key when the game starts
    DeactivateCollectible();
}

void AKeyCollectible::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the actor that entered the trigger is the player character
    AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Key Collected by Player!"));
        // Check if we have linked a barrier in the editor
        if (TargetBarrier)
        {
            // Call the function on the barrier to disable it
            TargetBarrier->DisableBarrier();
        }

        // Destroy the key after it has been collected
        Destroy();
    }
}

void AKeyCollectible::ActivateCollectible()
{
    UE_LOG(LogTemp, Warning, TEXT("Key is now active!"));
    // Make the key visible
    SetActorHiddenInGame(false);
    // Turn on its collision so it can be picked up
    SetActorEnableCollision(true);
}

void AKeyCollectible::DeactivateCollectible()
{
    // Make the key invisible
    SetActorHiddenInGame(true);
    // Turn off its collision
    SetActorEnableCollision(false);
}