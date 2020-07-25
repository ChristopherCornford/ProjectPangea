// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CableComponent.h"
#include "TrapClass.h"
#include "TrapBehaviour.h"
#include "ScanForNearbyAnimals.h"
#include "TamingTraps.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPANGEA_API UTamingTraps : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTamingTraps();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//TArray<TrapClass> SpawnedTrapList;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> BasicTrapActorToSpawn;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> BasicThrownTrapActorToSpawn;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> CableTrapActorToSpawn;

	TrapType TypeOfTrapToSpawn;

	AActor* CurrentlyPreparedThrownTrap;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	

	void UseRopeLasso();
	void UseRopeNetThrow();
	void UseRopeNetTrap();
	void UseWoodenCage();

	void SpawnTrap(TrapType TrapTypeToSpawn, TSubclassOf<AActor> TrapToSpawn);
	void SpawnTrapNew(TrapType TrapTypeToSpawn, TSubclassOf<AActor> TrapToSpawn);
	void SpawnThrownTrap(TrapType TrapTypeToSpawn, TSubclassOf<AActor> TrapToSpawn);
	void LaunchThrownTrap(AActor* SpawnedTrap);
};
