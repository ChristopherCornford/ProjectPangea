// Fill out your copyright notice in the Description page of Project Settings.


#include "EnviromentalZone.h"
#include "Engine/World.h"
#include "Biome.h"

// Sets default values
AEnviromentalZone::AEnviromentalZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AddAtmosphereComponents();
}

// Called when the game starts or when spawned
void AEnviromentalZone::BeginPlay()
{
	Super::BeginPlay();

	MyBiome = this->FindComponentByClass<UBiome>();

	UBiome* biome = Cast<UBiome>(MyBiome);
	
	LowerAtmosphereComponent->CurrentTemperature = (biome->AverageTemperatureHigh + biome->AverageTemperatureLow) / 2;
	LowerAtmosphereComponent->CurrentMoistureSaturation = biome->AverageHumidity;

	UpperAtmosphereComponent->CurrentTemperature = LowerAtmosphereComponent->CurrentTemperature / 2;
	UpperAtmosphereComponent->CurrentMoistureSaturation = LowerAtmosphereComponent->CurrentMoistureSaturation / 4;

	DailyHeatDifference = biome->AverageTemperatureHigh - biome->AverageTemperatureLow;

	DailySaturationDifference = biome->AverageHumidity * 0.25;
}

// Called every frame
void AEnviromentalZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (WeatherController->isDaytime)
		DaytimeHeating(DeltaTime);
	
}

void AEnviromentalZone::AddAtmosphereComponents()
{
	LowerAtmosphereComponent = CreateDefaultSubobject<UAtmosphere>(TEXT("Lower Atmposphere"));
	LowerAtmosphereComponent->SetHiddenInGame(false, false);
	RootComponent = LowerAtmosphereComponent;

	LowerAtmosphereComponent->SetAtmosphereType(0);

	UpperAtmosphereComponent = CreateDefaultSubobject<UAtmosphere>(TEXT("Upper Atmosphere"));
	UpperAtmosphereComponent->SetHiddenInGame(false, false);
	UpperAtmosphereComponent->AttachTo(LowerAtmosphereComponent);

	UpperAtmosphereComponent->SetAtmosphereType(1);

	FVector newPosition = FVector(0, 0, UpperAtmosphereComponent->BaseElevation) + (FVector)(this->GetActorLocation());

	UpperAtmosphereComponent->SetWorldLocation(newPosition);
}

void AEnviromentalZone::StartPrecipitation()
{
	onStartPrecipitation();

	isPrecipitating = true;

}

void AEnviromentalZone::StopPrecipitation()
{
	isPrecipitating = false;
}


void AEnviromentalZone::DaytimeHeating(float deltaTime)
{
	float solarTemperatureIn = (((DailyHeatDifference * 1.5) / 12.f) * deltaTime) / (60 * WeatherController->MinuteToHourRatio);

	// Heating the Lower Atmosphere
	LowerAtmosphereComponent->CurrentTemperature += solarTemperatureIn;


	// Heating the Upper Atmosphere
	UpperAtmosphereComponent->CurrentTemperature += solarTemperatureIn / 2;

	//Bleeding heat from Lower Atmosphere to Upper
	float bleedingTemperature = solarTemperatureIn * 0.1;

	UpperAtmosphereComponent->CurrentTemperature += bleedingTemperature;
	LowerAtmosphereComponent->CurrentTemperature -= bleedingTemperature;

	//Moisture Saturation increases
	if (LowerAtmosphereComponent->CurrentMoistureSaturation < 100)
		LowerAtmosphereComponent->CurrentMoistureSaturation += (DailySaturationDifference * deltaTime) / (60 * WeatherController->MinuteToHourRatio);
	else
	{
		UpperAtmosphereComponent->CurrentMoistureSaturation += (DailySaturationDifference * deltaTime) / (60 * WeatherController->MinuteToHourRatio);
	}

	LowerAtmosphereComponent->CurrentMoistureSaturation = (LowerAtmosphereComponent->CurrentMoistureSaturation >= 100) ? 100 : LowerAtmosphereComponent->CurrentMoistureSaturation;

	//Bleed Moisture into Upper Atmosphere
	if (UpperAtmosphereComponent->CurrentMoistureSaturation < 100)
		UpperAtmosphereComponent->CurrentMoistureSaturation += ((LowerAtmosphereComponent->CurrentMoistureSaturation * .1) * deltaTime) / (60 * WeatherController->MinuteToHourRatio);

	UpperAtmosphereComponent->CurrentMoistureSaturation = (UpperAtmosphereComponent->CurrentMoistureSaturation >= 100) ? 100 : UpperAtmosphereComponent->CurrentMoistureSaturation;

	CloudCoveragePercetage = UpperAtmosphereComponent->CurrentMoistureSaturation / 100;

	if (!isPrecipitating && CloudCoveragePercetage == 1)
		StartPrecipitation();
}

void AEnviromentalZone::NighttimeCooling(float deltaTime)
{

}

template<class T>
static T* GetComponentByClass(AActor* Actor)
{
	return Actor ? Cast<T>(Actor->GetComponentByClass(T::StaticClass())) : nullptr;
}