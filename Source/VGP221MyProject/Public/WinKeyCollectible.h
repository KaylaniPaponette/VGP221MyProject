// WinKeyCollectible.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WinKeyCollectible.generated.h"

// Forward declare classes
class USphereComponent;
class UStaticMeshComponent;
class AFPSCharacter;

UCLASS()
class VGP221MYPROJECT_API AWinKeyCollectible : public AActor
{
    GENERATED_BODY()

public:
    AWinKeyCollectible();

protected:
    virtual void BeginPlay() override;

public:
    // The visual mesh for the key
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* KeyMesh;

    // The trigger volume to detect the player
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* TriggerSphere;

    // The name of the map to open when the player collects this key
    UPROPERTY(EditInstanceOnly, Category = "Collectible")
    FName WinMapName;

    // Function to call when the player overlaps the trigger
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // Function to make the key appear in the world
    UFUNCTION(BlueprintCallable, Category = "Collectible")
    void ActivateCollectible();

private:
    // Function to hide the key at the start of the game
    void DeactivateCollectible();
};