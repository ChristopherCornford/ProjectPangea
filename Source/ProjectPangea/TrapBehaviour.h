// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CableComponent.h"
#include "AnimalMotion.h"
#include "TrapClass.h"
#include "TrapBehaviour.generated.h"

//enum TrapType { RopeNet, WoodenCage };

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPANGEA_API UTrapBehaviour : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTrapBehaviour();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	TrapType Type;

	bool IsTrapped = false;
	AActor* TrappedActor;
	FVector TrappedPos;

	bool IsReadied = false;
	FVector StartingPoint;
	FRotator InitRotation;

	float theta = 0.5;
	float g = -9.81f;
	float v = 40.0f;
	float PlayerToProjectileDistance;

	float RopeLassoExhaustionIncr = 5.0f;
	float RopeNetThrowExhaustionIncr = 5.0f;
	float RopeNetTrapExhaustionIncr = 5.0f;
	float RopeLegTrapExhaustionIncr = 5.0f;
	float WoodenCageExhaustionIncr = 5.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TrappedExhaustion(float ExhaustionIncrValue);

	void SetTrapType(TrapType InType);

	void SetIsReadied(bool InputBool);
	void SetStartingPoint(FVector InputStartingPoint);
	void SetInitRotation(FRotator InputInitRotation);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedActor,
			AActor* OtherActor, UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
