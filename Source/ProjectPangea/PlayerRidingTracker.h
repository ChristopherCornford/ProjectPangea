// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerRidingTracker.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTPANGEA_API UPlayerRidingTracker : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerRidingTracker();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	bool IsPlayerRiding = false;
	bool DidJustDismount = false;

	int DismountCount = 0;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetIsPlayerRiding(bool InputBool);
	bool GetIsPlayerRiding();
	void SetDidJustDismount(bool InputBool);
	bool GetDidJustDismount();
};
