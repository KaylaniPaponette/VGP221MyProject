// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FPSCharacter.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!FPSCameraComponent) {
		FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera")); // AddComponent in Unity
		FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
		FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
		FPSCameraComponent->bUsePawnControlRotation = true;
	}

	if (!FPSMesh) {
		FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
		FPSMesh->SetupAttachment(FPSCameraComponent);
		FPSMesh->bCastDynamicShadow = false;
		FPSMesh->CastShadow = false;
	}

	GetMesh()->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveFoward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	// Look
	PlayerInputComponent->BindAxis("LookHorizontal", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookVertical", this, &AFPSCharacter::AddControllerPitchInput);

	// Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::EndJump);

	// Fire
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
}

void AFPSCharacter::MoveFoward(float value)
{
	// 1. Unreal tutorial way
	// FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);

	FVector Direction = GetActorForwardVector();
	AddMovementInput(Direction, value);
}

void AFPSCharacter::MoveRight(float value)
{
	FVector Direction = GetActorRightVector();
	AddMovementInput(Direction, value);
}

void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::EndJump()
{
	bPressedJump = false;
}

//void AFPSCharacter::Fire()
//{// Implement firing logic here
//	if (!ProjectileClass) return;
//
//	// Init relevant infomration for where the projectile will be
//	FVector CameraLocation;
//	FRotator CameraRotation;
//	GetActorEyesViewPoint(CameraLocation, CameraRotation);
//
//	MuzzleOffset.Set(100.0f, 0.0f, 0.0f);
//
//	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
//
//	FRotator MuzzleRotation = CameraRotation;
//	MuzzleRotation.Pitch += 10.0f;
//
//	// Start of spawning the projectile
//	UWorld* World = GetWorld();
//	if (!World)  return;
//
//	FActorSpawnParameters SpawnParams;
//	SpawnParams.Owner = this;
//	SpawnParams.Instigator = GetInstigator();
//
//	// Unity Instantiate
//	AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
//	if (!Projectile) return;
//
//	// Launch spawned projectile in the camera rotation
//	FVector LaunchDirection = MuzzleRotation.Vector();
//	Projectile->FireInDirection(LaunchDirection);
//}
void AFPSCharacter::Fire()
{
	if (!ProjectileClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	// 1. Get camera transform
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	// 2. Calculate the projectile's spawn location first
	MuzzleOffset.Set(100.0f, 0.0f, 0.0f);
	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	// 3. Start and end points for our line trace (raycast)
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 10000.0f);

	// 4. Setup parameters to ignore the player character during the trace
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	// 5. Line trace
	FHitResult HitResult;
	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		CollisionParams
	);

	// 6. Projectile launch direction
	FVector LaunchDirection;
	if (bHit)
	{
		LaunchDirection = (HitResult.ImpactPoint - MuzzleLocation).GetSafeNormal();
	}
	else
	{
		// else fire straight from the camera's perspective
		LaunchDirection = CameraRotation.Vector();
	}

	// Use the calculated LaunchDirection for the projectile's rotation
	FRotator MuzzleRotation = LaunchDirection.Rotation();

	// 7. Spawn the projectile
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	if (Projectile)
	{
		// 8. Fire the projectile
		Projectile->FireInDirection(LaunchDirection);
	}
}
