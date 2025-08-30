// ================ new code ================
// Fill out your copyright notice in the Description page of Project Settings.


#include "VGP221GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "KeyCollectible.h"
#include "WinKeyCollectible.h" // NEW: Include the new win key header

void AVGP221GameModeBase::StartPlay()
{
    // Always call the parent class's version first!
    Super::StartPlay();

    // --- Find the Barrier Key ---
    AActor* FoundBarrierKeyActor = UGameplayStatics::GetActorOfClass(GetWorld(), AKeyCollectible::StaticClass());
    if (FoundBarrierKeyActor)
    {
        BarrierKeyToSpawn = Cast<AKeyCollectible>(FoundBarrierKeyActor);
        if (BarrierKeyToSpawn)
        {
            UE_LOG(LogTemp, Warning, TEXT("GameMode automatically found and assigned the Barrier KeyCollectible."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GameMode did NOT find an AKeyCollectible (Barrier Key) actor in the level. This is okay if you don't need one for this level."));
    }

    // --- NEW: Find the Win Key ---
    AActor* FoundWinKeyActor = UGameplayStatics::GetActorOfClass(GetWorld(), AWinKeyCollectible::StaticClass());
    if (FoundWinKeyActor)
    {
        WinKeyToSpawn = Cast<AWinKeyCollectible>(FoundWinKeyActor);
        if (WinKeyToSpawn)
        {
            UE_LOG(LogTemp, Warning, TEXT("GameMode automatically found and assigned the WinKeyCollectible."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameMode could NOT find an AWinKeyCollectible actor in the level! A win key MUST be placed in the world."));
    }
}

void AVGP221GameModeBase::PlayerDied(AController* PlayerController)
{
    UE_LOG(LogTemp, Warning, TEXT("Player has died. Opening Game Over map."));

    if (PlayerController)
    {
        PlayerController->DisableInput(nullptr);
    }

    if (!GameOverMapName.IsNone())
    {
        UGameplayStatics::OpenLevel(GetWorld(), GameOverMapName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameOverMapName is not set in the Game Mode Blueprint!"));
    }
}

void AVGP221GameModeBase::FleeingEnemyKilled()
{
    if (bBarrierKeyHasSpawned) return;

    FleeingEnemiesKilledCount++;
    UE_LOG(LogTemp, Warning, TEXT("Fleeing Enemy Killed! Count: %d"), FleeingEnemiesKilledCount);

    if (FleeingEnemiesKilledCount >= RequiredKills && !bBarrierKeyHasSpawned)
    {
        if (BarrierKeyToSpawn)
        {
            BarrierKeyToSpawn->ActivateCollectible();
            bBarrierKeyHasSpawned = true;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("BarrierKeyToSpawn is not set in the Game Mode!"));
        }
    }
}

// NEW: Implementation for the boss killed logic
void AVGP221GameModeBase::BossEnemyKilled()
{
    // Don't spawn the key if it has already been spawned
    if (bWinKeyHasSpawned) return;

    UE_LOG(LogTemp, Warning, TEXT("Boss Enemy Killed! Spawning the Win Key."));

    // Check if the WinKeyToSpawn pointer is valid
    if (WinKeyToSpawn)
    {
        // Activate the win key collectible
        WinKeyToSpawn->ActivateCollectible();

        // Set the flag to true so this code never runs again for this game session
        bWinKeyHasSpawned = true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("WinKeyToSpawn is not set! Make sure a WinKeyCollectible is placed in the level."));
    }
}












//
//// ================ new code ================
//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "VGP221GameModeBase.h"
//#include "Kismet/GameplayStatics.h"
//#include "KeyCollectible.h" 
//
//
//void AVGP221GameModeBase::StartPlay()
//{
//    // Always call the parent class's version first!
//    Super::StartPlay();
//
//    // Find the first actor of the KeyCollectible class in the world
//    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AKeyCollectible::StaticClass());
//
//    // Check if we actually found something
//    if (FoundActor)
//    {
//        // Cast the found AActor to our specific AKeyCollectible type
//        KeyToSpawn = Cast<AKeyCollectible>(FoundActor);
//
//        // Check if the cast was successful
//        if (KeyToSpawn)
//        {
//            // Success! Print a message to the log so we know it worked.
//            UE_LOG(LogTemp, Warning, TEXT("GameMode automatically found and assigned the KeyCollectible."));
//        }
//    }
//    else
//    {
//        // If no actor was found, print an error. This helps with debugging.
//        UE_LOG(LogTemp, Error, TEXT("GameMode could NOT find an AKeyCollectible actor in the level! Make sure one is placed in the world."));
//    }
//}
//
//void AVGP221GameModeBase::PlayerDied(AController* PlayerController)
//{
//    UE_LOG(LogTemp, Warning, TEXT("Player has died. Opening Game Over map."));
//
//    // Disable the player's input so they can't move after dying
//    if (PlayerController)
//    {
//        PlayerController->DisableInput(nullptr);
//    }
//
//    // Open the specified game over map
//    if (!GameOverMapName.IsNone())
//    {
//        UGameplayStatics::OpenLevel(GetWorld(), GameOverMapName);
//    }
//    else
//    {
//        UE_LOG(LogTemp, Error, TEXT("GameOverMapName is not set in the Game Mode Blueprint!"));
//    }
//}
//
//void AVGP221GameModeBase::FleeingEnemyKilled()
//{
//
//    // Don't increment the counter if the task is already done
//    if (bKeyHasSpawned) return;
//
//    FleeingEnemiesKilledCount++;
//    UE_LOG(LogTemp, Warning, TEXT("Fleeing Enemy Killed! Count: %d"), FleeingEnemiesKilledCount);
//
//    // Check if the player has met the kill requirement
//    if (FleeingEnemiesKilledCount >= RequiredKills && !bKeyHasSpawned)
//    {
//        // Check if the KeyToSpawn pointer is valid (was set in the editor)
//        if (KeyToSpawn)
//        {
//            // Activate the collectible
//            KeyToSpawn->ActivateCollectible();
//
//            // Set the flag to true so this code never runs again
//            bKeyHasSpawned = true;
//
//            //// To prevent this from running again, we can set the count to a lower number
//            //FleeingEnemiesKilledCount = -100; // A simple way to ensure it only runs once
//        }
//        else
//        {
//            UE_LOG(LogTemp, Error, TEXT("KeyToSpawn is not set in the Game Mode!"));
//        }
//    }
//}


//void AVGP221GameModeBase::StartPlay()
//{
//    Super::StartPlay();
//
//    check(GEngine != nullptr);
//
//    //// Display a debug message for five seconds. 
//    //// The -1 "Key" value argument prevents the message from being updated or refreshed.
//    //// to print to screen
//    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Meow! this is my VGP221GameMode!"));
//
//    //// to print to log
//    //UE_LOG(LogTemp, Warning, TEXT("Meow! this is my VGP221GameMode! Console message"));
//
//    //// printing values to console
//    //int TestNumber = 10;
//    //bool BoolTest = false;
//    //FString NameTest("Your string here");
//    //UE_LOG(LogTemp, Warning, TEXT("TestNumber: %i, BoolTest: %d, NameTest: %s"), TestNumber, BoolTest, *NameTest);
//
//    //int* NumPoint = &TestNumber;
//    //// Will output a random number associated with memory address
//    //UE_LOG(LogTemp, Warning, TEXT("NumPoint: %i"), NumPoint);
//    //// Dereference to display the actual number
//    //UE_LOG(LogTemp, Warning, TEXT("NumPoint: %i"), *NumPoint);
//
//    //// 4. Modern way of debugging values
//    //UE_LOGFMT(LogTemp, Warning, "TestNumber: {0}, BoolTest: {1}, NameTest {2}", TestNumber, BoolTest, "Name Test");
//}
