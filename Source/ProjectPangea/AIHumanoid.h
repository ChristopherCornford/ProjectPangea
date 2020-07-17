// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIHumanoid.generated.h"

UENUM()
enum JobRole
{
	Gatherer = 0,
	Trader,
	JOB_ROLE_MAX,
};

UCLASS()
class PROJECTPANGEA_API AAIHumanoid : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAIHumanoid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Information")
	FString NPC_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job Role")
	TEnumAsByte<JobRole> jobRole;

	UFUNCTION(BlueprintCallable, Category = "Class Actions")
	void PerformJob();


	typedef void(AAIHumanoid::*Job)(void);
	TArray<Job> JobArray;

	void InitJobs();

	virtual void Gather();
	virtual void Trade();

};
