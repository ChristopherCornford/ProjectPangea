// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStats.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTPANGEA_API UPlayerStats : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerStats();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float PlayerHealth;
	UPROPERTY(EditAnywhere)
		float MaxPlayerHealth = 100.0f;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetPlayerHealth();
	void SetPlayerHealth(float InputFloat);
	float GetMaxPlayerHealth();
	void IncrPlayerHealth(float AdjustVal);
	void DecrPlayerHealth(float AdjustVal);
};
