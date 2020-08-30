// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdatePlayerHidingState.h"

// Sets default values for this component's properties
UUpdatePlayerHidingState::UUpdatePlayerHidingState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UUpdatePlayerHidingState::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &UUpdatePlayerHidingState::OnOverlapBegin);
	GetOwner()->FindComponentByClass<UBoxComponent>()->OnComponentEndOverlap.AddDynamic(this, &UUpdatePlayerHidingState::OnOverlapEnd);
}


// Called every frame
void UUpdatePlayerHidingState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UUpdatePlayerHidingState::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->FindComponentByClass<UPlayerStealthTracker>() != NULL)
	{
		OtherActor->FindComponentByClass<UPlayerStealthTracker>()->SetIsPlayerHiding(true);
	}
}

void UUpdatePlayerHidingState::OnOverlapEnd(UPrimitiveComponent* OverlappedActor,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex)
{
	if (OtherActor->FindComponentByClass<UPlayerStealthTracker>() != NULL)
	{
		OtherActor->FindComponentByClass<UPlayerStealthTracker>()->SetIsPlayerHiding(false);
	}
}