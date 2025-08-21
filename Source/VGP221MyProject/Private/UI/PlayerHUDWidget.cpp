// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUDWidget.h"
#include "Player/FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "VGP221GameModeBase.h" 


float UPlayerHUDWidget::GetPlayerHealthPercent() const
{
	// Get the player character
	AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PlayerCharacter)
	{
		// Calculate and return the health percentage
		return (PlayerCharacter->Health > 0.0f) ? (PlayerCharacter->Health / 100.0f) : 0.0f;
	}

	return 0.0f;
}

int32 UPlayerHUDWidget::GetEnemiesRemaining() const
{
	// Get a reference to the current game mode
	AVGP221GameModeBase* GameMode = Cast<AVGP221GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		// Calculate the difference
		const int32 KillsRemaining = GameMode->GetRequiredKills() - GameMode->GetCurrentKills();

		// Return the result, ensuring it doesn't go below zero
		return FMath::Max(0, KillsRemaining);
	}

	// If we can't get the game mode for some reason, return 0
	return 0;
}