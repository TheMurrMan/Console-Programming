// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include"FPSProjectile.h"
#include "FPSBombActor.h"
#include "SmallCubeActor.h"
#include "FPSChargedProjectileActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AFPSProjectile::AFPSProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

//Spawn a bomb actor
void AFPSProjectile::SpawnBomb(FVector location)
{
	AFPSBombActor* myBomb = GetWorld()->SpawnActor<AFPSBombActor>(BombClass, location, GetActorRotation());
}

//Spawn a charged smallerCube actor
void AFPSProjectile::SpawnSmallerCube(FVector location, FVector scale)
{
	ASmallCubeActor* myCube = GetWorld()->SpawnActor<ASmallCubeActor>(SmallCubeClass, location, GetActorRotation());
	myCube->FindComponentByClass<UPrimitiveComponent>()->SetWorldScale3D(scale);
}

//Spawn a charged explosion actor
void AFPSProjectile::SpawnChargedExplosion(FVector location)
{
	AFPSChargedProjectileActor* myCharged = GetWorld()->SpawnActor<AFPSChargedProjectileActor>(ChargedProjClass, location, GetActorRotation());
}

void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		//If we are have the Charged Projectile we dont create smaller cubes but instead use charged explosion
		if (this->ActorHasTag(TEXT("ChargedProjectile"))) 
		{
			SpawnChargedExplosion(OtherActor->GetActorLocation());
		}
		else 
		{
			//create bomb if it is a smaller cube
			if (OtherActor->ActorHasTag(TEXT("SmallCubeActor")))
			{
				SpawnBomb(OtherActor->GetActorLocation());
			}
			else // if not a small cube create four smaller cubes
			{
				FVector scale = OtherComp->GetComponentScale();

				scale *= 0.25f;

				SpawnSmallerCube(OtherActor->GetActorLocation(), scale);
				SpawnSmallerCube(OtherActor->GetActorLocation(), scale);
				SpawnSmallerCube(OtherActor->GetActorLocation(), scale);
				SpawnSmallerCube(OtherActor->GetActorLocation(), scale);
			}
		}	

		//Destroy the target
		OtherActor->Destroy();

		//Destroy the projectile after its hit
		Destroy();
	}
}

//Save for reference later
/*
		FVector scale = OtherComp->GetComponentScale();

		scale *= 0.25f;

		if (scale.GetMin() < 0.5f)
		{
			SpawnBomb(OtherActor->GetActorLocation());
			OtherActor->Destroy();
		}
		else
		{
			OtherComp->SetWorldScale3D(scale);
		}

		UMaterialInstanceDynamic* matPtr = OtherComp->CreateAndSetMaterialInstanceDynamic(0);

		if(matPtr)
		{
			matPtr->SetVectorParameterValue("Color", FLinearColor::Red);
		}
*/