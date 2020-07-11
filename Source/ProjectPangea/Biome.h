// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Biome.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPANGEA_API UBiome : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBiome();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Biome Data", meta = (ClampMin = "-45.0", ClampMax = "35.0"))
	float AverageTemperatureHigh;

	UPROPERTY(EditAnywhere, Category = "Biome Data", meta = (ClampMin = "-45.0", ClampMax = "35.0"))
	float AverageTemperatureLow;

	UPROPERTY(EditAnywhere, Category = "Biome Data", meta = (ClampMin = "00.0", ClampMax = "100.0"))
	float AverageHumidity;

	UPROPERTY(EditAnywhere, Category = "Biome Data", meta = (ClampMin = "00.0", ClampMax = "80.0"))
	float AverageWindSpeed;
};
