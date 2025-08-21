// Barrier.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Barrier.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UNiagaraComponent; // <-- Add this forward declaration

UCLASS()
class VGP221MYPROJECT_API ABarrier : public AActor
{
	GENERATED_BODY()

public:
	ABarrier();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BarrierMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionComponent;

	// This component will hold our particle effect
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* NiagaraComponent; // <-- Add this property

	UFUNCTION(BlueprintCallable, Category = "Barrier")
	void DisableBarrier();
};