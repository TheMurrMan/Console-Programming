// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Runtime/Online/HTTP/Public/http.h>

#include "MyHTTPActor.generated.h"


//class FHttpModule;
//class FHttpRequestPtr;
//class FHttpResponsePtr;

UCLASS()
class FPSGAME_API AMyHTTPActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyHTTPActor();
	FHttpModule* http;

	void OnRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void HTTPCall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
