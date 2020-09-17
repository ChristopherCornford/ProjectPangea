// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceGeneratorComponent.generated.h"

UENUM()
enum Resource
{
	Gold,
	Lumber,
	Ore,
	Food,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPANGEA_API UResourceGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UResourceGeneratorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Information")
	TEnumAsByte<Resource> GeneratedResource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Information")
	int CurrentResourceCache;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Information")
	int QuantityNeededToShip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Information")
	bool isReadyToShip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Information")
	AActor* ParentNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent Information")
	TArray<AActor*> IdleAgents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent Information")
	TArray<AActor*> WorkingAgents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Actors")
	TArray<AActor*> IdleResourcePoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Actors")
	TArray<AActor*> OccupiedResourcePoints;

public:

	UFUNCTION(BlueprintCallable)
	void GiveActorJob(AActor* actor);

	void MoveAgentToResourceLocation(AActor* agent, FVector location);

	void ShipResources();
};
