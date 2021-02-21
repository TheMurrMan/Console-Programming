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
// Pass in float to set explode timer
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
	//Create particle system for visual effect
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionTemplate, GetActorLocation());

	//this determines what things we check for later in our overlap
	FCollisionObjectQueryParams queryParams;
	queryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	queryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	//radius of our area
	FCollisionShape collShape;
	collShape.SetSphere(500.0f);

	//determine whats in our radius
	TArray<FOverlapResult> outOverlaps;
	GetWorld()->OverlapMultiByObjectType(outOverlaps, GetActorLocation(), FQuat::Identity, queryParams, collShape);

	//For each result we check to see if we want to do something to it 
	for (FOverlapResult result : outOverlaps) 
	{
		//if the comp exists and has physics change the color
		UPrimitiveComponent* Overlap = result.GetComponent();
		if (Overlap && Overlap->IsSimulatingPhysics())
		{
			//Changing color to a target color
			UMaterialInstanceDynamic* MatInst = Overlap->CreateAndSetMaterialInstanceDynamic(0);
			if (MatInst) 
			{
				MatInst->SetVectorParameterValue("Color", targetColor);
			}
		}
	}
	//Destory this bomb
	Destroy();
}

// Called when the game starts or when spawned
void AFPSBombActor::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle Explode_TimeHandle;

	//When the bomb is spawned create timer for when it should explode
	GetWorldTimerManager().SetTimer(Explode_TimeHandle, this, &AFPSBombActor::Explode, explodeDelay);

	//starting color of bomb
	materialInst = meshComp->CreateAndSetMaterialInstanceDynamic(0);

	if (materialInst) 
	{
		currentColor = materialInst->K2_GetVectorParameterValue("Color");
	}
	//Random target color
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

