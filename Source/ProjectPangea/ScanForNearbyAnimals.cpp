// Fill out your copyright notice in the Description page of Project Settings.


#include "ScanForNearbyAnimals.h"

// Sets default values for this component's properties
UScanForNearbyAnimals::UScanForNearbyAnimals()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UScanForNearbyAnimals::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &UScanForNearbyAnimals::OnOverlapBegin);
	GetOwner()->FindComponentByClass<UBoxComponent>()->OnComponentEndOverlap.AddDynamic(this, &UScanForNearbyAnimals::OnOverlapEnd);
}


// Called every frame
void UScanForNearbyAnimals::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UScanForNearbyAnimals::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//ADD CHECKS FOR ANIMAL INSTEAD OF ALL OBJECTS
	IsAnimalNearby = true;
	CurrentlyOverlappedAnimal = OtherActor;
}

void UScanForNearbyAnimals::OnOverlapEnd(UPrimitiveComponent* OverlappedActor,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex)
{
	IsAnimalNearby = false;
	CurrentlyOverlappedAnimal = NULL;
}

AActor* UScanForNearbyAnimals::GetCurrentlyOverlappedAnimal()
{
	return CurrentlyOverlappedAnimal;
}