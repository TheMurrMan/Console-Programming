// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBombActor.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include "Engine/Engine.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AFPSBombActor::AFPSBombActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	explodeDelay = 2.0f;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	RootComponent = meshComp;
}

AFPSBombActor::AFPSBombActor(float time)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	explodeDelay = time;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	RootComponent = meshComp;
}

void AFPSBombActor::Explode()
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
			UMaterialInstanceDynamic* MatInst = Overlap->CreateAndSetMaterialInstanceDynamic(0);
			if (MatInst) 
			{
				MatInst->SetVectorParameterValue("Color", targetColor);
			}
		}
	}

	Destroy();
}

// Called when the game starts or when spawned
void AFPSBombActor::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle Explode_TimeHandle;

	GetWorldTimerManager().SetTimer(Explode_TimeHandle, this, &AFPSBombActor::Explode, explodeDelay);

	materialInst = meshComp->CreateAndSetMaterialInstanceDynamic(0);

	if (materialInst) 
	{
		currentColor = materialInst->K2_GetVectorParameterValue("Color");
	}
	targetColor = FLinearColor::MakeRandomColor();
}

// Called every frame
void AFPSBombActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float progress = (GetWorld()->TimeSeconds - CreationTime)/explodeDelay;
	FLinearColor newColor = FLinearColor::LerpUsingHSV(currentColor,targetColor,progress);
	if (materialInst) 
	{
		materialInst->SetVectorParameterValue("Color", newColor);
	}
}

