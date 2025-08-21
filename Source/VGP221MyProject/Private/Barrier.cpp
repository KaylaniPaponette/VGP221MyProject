// Barrier.cpp

#include "Barrier.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h" // <-- Add this include

ABarrier::ABarrier()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	BarrierMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrierMesh"));
	BarrierMesh->SetupAttachment(RootComponent);

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(BarrierMesh);
	CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	// Create the Niagara component and attach it
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent); // Attach it to the root
}

void ABarrier::OnConstruction(const FTransform& Transform)
{
	// Always call the parent class's version first
	Super::OnConstruction(Transform);

	// Make sure the Niagara Component is valid
	if (NiagaraComponent)
	{
		// Get the actor's current scale
		const FVector CurrentScale = GetActorScale3D();

		// Set the "User.Scale" parameter in our Niagara system to match the actor's scale.
		// The name "User.Scale" MUST exactly match the parameter we created in the Niagara editor.
		NiagaraComponent->SetVectorParameter("User.Scale", CurrentScale);
	}
}

void ABarrier::DisableBarrier()
{
	UE_LOG(LogTemp, Warning, TEXT("Barrier Disabled!"));

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	// Deactivate the particle system so it fades out and stops playing
	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
	}
}