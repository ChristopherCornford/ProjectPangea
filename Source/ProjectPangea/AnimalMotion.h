// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "AnimalMotion.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTPANGEA_API UAnimalMotion : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAnimalMotion();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//General
	FVector AnimalToPlayerVector;

	//Rotation
	UPROPERTY(EditAnywhere)
		float AnimalRotationSpeed = 4.0f;
	float AnimalRotationLeniency = 5.0f;
	bool IsFinishedTurning = false;

	//Animal States
	bool IsTamed = false;
	bool IsAlerted = false;
	bool IsExhausted = false;
	bool IsRiding = false;
	bool IsGrounded = true;
	bool IsTrapped = false;

	//Tamed
	UPROPERTY(EditAnywhere)
		float AnimalTamedSpeed = 9.0f;
	UPROPERTY(EditAnywhere)
		float TargetTamedDistance = 250.0f;

	//Fleeing
	UPROPERTY(EditAnywhere)
		float AnimalFleeSpeed = 8.0f;
	UPROPERTY(EditAnywhere)
		float TargetFleeDistance = 2000.0f;

	//Chasing
	UPROPERTY(EditAnywhere)
		float AnimalChasingSpeed = 8.0f;
	UPROPERTY(EditAnywhere)
		float TargetChasingDistance = 75.0f;

	//Exhaustion
	float Exhaustion = 0.0f;
	UPROPERTY(EditAnywhere)
		float ExhaustionIncrSpeed = 3.0f;
	UPROPERTY(EditAnywhere)
		float ExhaustionDecrSpeed = 2.0f;
	UPROPERTY(EditAnywhere)
		float MaxExhaustion = 1000.0f;

	//Other
	UPROPERTY(EditAnywhere)
		float AbandonHuntDistance = 4000.0f;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//General
	void CalculateAnimalToPlayerVector();

	//Movement
	void TamedAnimalMovement();
	void FleeingAnimalMovement();
	void ChasingAnimalMovement();
	void AnimalMovement(FString MovementType);

	//Rotation
	void TamedAnimalRotation();
	void FleeingAnimalRotation();
	void ChasingAnimalRotation();
	void AnimalRotation(float DirectionMultiplier);

	//Functions used to calculate Rotation
	float RadiansToDegrees(float RadiansInput);
	float CalcAngleFromDotProduct(FVector Input1, FVector Input2);
	float MakeAnglePosOrNeg(FVector InputVector, float InputAngle, FString Axis);
	float KeepWithinAngleRange(float InputAngle, float UpperLimit, float LowerLimit);
	float CalcTurnDirection(float InputAngle);
	void UpdateAnimalRot(float TurnDirectionMultiplier);
	bool GetIsFinishedTurning();

	//Animal States
	bool GetIsTamed();
	bool GetIsAlerted();
	bool GetIsExhausted();
	bool GetIsRiding();
	bool GetIsGrounded();
	bool GetIsTrapped();
	void SetIsTamed(bool InputBool);
	void SetIsAlerted(bool InputBool);
	void SetIsExhausted(bool InputBool);
	void SetIsRiding(bool InputBool);
	void SetIsGrounded(bool InputBool);
	void SetIsTrapped(bool InputBool);

	//Movement General
	FVector GetAnimalToPlayerVector();
	FVector MakeUnitVectorWithZeroZComponent(FVector InputVector);

	//Tamed
	float GetAnimalTamedSpeed();

	//Fleeing
	float GetTargetFleeDistance();
	float GetAbandonHuntDistance();
	float GetExhaustion();
	float GetExhaustionIncrSpeed();
	float GetExhaustionDecrSpeed();
	float GetMaxExhaustion();
	void SetExhaustion(float InputFloat);
	void IncrementExhaustion();
	void IncrementExhaustion(float ManualIncrValue);
	void DecrementExhaustion();

	//Chasing
	float GetTargetChasingDistance();
};
