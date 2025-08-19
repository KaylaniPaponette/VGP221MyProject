// PauseMenuWidget.cpp

#include "UI/PauseMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button click events
    if (ResumeButton)
    {
        ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
    }

    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnMainMenuClicked);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitClicked);
    }
}

void UPauseMenuWidget::OnResumeClicked()
{
    // Get the player controller
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        // Hide the pause menu
        this->RemoveFromParent();

        // Unpause the game
        UGameplayStatics::SetGamePaused(GetWorld(), false);

        // Set input mode back to game only
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = false;
    }
}

void UPauseMenuWidget::OnMainMenuClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
}

void UPauseMenuWidget::OnQuitClicked()
{
    // Get the player controller
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        // Quit the game
        PlayerController->ConsoleCommand("quit");
    }
}
