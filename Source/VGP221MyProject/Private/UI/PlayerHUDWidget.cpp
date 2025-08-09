// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUDWidget.h"
#include "Player/FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

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