// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimSequence.h"


AFPSCharacter::AFPSCharacter()
{
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1PComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1PComponent->SetupAttachment(CameraComponent);
	Mesh1PComponent->CastShadow = false;
	Mesh1PComponent->SetRelativeRotation(FRotator(2.0f, -15.0f, 5.0f));
	Mesh1PComponent->SetRelativeLocation(FVector(0, 0, -160.0f));

	// Create a gun mesh component
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(Mesh1PComponent, "GripPoint");
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);

	//Bind actions to simulate holding the charge key
	PlayerInputComponent->BindAction("Charge", IE_Pressed, this, &AFPSCharacter::Charging);
	PlayerInputComponent->BindAction("Charge", IE_Released, this, &AFPSCharacter::Charging);

	//PlayerInputComponent->BindAction("SpawnBomb", IE_Pressed, this, &AFPSCharacter::SpawnBomb);


	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

//Swapping function for checking charging
void AFPSCharacter::Charging() 
{

	//FTimerHandle timer;
	FTimerDelegate timerDel;
	
	float RandomScale = FMath::RandRange(1.0f, 10.0f);

	timerDel.BindUFunction(this, FName("ChargedFire"), RandomScale);

	//GetWorld()->GetTimerManager().SetTimer(timer, timerDel, 3.0f, false);
	//if we are currently charging clear the timer for charged projectile
	if (charging)
	{
		//GetWorldTimerManager().ClearTimer(Charger_TimeHandle);
		GetWorld()->GetTimerManager().ClearTimer(Charger_TimeHandle);
		charging = false;
	}
	else // if we arent charging start charging
	{
		//GetWorldTimerManager().SetTimer(Charger_TimeHandle, this, &AFPSCharacter::ChargedFire, chargedDelay);	
		GetWorld()->GetTimerManager().SetTimer(Charger_TimeHandle, timerDel, 3.0f, false);
		charging = true;
	}	
}

void AFPSCharacter::Fire()
{
	// try and fire a projectile
	if (ProjectileClass)
	{
		// Grabs location from the mesh that must have a socket called "Muzzle" in his skeleton
		FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
		// Use controller rotation which is our view direction in first person
		FRotator MuzzleRotation = GetControlRotation();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// spawn the projectile at the muzzle
		AFPSProjectile* spawnedProj = GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
		spawnedProj->ChangeProjSpeed(APIProjSpeed);
	}

	// try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
		}
	}
}

void AFPSCharacter::storeAPIData(float val1, float val2) 
{
	APIProjSpeed = val1;
	APISecondVar = val2;
}


//Made a copy of the fire function but changed to a charged projectile being spawned
void AFPSCharacter::ChargedFire(float scale)
{
	// try and fire a projectile
	if (ChargedProjectileClass)
	{
		// Grabs location from the mesh that must have a socket called "Muzzle" in his skeleton
		FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
		// Use controller rotation which is our view direction in first person
		FRotator MuzzleRotation = GetControlRotation();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// spawn the projectile at the muzzle
		AFPSProjectile* spawnedProj = GetWorld()->SpawnActor<AFPSProjectile>(ChargedProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
		//spawnedProj->ChangeProjSpeed(APIProjSpeed);
	}

	// try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
		}
	}
}

void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}


void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

//Save for reference Later
//UE_LOG(LogTemp, Warning, TEXT("cur Timer = %f") ,elapsed);