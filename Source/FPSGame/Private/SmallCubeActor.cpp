// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallCubeActor.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include "Engine/Engine.h"

// Sets default values
ASmallCubeActor::ASmallCubeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//explodeDelay = 2.0f;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	RootComponent = meshComp;

}

// Called when the game starts or when spawned
void ASmallCubeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASmallCubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

