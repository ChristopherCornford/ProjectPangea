// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
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
	
	float RopeNetTrapExhaustionIncr = 5.0f;
	float WoodenCageExhaustionIncr = 5.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetTrapType(TrapType InType);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedActor,
			AActor* OtherActor, UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
