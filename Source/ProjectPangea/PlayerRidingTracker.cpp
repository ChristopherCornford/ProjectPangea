// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRidingTracker.h"

// Sets default values for this component's properties
UPlayerRidingTracker::UPlayerRidingTracker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPlayerRidingTracker::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UPlayerRidingTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DidJustDismount)
	{
		DismountCount++;

		if (DismountCount > 3)
		{
			DismountCount = 0;
			SetDidJustDismount(false);
		}
	}
}

void UPlayerRidingTracker::SetIsPlayerRiding(bool InputBool)
{
	IsPlayerRiding = InputBool;
	if (InputBool)
	{
		UE_LOG(LogTemp, Log, TEXT("IsPlayerRiding = true"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("IsPlayerRiding = false"));
	}
}
bool UPlayerRidingTracker::GetIsPlayerRiding()
{
	return IsPlayerRiding;
}
void UPlayerRidingTracker::SetDidJustDismount(bool InputBool)
{
	DidJustDismount = InputBool;
}
bool UPlayerRidingTracker::GetDidJustDismount()
{
	return DidJustDismount;
}