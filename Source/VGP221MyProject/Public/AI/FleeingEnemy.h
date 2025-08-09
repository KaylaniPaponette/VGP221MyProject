// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BaseAICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "FleeingEnemy.generated.h"

/**
 * */
UCLASS()
class VGP221MYPROJECT_API AFleeingEnemy : public ABaseAICharacter
{
	GENERATED_BODY()

public:
	AFleeingEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

private:
	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

	virtual void OnStateEnter(EAIState State) override;
	virtual void OnStateUpdate(EAIState State, float DeltaTime) override;
	virtual void OnStateExit(EAIState State) override;

	// Helper function to find a new flee location
	void FindNewFleeLocation();

};
