// Fill out your copyright notice in the Description page of Project Settings.


#include "AIHumanoid.h"

// Sets default values
AAIHumanoid::AAIHumanoid()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAIHumanoid::BeginPlay()
{

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


