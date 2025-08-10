// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBarWidget.generated.h"

/**
 * */
UCLASS()
class VGP221MYPROJECT_API UEnemyHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// This function will be callable from the UMG Blueprint to get the owner's health percentage.
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetOwnerHealthPercent() const;

protected:
	// The owning actor of this widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	AActor* OwnerActor;

public:
	// Setter for the owner actor
	void SetOwnerActor(AActor* NewOwner) { OwnerActor = NewOwner; }

};
