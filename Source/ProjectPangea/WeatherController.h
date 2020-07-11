// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DirectionalLight.h"
#include "WeatherController.generated.h"

UCLASS()
class PROJECTPANGEA_API AWeatherController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Day / Night Settings")
	ADirectionalLight* Sun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day / Night Settings", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float SunPosition;

	UPROPERTY(EditAnywhere, Category = "Day / Night Settings")
	float SunAngle;

	UPROPERTY(EditAnywhere, Category = "Day / Night Settings")
	float SunSpeed;

	UPROPERTY(EditAnywhere, Category = "Day / Night Settings")
	int Hour;

	UPROPERTY(EditAnywhere, Category = "Climate Control")
	bool isDaytime;

	UPROPERTY(EditAnywhere, Category = "Testing")
	float MinuteToHourRatio;

	UFUNCTION(BlueprintSetter, Category = "Day / Night Settings")
	void SetSunPosition(float newPosition);

	void MoveSunWorldPosition(float DeltaTime);
};
