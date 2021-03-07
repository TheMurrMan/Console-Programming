// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHTTPActor.h"
// Sets default values
AMyHTTPActor::AMyHTTPActor()
{
    http = &FHttpModule::Get();
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMyHTTPActor::OnRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) 
{
    //Create a pointer to hold the json serialized data
    TSharedPtr<FJsonObject> JsonObject;

    //Create a reader pointer to read the json data
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

    //Deserialize the json data given Reader and the actual object to deserialize
    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
       
        TSharedPtr<FJsonObject> obj = JsonObject->GetObjectField("Time Series (Daily)");
        TSharedPtr<FJsonObject> day = obj->GetObjectField("2021-03-05");
        FString firstLine = day->GetStringField("1. open");
        FString secondLine = day->GetStringField("4. close");

        float open = FCString::Atof(*firstLine);
        float close = FCString::Atof(*secondLine);
        float final = open - close;
 
        FString finalString = FString::SanitizeFloat(final);

        GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, firstLine);
        GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Green, secondLine);
        GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Green, finalString);

        //FTimerDelegate timerDel;

        
        //float RandomScale = FMath::RandRange(1.0f, 10.0f);

        //timerDel.BindUFunction(ProjBP, FName("ChangeProjSpeed"), final);
        //ProjBP->ChangeProjSpeed(final);
        

        //UE_LOG(LogTemp, Warning, TEXT("Size = %d"), obj);
        //FString arrayString = objArray[0]->AsString();
        //GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, num);
        //Get the value of the json object by field name
        //TArray<TSharedPtr<FJsonValue>> objArray = JsonObject->GetStringField("Meta Data");
        /*
        for (int32 i = 0; i < objArray.Num(); i++)
        {
            FString name = objArray[i]->AsString();
            GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, name);
        }
        */
        //int32 recievedInt = JsonObject->GetIntegerField(objArray[0]->AsString());
        //JsonObject->GetStringField("Time Series (Daily)");
        //Output it to the engine
        //GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, objArray[0]->AsString());
    }
}

void AMyHTTPActor::HTTPCall() 
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &AMyHTTPActor::OnRecieved);
    //This is the url on which to process the request
    Request->SetURL("https://www.alphavantage.co/query?function=TIME_SERIES_DAILY_ADJUSTED&symbol=TSLA&apikey=KCBUJSWTC46XUIRC");
    Request->SetVerb("GET");
    Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
    Request->SetHeader("Content-Type", TEXT("application/json"));
    Request->ProcessRequest();
}

// Called when the game starts or when spawned
void AMyHTTPActor::BeginPlay()
{
    HTTPCall();
	Super::BeginPlay();
	
}

// Called every frame
void AMyHTTPActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

