// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

void AMainMenuGameMode::StartPlay()
{
    Super::StartPlay();

    if (MainMenuWidgetClass)
    {
        MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
        if (MainMenuWidget)
        {
            MainMenuWidget->AddToViewport();

            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC)
            {
                PC->SetShowMouseCursor(true);
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
                PC->SetInputMode(InputMode);
            }
        }
    }
}
