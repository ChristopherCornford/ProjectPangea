// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalMotion.h"

// Sets default values for this component's properties
UAnimalMotion::UAnimalMotion()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAnimalMotion::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UAnimalMotion::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CalculateAnimalToPlayerVector();
}

//General
void UAnimalMotion::CalculateAnimalToPlayerVector()
{
	FVector CurrPlayerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector CurrAnimalPos = GetOwner()->GetActorLocation();
	AnimalToPlayerVector = CurrPlayerPos - CurrAnimalPos;
}

//Movement
void UAnimalMotion::TamedAnimalMovement()
{
	AnimalMovement("Tamed");
}
void UAnimalMotion::FleeingAnimalMovement()
{
	AnimalMovement("Fleeing");
}
void UAnimalMotion::ChasingAnimalMovement()
{
	AnimalMovement("Chasing");
}
void UAnimalMotion::AnimalMovement(FString MovementType)
{
	FVector UnitVector = MakeUnitVectorWithZeroZComponent(AnimalToPlayerVector);
	if (MovementType == "Tamed")
	{
		//Tamed
		if (AnimalToPlayerVector.Size() > TargetTamedDistance)
		{
			GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + (AnimalTamedSpeed * UnitVector));
		}
	}
	else if (MovementType == "Fleeing")
	{
		//Fleeing
		if (AnimalToPlayerVector.Size() < TargetFleeDistance)
		{
			GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() - (AnimalFleeSpeed * UnitVector));
		}
	}
	else if (MovementType == "Chasing")
	{
		//Chasing
		if (AnimalToPlayerVector.Size() > TargetChasingDistance)
		{
			GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + (AnimalTamedSpeed * UnitVector));
		}
	}
}

//Rotation
void UAnimalMotion::TamedAnimalRotation()
{
	AnimalRotation(1.0f);
}
void UAnimalMotion::FleeingAnimalRotation()
{
	AnimalRotation(-1.0f);
}
void UAnimalMotion::ChasingAnimalRotation()
{
	AnimalRotation(1.0f);
}
void UAnimalMotion::AnimalRotation(float DirectionMultiplier)
{
	//Vectors used
	//CHANGE TO FORWARDVECTOR
	FVector CurrentAnimalFacingDir = GetOwner()->GetActorForwardVector();
	FVector SignedAnimalToPlayerVector = DirectionMultiplier * AnimalToPlayerVector;

	//Angles used
	//Both measured from positive x axis (the starting front facing direction of the animal)
	//NEED TO MAKE THIS FIND THE INITIAL FACINGDIR, BY SAVING THE FORWARDVECTOR OF THE ANIMAL IN THE BEGIN PHASE
	float CurrentAnimalAngle = CalcAngleFromDotProduct(CurrentAnimalFacingDir, FVector(1.0f, 0.0f, 0.0f));
	float TargetAnimalAngle = CalcAngleFromDotProduct(SignedAnimalToPlayerVector, FVector(1.0f, 0.0f, 0.0f));

	//Differentiate between positive and negative angles
	CurrentAnimalAngle = MakeAnglePosOrNeg(CurrentAnimalFacingDir, -CurrentAnimalAngle, "Y");
	TargetAnimalAngle = MakeAnglePosOrNeg(SignedAnimalToPlayerVector, -TargetAnimalAngle, "Y");

	//Find difference between the two angles
	float AngleToTurn = CurrentAnimalAngle - TargetAnimalAngle;

	//Keep difference within -180 < x < 180 range
	AngleToTurn = KeepWithinAngleRange(AngleToTurn, 180.0f, -180.0f);

	//Calculate the turn direction (requires an angle range of -180 < x < 180)
	float TurnDirectionMultiplier = CalcTurnDirection(AngleToTurn);

	//Rotate animal
	if ((CurrentAnimalAngle < (TargetAnimalAngle - AnimalRotationLeniency))
		|| (CurrentAnimalAngle > (TargetAnimalAngle + AnimalRotationLeniency)))
	{
		UpdateAnimalRot(TurnDirectionMultiplier);
	}
}

//Functions used to calculate Rotation
float UAnimalMotion::RadiansToDegrees(float RadiansInput)
{
	float DegreesOutput = (RadiansInput * 180.0f) / UKismetMathLibrary::GetPI();
	return DegreesOutput;
}
float UAnimalMotion::CalcAngleFromDotProduct(FVector Input1, FVector Input2)
{
	float OutputAngle = UKismetMathLibrary::Acos(FVector::DotProduct(Input1, Input2) / (Input1.Size() * Input2.Size()));
	OutputAngle = RadiansToDegrees(OutputAngle);
	return OutputAngle;
}
float UAnimalMotion::MakeAnglePosOrNeg(FVector InputVector, float InputAngle, FString Axis)
{
	if (Axis == "X" || Axis == "x")
	{
		if (InputVector.X < 0)
		{
			InputAngle = -InputAngle;
		}
	}
	if (Axis == "Y" || Axis == "y")
	{
		if (InputVector.Y < 0)
		{
			InputAngle = -InputAngle;
		}
	}
	return InputAngle;
}
float UAnimalMotion::KeepWithinAngleRange(float InputAngle, float UpperLimit, float LowerLimit)
{
	float Range = UpperLimit - LowerLimit;
	while (InputAngle > UpperLimit)
	{
		InputAngle -= Range;
	}
	while (InputAngle < LowerLimit)
	{
		InputAngle += Range;
	}
	return InputAngle;
}
float UAnimalMotion::CalcTurnDirection(float InputAngle)
{
	float TurnDirectionMultiplier;
	if (InputAngle < 0.0f)
	{
		//ANTI-CLOCKWISE
		TurnDirectionMultiplier = -1.0f;
	}
	else if (InputAngle > 0.0f)
	{
		//CLOCKWISE
		TurnDirectionMultiplier = 1.0f;
	}
	return TurnDirectionMultiplier;
}
void UAnimalMotion::UpdateAnimalRot(float TurnDirectionMultiplier)
{
	GetOwner()->SetActorRotation(GetOwner()->GetActorRotation()
		+ (TurnDirectionMultiplier * FRotator(0.0f, AnimalRotationSpeed, 0.0f)));
}
bool UAnimalMotion::GetIsFinishedTurning()
{
	return IsFinishedTurning;
}

//Animal States
bool UAnimalMotion::GetIsTamed()
{
	return IsTamed;
}
bool UAnimalMotion::GetIsAlerted()
{
	return IsAlerted;
}
bool UAnimalMotion::GetIsExhausted()
{
	return IsExhausted;
}
bool UAnimalMotion::GetIsRiding()
{
	return IsRiding;
}
bool UAnimalMotion::GetIsGrounded()
{
	return IsGrounded;
}
void UAnimalMotion::SetIsTamed(bool InputBool)
{
	IsTamed = InputBool;
}
void UAnimalMotion::SetIsAlerted(bool InputBool)
{
	IsAlerted = InputBool;
}
void UAnimalMotion::SetIsExhausted(bool InputBool)
{
	IsExhausted = InputBool;
}
void UAnimalMotion::SetIsRiding(bool InputBool)
{
	IsRiding = InputBool;
}
void UAnimalMotion::SetIsGrounded(bool InputBool)
{
	IsGrounded = InputBool;
}

//Movement General
FVector UAnimalMotion::GetAnimalToPlayerVector()
{
	return AnimalToPlayerVector;
}
FVector UAnimalMotion::MakeUnitVectorWithZeroZComponent(FVector InputVector)
{
	FVector UnitVector = InputVector / FMath::Sqrt(FMath::Square(InputVector.X) + FMath::Square(InputVector.Y) + FMath::Square(InputVector.Z));
	UnitVector = FVector(UnitVector.X, UnitVector.Y, 0.0f);
	return UnitVector;
}

//Tamed
float UAnimalMotion::GetAnimalTamedSpeed()
{
	return AnimalTamedSpeed;
}

//Fleeing
float UAnimalMotion::GetTargetFleeDistance()
{
	return TargetFleeDistance;
}
float UAnimalMotion::GetAbandonHuntDistance()
{
	return AbandonHuntDistance;
}
float UAnimalMotion::GetExhaustion()
{
	return Exhaustion;
}
float UAnimalMotion::GetExhaustionIncrSpeed()
{
	return ExhaustionIncrSpeed;
}
float UAnimalMotion::GetExhaustionDecrSpeed()
{
	return ExhaustionDecrSpeed;
}
float UAnimalMotion::GetMaxExhaustion()
{
	return MaxExhaustion;
}
void UAnimalMotion::SetExhaustion(int InputInt)
{
	Exhaustion = InputInt;
}
void UAnimalMotion::IncrementExhaustion()
{
	Exhaustion += ExhaustionIncrSpeed;
}
void UAnimalMotion::DecrementExhaustion()
{
	Exhaustion -= ExhaustionDecrSpeed;
}

//Chasing
float UAnimalMotion::GetTargetChasingDistance()
{
	return TargetChasingDistance;
}