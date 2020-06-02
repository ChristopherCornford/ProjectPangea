// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalSight.h"

// Sets default values for this component's properties
UAnimalSight::UAnimalSight()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UAnimalSight::BeginPlay()
{
	Super::BeginPlay();

	AnimalMotion = GetOwner()->FindComponentByClass<UAnimalMotion>();
	PlayerStats = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UPlayerStats>();

	FrameCount = IdleAttackRate;
}

// Called every frame
void UAnimalSight::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Output Exhaustion to log
	//UE_LOG(LogTemp, Log, TEXT("Exhaustion level: %.1f/%.1f"), AnimalMotion->GetExhaustion(), AnimalMotion->GetMaxExhaustion());

	//Output AnimalToPlayerDistance to log
	//UE_LOG(LogTemp, Log, TEXT("AnimalToPlayerDistance: %f"), AnimalMotion->GetAnimalToPlayerVector().Size());

	PlayerAngleFromSightCentre = CalcPlayerAngleFromSightCentre();

	if (AnimalMotion->GetIsTamed())
	{
		SetStatesToTamedSetup();
	}
	else
	{
		IfInUnawareState();
		IfInAlertedState();
		IfInExhaustedState();
	}
}

//General
float UAnimalSight::CalcPlayerAngleFromSightCentre()
{
	FVector CurrentAnimalFacingDir = GetOwner()->GetActorForwardVector();
	FVector AnimalToPlayerVector = AnimalMotion->GetAnimalToPlayerVector();

	float OutputAngle = AnimalMotion->CalcAngleFromDotProduct(CurrentAnimalFacingDir, AnimalToPlayerVector);
	return OutputAngle;
}

//Behaviour separated by animal alertness state
void UAnimalSight::SetStatesToTamedSetup()
{
	AnimalMotion->SetIsAlerted(false);
	AnimalMotion->SetIsExhausted(false);
	AnimalMotion->SetExhaustion(0.0f);
}
void UAnimalSight::IfInUnawareState()
{
	if (!AnimalMotion->GetIsAlerted() && !AnimalMotion->GetIsExhausted())
	{
		//Default to Alerted
		if ((PlayerAngleFromSightCentre < HalfAnimalSightAngle) &&
			(AnimalMotion->GetAnimalToPlayerVector().Size() < AnimalSightReach))
		{
			AnimalMotion->SetIsAlerted(true);
		}
	}
}
void UAnimalSight::IfInAlertedState()
{
	if (AnimalMotion->GetIsAlerted() && !AnimalMotion->GetIsExhausted())
	{
		//Fleeing/Chasing Motion
		if (CreatureType == Herbivore)
		{
			HerbivoreAlertBehaviour();
		}
		else if (CreatureType == Carnivore)
		{
			CarnivoreAlertBehaviour();
		}

		//Alerted to Exhausted
		if (AnimalMotion->GetExhaustion() >= AnimalMotion->GetMaxExhaustion())
		{
			//check this works - is comparative symbol right? - switching from int to float might be breaking this
			AnimalMotion->SetIsExhausted(true);
		}

		//Alerted back to Default
		if (AnimalMotion->GetAnimalToPlayerVector().Size() > AnimalMotion->GetAbandonHuntDistance())
		{
			AnimalMotion->SetIsAlerted(false);
			AnimalMotion->SetExhaustion(0.0f);
		}
	}
}
void UAnimalSight::IfInExhaustedState()
{
	if (AnimalMotion->GetIsAlerted() && AnimalMotion->GetIsExhausted())
	{
		//Decrease Exhaustion when Exhausted and recovering
		if (AnimalMotion->GetExhaustion() > 0.0f)
		{
			AnimalMotion->DecrementExhaustion();
		}

		//Exhausted back to Alerted
		if (AnimalMotion->GetExhaustion() <= 0.0f)
		{
			AnimalMotion->SetIsExhausted(false);
		}

		//Exhausted back to Default
		if (AnimalMotion->GetAnimalToPlayerVector().Size() > AnimalMotion->GetAbandonHuntDistance())
		{
			AnimalMotion->SetIsAlerted(false);
			AnimalMotion->SetIsExhausted(false);
			AnimalMotion->SetExhaustion(0.0f);
		}
	}
}

//Creature type-specific behaviour
void UAnimalSight::HerbivoreAlertBehaviour()
{
	if (AnimalMotion->GetAnimalToPlayerVector().Size() < AnimalMotion->GetTargetFleeDistance())
	{
		//Fleeing Movement
		AnimalMotion->FleeingAnimalMovement();

		//Fleeing Rotation
		AnimalMotion->FleeingAnimalRotation();

		//Increase Exhaustion when moving
		if (AnimalMotion->GetExhaustion() < AnimalMotion->GetMaxExhaustion())
		{
			AnimalMotion->IncrementExhaustion();
		}
	}
	else
	{
		//Decrease Exhaustion when stationary at TargetFleeDistance
		if (AnimalMotion->GetExhaustion() > 0.0f)
		{
			AnimalMotion->DecrementExhaustion();
		}
	}
}
void UAnimalSight::CarnivoreAlertBehaviour()
{
	if (AnimalMotion->GetAnimalToPlayerVector().Size() > AnimalMotion->GetTargetChasingDistance())
	{
		//Chasing Movement
		AnimalMotion->ChasingAnimalMovement();

		//Chasing Rotation
		AnimalMotion->ChasingAnimalRotation();

		//Increase Exhaustion when moving
		if (AnimalMotion->GetExhaustion() < AnimalMotion->GetMaxExhaustion())
		{
			AnimalMotion->IncrementExhaustion();
		}
	}
	else
	{
		//Decrease Exhaustion when stationary at TargetFleeDistance
		if (AnimalMotion->GetExhaustion() > 0.0f)
		{
			AnimalMotion->DecrementExhaustion();
		}

		//DamagingPlayer
		if (AnimalMotion->GetAnimalToPlayerVector().Size() < AnimalMotion->GetTargetChasingDistance() + 50.0f)
		{
			if (FrameCount > IdleAttackRate)
			{
				PlayerStats->DecrPlayerHealth(AnimalAttackDamage);
				FrameCount = 0;
			}
			FrameCount++;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("PlayerHealth: %f/%f"), PlayerStats->GetPlayerHealth(), PlayerStats->GetMaxPlayerHealth());
	UE_LOG(LogTemp, Log, TEXT("FrameCount: %d"), FrameCount);
}