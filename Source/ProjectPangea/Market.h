// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Market.generated.h"

UCLASS()
class PROJECTPANGEA_API AMarket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMarket();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent Information")
		TArray<AActor*> AgentsInMarket;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trading Stall Information")
		TArray<AActor*> OpenStalls;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trading Stall Information")
		TArray<AActor*> ClosedStalls;
};
