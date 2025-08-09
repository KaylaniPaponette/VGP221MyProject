// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BaseAICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "TurretEnemy.generated.h"

/**
 * */
UCLASS()
class VGP221MYPROJECT_API ATurretEnemy : public ABaseAICharacter
{
	GENERATED_BODY()

public:
	ATurretEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<class AFPSProjectile> ProjectileClass;

private:
	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

	virtual void OnStateEnter(EAIState State) override;
	virtual void OnStateUpdate(EAIState State, float DeltaTime) override;
	virtual void OnStateExit(EAIState State) override;

	void Fire();

	FTimerHandle FireRateTimerHandle;
	float FireRate = 1.0f;
	bool bCanFire = true;

};
