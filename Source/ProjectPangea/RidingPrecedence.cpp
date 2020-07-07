// Fill out your copyright notice in the Description page of Project Settings.


#include "RidingPrecedence.h"

// Sets default values for this component's properties
URidingPrecedence::URidingPrecedence()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void URidingPrecedence::BeginPlay()
{
	Super::BeginPlay();	

	TamingTracker = GetOwner()->FindComponentByClass<UTamingTracker>();
}


// Called every frame
void URidingPrecedence::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsRequestInProgress)
	{
		//UE_LOG(LogTemp, Log, TEXT("BACKUP INT CHECK: %d"), BackUpIfListNotFull);
		if (BackUpIfListNotFull > 5) //(PosTagList.Num() == TamingTracker->GetTagNo()))
		{
			FindShortestDistance();
			IsRequestInProgress = false;
			IsRequestCompleted = true;
			BackUpIfListNotFull = 0;
		}
		BackUpIfListNotFull++;
	}
}

void URidingPrecedence::AddRideRequest(FVector PositionIdentifier, float PlayerDistance)
{
	IsRequestInProgress = true;
	PosTagList.Add(PositionIdentifier);
	DistanceList.Add(PlayerDistance);
	UE_LOG(LogTemp, Log, TEXT("Ride Request Added: (%f, %f)"), PositionIdentifier.X, PositionIdentifier.Y);
	UE_LOG(LogTemp, Log, TEXT("Dist = %f"), PlayerDistance);
}
void URidingPrecedence::FindShortestDistance()
{
	float ShortestDistance = DistanceList[0];
	int CorrespondingIndex = 0;
	for (int i = 1; i < DistanceList.Num(); i++)
	{
		if (DistanceList[i] < ShortestDistance)
		{
			ShortestDistance = DistanceList[i];
			CorrespondingIndex = i;
		}
	}
	RideAnimalPosTag = PosTagList[CorrespondingIndex];
	UE_LOG(LogTemp, Log, TEXT("ShortestDistance found for: (%f, %f)"), RideAnimalPosTag.X, RideAnimalPosTag.Y);
	UE_LOG(LogTemp, Log, TEXT("ShortestDistance = %f"), ShortestDistance);
}
FVector URidingPrecedence::GetRideAnimal()
{
	if (IsRequestCompleted)
	{
		return RideAnimalPosTag;
		UE_LOG(LogTemp, Log, TEXT("Correctly returning RideAnimalPosTag"))
	}
	else
	{
		return FVector(NULL);
		UE_LOG(LogTemp, Log, TEXT("Request Not Completed, returned NULL"));
	}
}
bool URidingPrecedence::GetIsRequestCompleted()
{
	//UE_LOG(LogTemp, Log, TEXT("GetIsRequestCompleted is called"));
	return IsRequestCompleted;
}
void URidingPrecedence::ResetAll()
{
	PosTagList.Empty();
	DistanceList.Empty();
	IsRequestCompleted = false;
	RideAnimalPosTag = FVector(NULL);
	UE_LOG(LogTemp, Log, TEXT("ALL VALUES RESET TO DEFAULT - Ready for next ride request"));
}
//int URidingPrecedence::ScanForAndReturnRideableAnimalNo()
//{
//	int RideableAnimalNo;
//	//FIND ALL ANIMALS WITHIN RIDE DISTANCE?
//	return RideableAnimalNo;
//}