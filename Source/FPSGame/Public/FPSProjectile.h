// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSProjectile.generated.h"


class UProjectileMovementComponent;
class USphereComponent;
class AFPSBombActor;
class ASmallCubeActor;
class AFPSChargedProjectileActor;


UCLASS()
class AFPSProjectile : public AActor
{
	GENERATED_BODY()

protected:

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Projectile")
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	/** Bomb class reference */
	UPROPERTY(EditDefaultsOnly, Category = "Bombs")
	TSubclassOf<AFPSBombActor> BombClass;

	/** Small Cube class reference */
	UPROPERTY(EditDefaultsOnly, Category = "Cube")
	TSubclassOf<ASmallCubeActor> SmallCubeClass;

	/** Charged Projectile class reference */
	UPROPERTY(EditDefaultsOnly, Category = "ChargedProj")
	TSubclassOf<AFPSChargedProjectileActor> ChargedProjClass;



	float ProjSpeed = 3000.0f;

public:

	AFPSProjectile();
	AFPSProjectile(float speed);

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void ChangeProjSpeed(float val);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	/** Spawns a FPSBombActor  **/
	void SpawnBomb(FVector location);

	/** Spawns a SmallCubeActor **/
	void SpawnSmallerCube(FVector location, FVector scale);

	/** Spawns a FPSChargedProjectileActor **/
	void SpawnChargedExplosion(FVector location);
};

