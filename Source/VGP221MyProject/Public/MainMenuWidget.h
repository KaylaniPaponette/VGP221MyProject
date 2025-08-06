// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"


class UButton;

/**
 * 
 */
UCLASS()
class VGP221MYPROJECT_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    UFUNCTION()
    void OnPlayClicked();

    UFUNCTION()
    void OnQuitClicked();

    // Exposing buttons for Blueprints binding
    UPROPERTY(meta = (BindWidget))
    UButton* PlayButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;
};
