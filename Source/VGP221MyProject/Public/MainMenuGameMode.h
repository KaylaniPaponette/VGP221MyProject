// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class VGP221MYPROJECT_API AMainMenuGameMode : public AGameModeBase
{
    GENERATED_BODY()

protected:
    virtual void StartPlay() override;

    // Widget class to use for the main menu UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;

    // Reference to the actual widget instance
    UPROPERTY()
    UUserWidget* MainMenuWidget;
};