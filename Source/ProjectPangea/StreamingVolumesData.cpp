// Fill out your copyright notice in the Description page of Project Settings.


#include "StreamingVolumesData.h"

// Sets default values
AStreamingVolumesData::AStreamingVolumesData()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStreamingVolumesData::BeginPlay()
{
	Super::BeginPlay();
	
    int_queueCountdown = 0;
}

// Called every frame
void AStreamingVolumesData::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AStreamingVolumesData::queueUnload(FName inLevelToUnload)
{
    hasQueuedUpload = true;
    testQueing = true;
    int_queueCountdown = 2;
    levelToUnload_queued = inLevelToUnload;
}



void AStreamingVolumesData::clearQueue()
{
    hasQueuedUpload = false;
    //testQueing = false;
    int_queueCountdown = 0;
    levelToUnload_queued = "";
}

