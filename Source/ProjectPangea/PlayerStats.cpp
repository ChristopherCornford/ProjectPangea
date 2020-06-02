// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStats.h"

// Sets default values for this component's properties
UPlayerStats::UPlayerStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPlayerStats::BeginPlay()
{
	Super::BeginPlay();

	PlayerHealth = MaxPlayerHealth;
}


// Called every frame
void UPlayerStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//PlayerHealth functions
float UPlayerStats::GetPlayerHealth()
{
	return PlayerHealth;
}
void UPlayerStats::SetPlayerHealth(float InputFloat)
{
	PlayerHealth = InputFloat;
}
float UPlayerStats::GetMaxPlayerHealth()
{
	return MaxPlayerHealth;
}
void UPlayerStats::IncrPlayerHealth(float AdjustVal)
{
	PlayerHealth += AdjustVal;
	if (PlayerHealth > MaxPlayerHealth)
	{
		PlayerHealth = MaxPlayerHealth;
	}
}
void UPlayerStats::DecrPlayerHealth(float AdjustVal)
{
	PlayerHealth -= AdjustVal;
	if (PlayerHealth < 0.0f)
	{
		PlayerHealth = 0.0f;
	}
}