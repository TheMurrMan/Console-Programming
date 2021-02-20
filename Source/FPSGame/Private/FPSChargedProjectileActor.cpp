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
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionTemplate, GetActorLocation());

	FCollisionObjectQueryParams queryParams;
	queryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	queryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	FCollisionShape collShape;
	collShape.SetSphere(500.0f);

	TArray<FOverlapResult> outOverlaps;
	GetWorld()->OverlapMultiByObjectType(outOverlaps, GetActorLocation(), FQuat::Identity, queryParams, collShape);

	for (FOverlapResult result : outOverlaps)
	{
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
	ChargedExplode();
}

// Called every frame
void AFPSChargedProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

