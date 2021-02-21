// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSChargedProjectileActor.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include "Engine/Engine.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
// Sets default values
AFPSChargedProjectileActor::AFPSChargedProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	RootComponent = meshComp;
}

void AFPSChargedProjectileActor::ChargedExplode() 
{
	//Create particle system for visual effect
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionTemplate, GetActorLocation());

	//this determines what things we check for later in our overlap
	FCollisionObjectQueryParams queryParams;
	queryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	queryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	//radius of our area
	FCollisionShape collShape;
	collShape.SetSphere(750.0f);

	//determine whats in our radius
	TArray<FOverlapResult> outOverlaps;
	GetWorld()->OverlapMultiByObjectType(outOverlaps, GetActorLocation(), FQuat::Identity, queryParams, collShape);

	//For each result we check to see if we want to do something to it 
	for (FOverlapResult result : outOverlaps)
	{
		//if the comp exists and has physics destroy it 
		UPrimitiveComponent* Overlap = result.GetComponent();
		if (Overlap && Overlap->IsSimulatingPhysics())
		{
			result.GetActor()->Destroy();
		}
	}
}

// Called when the game starts or when spawned
void AFPSChargedProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	//Explode immediately
	ChargedExplode();
}

// Called every frame
void AFPSChargedProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

