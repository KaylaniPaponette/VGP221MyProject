// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FPSProjectile.generated.h"

// Forward declare the Niagara component
class UNiagaraComponent;

UCLASS()
class VGP221MYPROJECT_API AFPSProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSProjectile();

public:
    UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleDefaultsOnly, Category = "Movement")
    class UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
    UStaticMeshComponent* ProjectileMeshComponent;

	// Niagara Component for projectile tracing
    UPROPERTY(VisibleAnywhere, Category = "Effects")
    UNiagaraComponent* TracerComponent;

    // Niagara System for the hit effect
    UPROPERTY(EditAnywhere, Category = "Effects")
    class UNiagaraSystem* ImpactEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float Damage = 25.0f;

    UFUNCTION()
    void FireInDirection(const FVector& ShoortDirection);

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
