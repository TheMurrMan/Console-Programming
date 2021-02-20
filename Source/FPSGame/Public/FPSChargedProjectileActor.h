// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSChargedProjectileActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem;

UCLASS()
class FPSGAME_API AFPSChargedProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSChargedProjectileActor();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* meshComp;
	UMaterialInstanceDynamic* materialInst;

	/** Sphere collision component */

	UPROPERTY(EditDefaultsOnly, Category = "ChargedActor")
	UParticleSystem* ExplosionTemplate;

	UFUNCTION()
	void ChargedExplode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
