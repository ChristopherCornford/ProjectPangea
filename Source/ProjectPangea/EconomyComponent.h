// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "EconomyComponent.generated.h"


UENUM()
enum NodeSize
{
	Small,
	Medium,
	Large,
};

UENUM()
enum NodeFunction
{
	Gathering,
	Transport,
	Market,
	None,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPANGEA_API UEconomyComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEconomyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	TEnumAsByte<NodeSize> nodeSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	TEnumAsByte<NodeFunction> nodeFunction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node Connections")
	TArray<AActor*> NeighboringNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* AreaBoundry;
};
