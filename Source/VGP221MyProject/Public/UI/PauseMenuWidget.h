// PauseMenuWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

UCLASS()
class VGP221MYPROJECT_API UPauseMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    // Native construct override
    virtual void NativeConstruct() override;

    // Button click handlers
    UFUNCTION()
    void OnResumeClicked();

    UFUNCTION()
    void OnMainMenuClicked();

    UFUNCTION()
    void OnQuitClicked();

    // Button properties
    UPROPERTY(meta = (BindWidget))
    class UButton* ResumeButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* MainMenuButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitButton;
};
