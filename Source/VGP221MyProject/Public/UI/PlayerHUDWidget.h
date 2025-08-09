// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * */
UCLASS()
class VGP221MYPROJECT_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// This function will be callable from the UMG Blueprint to get the player's health percentage.
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetPlayerHealthPercent() const;

};
