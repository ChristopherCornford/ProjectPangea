// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AnimalMotion.h"
#include "PlayerStats.h"
#include "AnimalSight.generated.h"


UENUM()
enum CreatureType
{
	Herbivore UMETA(Display Name = "Herbivore"),
	Carnivore UMETA(Display Name = "Carnivore")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTPANGEA_API UAnimalSight : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAnimalSight();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float PlayerAngleFromSightCentre;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<CreatureType> CreatureType = Herbivore;

	//Angle size variables
	UPROPERTY(EditAnywhere)
		float HalfAnimalSightAngle = 30.0f;
	UPROPERTY(EditAnywhere)
		float AnimalSightReach = 1700.0f;

	//Attacking
	UPROPERTY(EditAnywhere)
		float AnimalAttackDamage = 10.0f;
	int FrameCount;
	UPROPERTY(EditAnywhere)
		int IdleAttackRate = 30;

	UAnimalMotion* AnimalMotion;
	UPlayerStats* PlayerStats;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//General
	float CalcPlayerAngleFromSightCentre();

	//Behaviour separated by animal alertness state
	void SetStatesToTamedSetup();
	void IfInUnawareState();
	void IfInAlertedState();
	void IfInExhaustedState();

	//Creature type-specific behaviour
	void HerbivoreAlertBehaviour();
	void CarnivoreAlertBehaviour();
};
