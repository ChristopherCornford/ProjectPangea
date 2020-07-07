// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "TamingTracker.h"
#include "RidingPrecedence.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPANGEA_API URidingPrecedence : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URidingPrecedence();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UTamingTracker* TamingTracker;

	bool IsRequestInProgress = false;
	bool IsRequestCompleted = false;
	int BackUpIfListNotFull = 0;

	TArray<FVector> PosTagList;
	TArray<float> DistanceList;
	//TArray<bool> HasRequestedList;

	FVector RideAnimalPosTag;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddRideRequest(FVector PositionIdentifier, float PlayerDistance);
	void FindShortestDistance();
	FVector GetRideAnimal();
	bool GetIsRequestCompleted();
	void ResetAll();
	//int ScanForAndReturnRideableAnimalNo();
};
