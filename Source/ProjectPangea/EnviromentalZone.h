// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "WeatherController.h"
#include "Atmosphere.h"
#include "EnviromentalZone.generated.h"

UCLASS()
class PROJECTPANGEA_API AEnviromentalZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnviromentalZone();
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather Controller")
	AWeatherController* WeatherController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climate")
	UActorComponent* MyBiome;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Atmosphere Components")
	UAtmosphere* LowerAtmosphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Atmosphere Components")
	UAtmosphere* UpperAtmosphereComponent;

	UPROPERTY(EditAnywhere, Category = "Weather", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	FVector WindDirection;

	UPROPERTY(EditAnywhere, Category = "Weather")
	float WindSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float CloudCoveragePercetage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	bool isPrecipitating;

	float DailyHeatDifference;
	float DailySaturationDifference;

	void AddAtmosphereComponents();

	UFUNCTION(BlueprintCallable, Category = "Weather Debug")
	void StartPrecipitation();

	UFUNCTION(BlueprintCallable, Category = "Weather Debug")
	void StopPrecipitation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Weather Events")
	void onStartPrecipitation();

	UFUNCTION(BlueprintCallable, Category = "Weather Debug")
	void DaytimeHeating(float deltaTime);

	UFUNCTION(BlueprintCallable, Category = "Weather Debug")
	void NighttimeCooling(float deltaTime);


};
