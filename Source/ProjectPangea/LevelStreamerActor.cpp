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
    ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    //if (OtherActor == (AActor*)MyCharacter && LevelToLoad != "")
    if (OtherActor == (AActor*)MyCharacter)
    {
        FString debugStr = FString(TEXT("Character volume overlap!"));
        UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, debugStr);

        isInZone = true;
        if (streamingVolumeData != NULL)
        {
            streamingVolumeData->currentVolume = this;
            streamingVolumeData->levelToLoad = LevelToLoad;
            streamingVolumeData->levelToUnload = LevelToUnload;
        }
        else
        {
            debugStr = FString(TEXT("Error: Streaming volumes data class not linked to streaming volume!"));
            UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, debugStr);
        }
    }
}



void ALevelStreamerActor::OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    //if (OtherActor == (AActor*)MyCharacter && LevelToLoad != "")
    if (OtherActor == (AActor*)MyCharacter)
    {
        FString debugStr = FString(TEXT("Character volume overlap ended!"));
        UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, debugStr);

        isInZone = false;
        if (streamingVolumeData != NULL)
        {
            // only delete saved current volume if it's this one
            if (streamingVolumeData->currentVolume == this)
            {
                streamingVolumeData->currentVolume = NULL;
                streamingVolumeData->levelToLoad = "";
                streamingVolumeData->levelToUnload = "";
            }
        }
        else
        {
            debugStr = FString(TEXT("Error: Streaming volumes data class not linked to streaming volume!"));
            UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, debugStr);
        }
    }
}

