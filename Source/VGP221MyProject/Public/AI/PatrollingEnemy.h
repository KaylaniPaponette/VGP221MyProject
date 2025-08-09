// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BaseAICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "PatrollingEnemy.generated.h"

/**
 * */
UCLASS()
class VGP221MYPROJECT_API APatrollingEnemy : public ABaseAICharacter
{
	GENERATED_BODY()

public:
	APatrollingEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<AActor*> PatrolPoints;

private:
	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

	virtual void OnStateEnter(EAIState State) override;
	virtual void OnStateUpdate(EAIState State, float DeltaTime) override;
	virtual void OnStateExit(EAIState State) override;

	int32 CurrentPatrolPointIndex = 0;

};
