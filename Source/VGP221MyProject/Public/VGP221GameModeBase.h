//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/GameModeBase.h"
//#include "Logging/StructuredLog.h"
//#include "VGP221GameModeBase.generated.h"
//
//
///**
// * 
// */
//UCLASS()
//class VGP221MYPROJECT_API AVGP221GameModeBase : public AGameModeBase
//{
//	GENERATED_BODY()
//	
//	virtual void StartPlay() override;
//};

// ================ new code ================
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Logging/StructuredLog.h"
#include "VGP221GameModeBase.generated.h"


/**
 * */
UCLASS()
class VGP221MYPROJECT_API AVGP221GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	// This function will be called from the player character when it dies
	void PlayerDied(AController* PlayerController);

protected:
	// This property will let you choose the game over map in the Blueprint editor
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	FName GameOverMapName;

	virtual void StartPlay() override;
};