// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Components/WidgetComponent.h" // Required for the health bar
#include "Perception/AIPerceptionTypes.h"
#include "BaseAICharacter.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Patrol,
	Chase,
	Attack,
	Flee,
	Idle,
	Boss_Melee,
	Boss_Ranged
};

UCLASS()
class VGP221MYPROJECT_API ABaseAICharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	EAIState GetCurrentState() const { return CurrentState; }

	//UFUNCTION()
	//virtual void SetAIState(EAIState NewState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsBoss = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	EAIState CurrentState = EAIState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float CurrentSpeed;

	// Enemy health bar widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthBarWidgetComponent;

	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual void SetState(EAIState NewState);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FGenericTeamId TeamId;

	virtual void OnStateEnter(EAIState State);
	virtual void OnStateUpdate(EAIState State, float DeltaTime);
	virtual void OnStateExit(EAIState State);

};
