// In FPSProjectile.cpp

#include "Projectile/FPSProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
//  Include Niagara headers
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

AFPSProjectile::AFPSProjectile()
{
    PrimaryActorTick.bCanEverTick = false; // We don't need to tick

    // Setup the sphere collision
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->InitSphereRadius(15.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
    CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
    RootComponent = CollisionComponent;

    // Setup the movement component
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 1500.0f;
    ProjectileMovementComponent->MaxSpeed = 1500.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false;

    // Hide the old sphere mesh
    ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
    ProjectileMeshComponent->SetupAttachment(RootComponent);
    ProjectileMeshComponent->SetVisibility(false);
    ProjectileMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    //  SETUP THE NIAGARA TRACER COMPONENT
    TracerComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TracerComponent"));
    TracerComponent->SetupAttachment(RootComponent);

    InitialLifeSpan = 3.0f;
}

void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    //  SPAWN THE NIAGARA IMPACT EFFECT
    if (ImpactEffect)
    {
        // Use the Niagara function to spawn the system
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
    }

    // Destroy the projectile actor after it hits something
    Destroy();
}