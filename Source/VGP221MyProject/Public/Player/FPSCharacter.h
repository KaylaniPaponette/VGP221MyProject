// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Blueprint/UserWidget.h" // Required for UUserWidget
#include "Projectile/FPSProjectile.h"
#include "FPSCharacter.generated.h"

UCLASS()
class VGP221MYPROJECT_API AFPSCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Health property, editable in the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float Health = 100.0f;

	// Override the TakeDamage function
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* FPSMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AFPSProjectile> ProjectileClass;

	UFUNCTION()
	void MoveFoward(float value);

	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void EndJump();

	UFUNCTION()
	void Fire();

public:
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FGenericTeamId TeamId;

protected:
	// The C++ class of our HUD, assignable in the Blueprint editor
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDClass;

private:
	// The instance of our HUD
	UPROPERTY()
	UUserWidget* PlayerHUD;

	// --- Dash Mechanic Variables ---

	UPROPERTY(EditAnywhere, Category = "Movement|Dash")
	float DashDistance = 5000.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Dash")
	float DashCooldown = 1.f;

	bool bCanDash = true;

	// Timer handle for the dash cooldown
	FTimerHandle DashCooldownTimerHandle;

	// --- Dash Functions ---

	void OnDash();
	void ResetDash();

};
