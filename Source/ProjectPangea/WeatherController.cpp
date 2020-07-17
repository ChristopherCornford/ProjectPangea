// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherController.h"

// Sets default values
AWeatherController::AWeatherController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeatherController::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AWeatherController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	isDaytime = (SunPosition >= 0) ? true : false;

	MoveSunWorldPosition(DeltaTime);
}

void AWeatherController::SetSunPosition(float newPosition)
{
	SunPosition = newPosition;
}

void AWeatherController::MoveSunWorldPosition(float DeltaTime)
{
	SunAngle = SunAngle + ((DeltaTime * SunSpeed) / (60 * MinuteToHourRatio)); // Base Speed is the velocity

	FRotator Rotator = FRotator::ZeroRotator;
	Rotator.Add(this->SunAngle, 0, 0);

	AActor* SunActor = Cast<AActor>(Sun);

	SunActor->SetActorRotation(Rotator);

	if (this->SunAngle >= 360)
	{
		SunAngle = 0;
	}

	Hour = SunAngle / 15 + 18;

	if (Hour >= 24)
		Hour -= 24;
}




