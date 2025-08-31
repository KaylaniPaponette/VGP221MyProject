// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KillBoxComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VGP221MYPROJECT_API UKillBoxComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UKillBoxComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    // Function to handle the overlap event on the owner actor
    UFUNCTION()
    void OnOwnerOverlap(
        AActor* OverlappedActor,
        AActor* OtherActor
    );
};