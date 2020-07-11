// Fill out your copyright notice in the Description page of Project Settings.


#include "Atmosphere.h"
#include "../Plugins/Experimental/AlembicImporter/Source/ThirdParty/Alembic/AlembicDeploy/include/ImathMath.h"

// Sets default values for this component's properties
UAtmosphere::UAtmosphere()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	

	// ...
}

// Called when the game starts
void UAtmosphere::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAtmosphere::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DewpointTemperature = CalculateDewpointTemperature();
	
	// ...
}

float UAtmosphere::CalculateDewpointTemperature()
{
	float constA = (log(CurrentMoistureSaturation / 100) + ((17.62f * CurrentTemperature) / (243.12f + CurrentTemperature)));
	float dewPoint = (243.12f * constA) / (17.62f - constA);

	return dewPoint;
}

void UAtmosphere::SetAtmosphereType(int index = 0)
{
	atmosphereType = (TEnumAsByte<AtmosphereType>)index;

	switch (atmosphereType)
	{
	case AtmosphereType::LowerAtmosphere:
		BaseElevation = 0.0;
		break;

	case AtmosphereType::UpperAtmosphere:
		BaseElevation = 50000.0;
		break;
	}
}
