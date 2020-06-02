// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AnimalMotion.h"
#include "RideAnimal.generated.h"


UENUM()
enum AnimalFlying
{
	On UMETA(DisplayName = "On"),
	Off UMETA(DisplayName = "Off")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTPANGEA_API URideAnimal : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URideAnimal();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	APawn* PlayerPawn;
	ACharacter* PlayerCharacter;
	FVector PlayerPawnInitialPos;

	UAnimSequence* StandingAnimation;
	UAnimSequence* RidingAnimation;

	UPROPERTY(EditAnywhere)
		float RidingHeight = 130.0f;

	UPROPERTY(EditAnywhere)
		float RidingRegionDistance = 150.0f;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<AnimalFlying> AnimalFlying = Off;

	UPROPERTY(EditAnywhere)
		float AnimalFlyingSpeed = 30.0f;
	UPROPERTY(EditAnywhere)
		float MaximumFlyingDismountHeight = 300.0f;

	UAnimalMotion* AnimalMotion;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SaveInitialStates();
	void UpdateIsRiding();
	void SetupMountState();
	void SetInitialRiderRotation();
	void SetupDismountState();
	void SetDismountedPositions();
	void UpdateIsGrounded();
	void FlyingSpecificMotion();
	void UpdateRiderPosAndRot();
};