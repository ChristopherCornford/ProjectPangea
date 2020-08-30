// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "PlayerStealthTracker.h"
#include "UpdatePlayerHidingState.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPANGEA_API UUpdatePlayerHidingState : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUpdatePlayerHidingState();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedActor,
			AActor* OtherActor, UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedActor,
			AActor* OtherActor, UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex);
};
