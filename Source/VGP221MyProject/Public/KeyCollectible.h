// KeyCollectible.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyCollectible.generated.h"

// Forward declare classes
class USphereComponent;
class ABarrier;

UCLASS()
class VGP221MYPROJECT_API AKeyCollectible : public AActor
{
    GENERATED_BODY()

public:
    AKeyCollectible();

protected:
    virtual void BeginPlay() override;

public:
    // The visual mesh for the key
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* KeyMesh;

    // The trigger volume to detect the player
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* TriggerSphere;

    // This allows us to link this key to a specific barrier in the level
    UPROPERTY(EditInstanceOnly, Category = "Collectible")
    ABarrier* TargetBarrier;

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