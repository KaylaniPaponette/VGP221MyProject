// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAICharacter.h"
#include "UI/EnemyHealthBarWidget.h" 
#include "VGP221GameModeBase.h" // Include the Game Mode header
#include "Kismet/GameplayStatics.h" // Needed for GetAuthGameMode
#include "AI/FleeingEnemy.h" // Include the FleeingEnemy header to check its type

// Sets default values
ABaseAICharacter::ABaseAICharacter()
{
 	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
	TeamId = FGenericTeamId(1); // Set team ID for AI

	// Create the widget component
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // Set to face the camera
	HealthBarWidgetComponent->SetDrawSize(FVector2D(150, 20)); // Size of the health bar
	HealthBarWidgetComponent->SetVisibility(false); // Hide the health bar by default
}

// Called when the game starts or when spawned
void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();
	SetState(EAIState::Idle);

	// Set the owner of the widget so it can get health
	if (UEnemyHealthBarWidget* HealthBarWidget = Cast<UEnemyHealthBarWidget>(HealthBarWidgetComponent->GetUserWidgetObject()))
	{
		HealthBarWidget->SetOwnerActor(this);
	}
	
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

	if (Health > 0.0f)
	{
		HealthBarWidgetComponent->SetVisibility(true);
	}
	else // Health is 0 or less, the AI is dead
	{
		HealthBarWidgetComponent->SetVisibility(false);

		// --- NEW CODE START ---
		// Get the current game mode
		AVGP221GameModeBase* GameMode = Cast<AVGP221GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
		{
			// Check if the character that just died is a FleeingEnemy
			if (this->IsA<AFleeingEnemy>())
			{
				// If it is, tell the game mode
				GameMode->FleeingEnemyKilled();
			}
		}
		// --- NEW CODE END ---

		Destroy(); // Destroy the actor
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
