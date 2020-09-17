// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EconomicAgentComponent.generated.h"


UENUM()
enum AgentRole
{
	Gatherer,
	Merchant,
	Caravan,
};


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPANGEA_API UEconomicAgentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEconomicAgentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Economic Agent Information")
	TEnumAsByte<AgentRole> agentRole;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Job Information")
	AActor* JobGiver;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Job Information")
	AActor* JobTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job Information")
	bool isJobComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job Information")
	FString CurrentInventory;

public:

	void BeginJob();

	UFUNCTION(BlueprintImplementableEvent)
	void onBeginJob();

	UFUNCTION(BlueprintCallable)
	void CompleteJob();

	UFUNCTION(BlueprintImplementableEvent)
	void onCompleteJob();

	void Gather();

	void Trade();

	void Transport();
};
