// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStealthTracker.h"

// Sets default values for this component's properties
UPlayerStealthTracker::UPlayerStealthTracker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPlayerStealthTracker::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UPlayerStealthTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerStealthTracker::SetIsPlayerHiding(bool InputBool)
{
	IsPlayerHiding = InputBool;
}
bool UPlayerStealthTracker::GetIsPlayerHiding()
{
	return IsPlayerHiding;
}

