// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BaseAICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "BossEnemy.generated.h"

class UAnimMontage;
class AFPSProjectile;

UCLASS()
class VGP221MYPROJECT_API ABossEnemy : public ABaseAICharacter
{
	GENERATED_BODY()

public:
	ABossEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	// We only need one attack montage since we're reusing the animation
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* AttackMontage;

	// Projectile to use in the ranged phase
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<AFPSProjectile> ProjectileClass;

private:
	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

	virtual void OnStateEnter(EAIState State) override;
	virtual void OnStateUpdate(EAIState State, float DeltaTime) override;
	virtual void OnStateExit(EAIState State) override;

	// Checks health and sets the correct stage/state
	void UpdateStage();

	// Attack functions
	void MeleeAttack();
	void RangedAttack();
	void ResetAttack();

	// Boss properties, editable in the Blueprint
	UPROPERTY(EditAnywhere, Category = "Boss Attributes")
	float MaxHealth = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Boss Attributes")
	float MeleeDamage = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Boss Attributes")
	float MeleeAttackRate = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Boss Attributes")
	float RangedAttackRate = 1.5f;

	bool bCanAttack = true;
	int32 Stage = 1;

	FTimerHandle AttackTimerHandle;
};