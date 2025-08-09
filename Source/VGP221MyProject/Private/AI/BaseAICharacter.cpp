// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAICharacter.h"

// Sets default values
ABaseAICharacter::ABaseAICharacter()
{
	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
	TeamId = FGenericTeamId(1); // Set team ID for AI
}

// Called when the game starts or when spawned
void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();
	SetState(EAIState::Idle);

}

// Called every frame
void ABaseAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OnStateUpdate(CurrentState, DeltaTime);

}

// Called to bind functionality to input
void ABaseAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseAICharacter::SetState(EAIState NewState)
{
	if (CurrentState != NewState)
	{
		OnStateExit(CurrentState);
		CurrentState = NewState;
		OnStateEnter(CurrentState);
	}
}

float ABaseAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= ActualDamage;
	if (Health <= 0)
	{
		Destroy();
	}

	return ActualDamage;
}

void ABaseAICharacter::OnStateEnter(EAIState State)
{
	// Overridden by child classes
}

void ABaseAICharacter::OnStateUpdate(EAIState State, float DeltaTime)
{
}

void ABaseAICharacter::OnStateExit(EAIState State)
{
}
