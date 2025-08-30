// WinKeyCollectible.cpp

#include "WinKeyCollectible.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Player/FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

AWinKeyCollectible::AWinKeyCollectible()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create the components
    KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
    RootComponent = KeyMesh;

    TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
    TriggerSphere->SetupAttachment(RootComponent);
    TriggerSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AWinKeyCollectible::BeginPlay()
{
    Super::BeginPlay();

    // Bind our overlap function to the trigger's event
    TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AWinKeyCollectible::OnOverlapBegin);

    // Hide the key when the game starts
    DeactivateCollectible();
}

void AWinKeyCollectible::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the actor that entered the trigger is the player character
    AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Win Key Collected by Player! Loading Win Screen."));

        // Open the specified win map
        if (!WinMapName.IsNone())
        {
            UGameplayStatics::OpenLevel(GetWorld(), WinMapName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("WinMapName is not set in the WinKeyCollectible actor!"));
        }

        // We can destroy the key, though the level is about to change anyway
        Destroy();
    }
}

void AWinKeyCollectible::ActivateCollectible()
{
    UE_LOG(LogTemp, Warning, TEXT("Win Key is now active!"));
    // Make the key visible
    SetActorHiddenInGame(false);
    // Turn on its collision so it can be picked up
    SetActorEnableCollision(true);
}

void AWinKeyCollectible::DeactivateCollectible()
{
    // Make the key invisible
    SetActorHiddenInGame(true);
    // Turn off its collision
    SetActorEnableCollision(false);
}