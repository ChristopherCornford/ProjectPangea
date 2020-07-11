// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Atmosphere.generated.h"

UENUM()
enum AtmosphereType
{
	LowerAtmosphere,
	UpperAtmosphere,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPANGEA_API UAtmosphere : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAtmosphere();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere Location")
	TEnumAsByte<AtmosphereType> atmosphereType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atmosphere Location")
	float BaseElevation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climate", meta = (ClampMin = "-45.0", ClampMax = "35.0"))
	float CurrentTemperature;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climate", meta = (ClampMin = "00.0", ClampMax = "100.0"))
	float CurrentMoistureSaturation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climate", meta = (ClampMin = "00.0", ClampMax = "100.0"))
	float DewpointTemperature;

	float CalculateDewpointTemperature();

	void SetAtmosphereType(int index);
};
