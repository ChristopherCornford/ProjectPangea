// Fill out your copyright notice in the Description page of Project Settings.


#include "TamingTracker.h"

// Sets default values for this component's properties
UTamingTracker::UTamingTracker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTamingTracker::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UTamingTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTamingTracker::AddTamedAnimal()
{
	//TagTrackerList.Add(AnimalTag);
	TagNo++;
	UE_LOG(LogTemp, Log, TEXT("NEW TagNo: %d"), TagNo);
}

int UTamingTracker::GetTagNo()
{
	return TagNo;
}