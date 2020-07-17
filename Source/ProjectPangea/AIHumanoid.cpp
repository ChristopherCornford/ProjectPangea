// Fill out your copyright notice in the Description page of Project Settings.


#include "AIHumanoid.h"
#include "Engine.h"

// Sets default values
AAIHumanoid::AAIHumanoid()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	int randInt = FMath::RandRange(0, 1);

	this->jobRole = (JobRole)randInt;

}

// Called when the game starts or when spawned
void AAIHumanoid::BeginPlay()
{
	Super::BeginPlay();
	
	InitJobs();
}

// Called every frame
void AAIHumanoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAIHumanoid::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIHumanoid::PerformJob()
{
	(this->* (JobArray[(int)jobRole]))();
}

void AAIHumanoid::InitJobs()
{
	JobArray = { &AAIHumanoid::Gather, &AAIHumanoid::Trade };
}

void AAIHumanoid::Gather()
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, this->NPC_Name + TEXT(" is GATHERING!"));
}

void AAIHumanoid::Trade()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, this->NPC_Name + TEXT(" is TRADING!"));
}

