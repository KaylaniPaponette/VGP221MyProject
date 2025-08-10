
// ================ new code ================
// Fill out your copyright notice in the Description page of Project Settings.


#include "VGP221GameModeBase.h"
#include "Kismet/GameplayStatics.h"

void AVGP221GameModeBase::PlayerDied(AController* PlayerController)
{
    UE_LOG(LogTemp, Warning, TEXT("Player has died. Opening Game Over map."));

    // Disable the player's input so they can't move after dying
    if (PlayerController)
    {
        PlayerController->DisableInput(nullptr);
    }

    // Open the specified game over map
    if (!GameOverMapName.IsNone())
    {
        UGameplayStatics::OpenLevel(GetWorld(), GameOverMapName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameOverMapName is not set in the Game Mode Blueprint!"));
    }
}


void AVGP221GameModeBase::StartPlay()
{
    Super::StartPlay();

    check(GEngine != nullptr);

    //// Display a debug message for five seconds. 
    //// The -1 "Key" value argument prevents the message from being updated or refreshed.
    //// to print to screen
    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Meow! this is my VGP221GameMode!"));

    //// to print to log
    //UE_LOG(LogTemp, Warning, TEXT("Meow! this is my VGP221GameMode! Console message"));

    //// printing values to console
    //int TestNumber = 10;
    //bool BoolTest = false;
    //FString NameTest("Your string here");
    //UE_LOG(LogTemp, Warning, TEXT("TestNumber: %i, BoolTest: %d, NameTest: %s"), TestNumber, BoolTest, *NameTest);

    //int* NumPoint = &TestNumber;
    //// Will output a random number associated with memory address
    //UE_LOG(LogTemp, Warning, TEXT("NumPoint: %i"), NumPoint);
    //// Dereference to display the actual number
    //UE_LOG(LogTemp, Warning, TEXT("NumPoint: %i"), *NumPoint);

    //// 4. Modern way of debugging values
    //UE_LOGFMT(LogTemp, Warning, "TestNumber: {0}, BoolTest: {1}, NameTest {2}", TestNumber, BoolTest, "Name Test");
}
