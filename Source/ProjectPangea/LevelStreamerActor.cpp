// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamerActor.h"

// Sets default values
ALevelStreamerActor::ALevelStreamerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
    RootComponent = OverlapVolume;

    OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALevelStreamerActor::OverlapBegins);
    OverlapVolume->OnComponentEndOverlap.AddUniqueDynamic(this, &ALevelStreamerActor::OverlapEnds);

    isInZone = false;
    //ALevelStreamerActor::isInZone = false;
}

// Called when the game starts or when spawned
void ALevelStreamerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelStreamerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ALevelStreamerActor::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    FString debugStr = FString(TEXT("Exit check!"));
    UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, debugStr);

    ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    if (OtherActor == (AActor*)MyCharacter && LevelToLoad != "")
    {
        isInZone = true;

        debugStr = FString(TEXT("Enter room!"));
        UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, debugStr);
    }
}



void ALevelStreamerActor::OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    FString debugStr = FString(TEXT("Exit check!"));
    UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, debugStr);

    ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    if (OtherActor == (AActor*)MyCharacter && LevelToLoad != "")
    {
        isInZone = false;

        debugStr = FString(TEXT("Room exited!"));
        UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, debugStr);
    }
}

