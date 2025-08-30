// ================ new code ================
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Logging/StructuredLog.h"
#include "VGP221GameModeBase.generated.h"

// Forward declare our key classes
class AKeyCollectible;
class AWinKeyCollectible;

/**
 * */
UCLASS()
class VGP221MYPROJECT_API AVGP221GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Called when a fleeing enemy is killed
	void FleeingEnemyKilled();

	// NEW: Called when a boss enemy is killed
	void BossEnemyKilled();

	// Called when the player dies
	void PlayerDied(AController* PlayerController);

	UFUNCTION(BlueprintPure, Category = "Game Rules")
	int32 GetCurrentKills() const { return FleeingEnemiesKilledCount; }

	UFUNCTION(BlueprintPure, Category = "Game Rules")
	int32 GetRequiredKills() const { return RequiredKills; }

protected:
	virtual void StartPlay() override;

	// Name of the map to load when the player dies
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	FName GameOverMapName;

	// NEW: Name of the map to load when the player wins
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	FName WinMapName;

private:
	// A reference to the key that opens the barrier
	UPROPERTY(VisibleInstanceOnly, Category = "Game Rules")
	AKeyCollectible* BarrierKeyToSpawn;

	// NEW: A reference to the key that wins the game
	UPROPERTY(VisibleInstanceOnly, Category = "Game Rules")
	AWinKeyCollectible* WinKeyToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 RequiredKills = 3;

	int32 FleeingEnemiesKilledCount = 0;

	bool bBarrierKeyHasSpawned = false;

	// NEW: A flag to ensure the boss key only spawns once
	bool bWinKeyHasSpawned = false;
};







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

//// ================ new code ================
//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/GameModeBase.h"
//#include "Logging/StructuredLog.h"
//#include "VGP221GameModeBase.generated.h"
//
//class AKeyCollectible;
//
///**
// * */
//UCLASS()
//class VGP221MYPROJECT_API AVGP221GameModeBase : public AGameModeBase
//{
//	GENERATED_BODY()
//
//public:
//	void FleeingEnemyKilled();
//	void PlayerDied(AController* PlayerController);
//
//	UFUNCTION(BlueprintPure, Category = "Game Rules")
//	int32 GetCurrentKills() const { return FleeingEnemiesKilledCount; }
//
//	UFUNCTION(BlueprintPure, Category = "Game Rules")
//	int32 GetRequiredKills() const { return RequiredKills; }
//
//protected:
//	virtual void StartPlay() override;
//
//	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
//	FName GameOverMapName;
//
//private:
//	// This is no longer editable, but we can see it in the World Settings for debugging.
//	UPROPERTY(VisibleInstanceOnly, Category = "Game Rules")
//	AKeyCollectible* KeyToSpawn;
//
//	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
//	int32 RequiredKills = 3;
//
//	int32 FleeingEnemiesKilledCount = 0;
//
//	bool bKeyHasSpawned = false;
//
//};