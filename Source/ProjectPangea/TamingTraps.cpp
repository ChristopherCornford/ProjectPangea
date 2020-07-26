// Fill out your copyright notice in the Description page of Project Settings.


#include "TamingTraps.h"

// Sets default values for this component's properties
UTamingTraps::UTamingTraps()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTamingTraps::BeginPlay()
{
	Super::BeginPlay();	
}


// Called every frame
void UTamingTraps::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//MAYBE STOP PLAYER FROM PLACING TWO TRAPS IN THE SAME PLACE?

	//ADD INVENTORY CHECKS FOR EACH!

	//CHECK WITHIN REACH?

	//SHOULDN'T BE ABLE TO SPAWN, FOR EXAMPLE A ROPE LASSO AND A ROPE NET THROWN AT THE SAME TIME

	if (!IsAnotherTrapInProgress && !IsRopeLassoInProgress && !IsRopeLassoFloorAttachmentInProgress)
	{
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("One"))
		{
			UseRopeLasso();
			IsRopeLassoInProgress = true;
		}
		else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("Two"))
		{
			UseRopeNetThrow();
			IsAnotherTrapInProgress = true;
		}
		else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("Three"))
		{
			UseRopeNetTrap();
		}
		else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("Four"))
		{
			UseRopeLegTrap();
		}
		else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("Five"))
		{
			UseWoodenCage();
		}
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustReleased("One"))
	{
		if (IsRopeLassoInProgress && !IsAnotherTrapInProgress)
		{
			LaunchThrownTrap(CurrentlyPreparedThrownTrap);
			IsRopeLassoInProgress = false;
			IsRopeLassoFloorAttachmentInProgress = true;
		}
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustReleased("Two"))
	{
		if (IsAnotherTrapInProgress && !IsRopeLassoInProgress)
		{
			LaunchThrownTrap(CurrentlyPreparedThrownTrap);
			IsAnotherTrapInProgress = false;
		}
	}

	if (IsRopeLassoFloorAttachmentInProgress)
	{
		if (!IsValid(CurrentlyPreparedThrownTrap))
		{
			IsRopeLassoFloorAttachmentInProgress = false;
		}
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("Q"))
	{
		if (IsRopeLassoFloorAttachmentInProgress)
		{
			AttachRopeLassoToGround();
			IsRopeLassoFloorAttachmentInProgress = false;
		}
	}
}

//MAYBE CHECK IF CHOSEN LOCATION IS FREE FIRST, OR ELSE, NOT ALLOWED TO SPAWN?
void UTamingTraps::UseRopeLasso()
{
	UE_LOG(LogTemp, Log, TEXT("Using Rope Lasso"));
	TypeOfTrapToSpawn = RopeLasso;
	SpawnThrownTrap(TypeOfTrapToSpawn, CableTrapActorToSpawn);
}
void UTamingTraps::AttachRopeLassoToGround()
{
	UE_LOG(LogTemp, Log, TEXT("Attaching Rope Lasso to the Ground"));
	UCableComponent* Cable = CurrentlyPreparedThrownTrap->FindComponentByClass<UCableComponent>();
	Cable->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	FVector SpawnLocation = GetOwner()->GetActorLocation();
	SpawnLocation.Z = 35.0f;
	FRotator SpawnRotation = GetOwner()->GetActorRotation();
	AActor* CableFloorAttachment = GetWorld()->SpawnActor(CableFloorAttachmentToSpawn, &SpawnLocation, &SpawnRotation);
	Cable->AttachToComponent(CableFloorAttachment->FindComponentByClass<UStaticMeshComponent>(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}
void UTamingTraps::UseRopeNetThrow()
{
	UE_LOG(LogTemp, Log, TEXT("Throwing Rope Net"));
	TypeOfTrapToSpawn = RopeNetThrow;
	SpawnThrownTrap(TypeOfTrapToSpawn, BasicThrownTrapActorToSpawn);
}
void UTamingTraps::UseRopeNetTrap()
{
	UE_LOG(LogTemp, Log, TEXT("Using Rope Net Trap"));
	TypeOfTrapToSpawn = RopeNetTrap;
	SpawnTrap(TypeOfTrapToSpawn, BasicTrapActorToSpawn);
}
void UTamingTraps::UseRopeLegTrap()
{
	UE_LOG(LogTemp, Log, TEXT("Using Rope Leg Trap"));
	TypeOfTrapToSpawn = RopeLegTrap;
	SpawnTrap(TypeOfTrapToSpawn, BasicTrapActorToSpawn);
}
void UTamingTraps::UseWoodenCage()
{
	UE_LOG(LogTemp, Log, TEXT("Using Wooden Cage"));
	TypeOfTrapToSpawn = WoodenCage;
	SpawnTrap(TypeOfTrapToSpawn, BasicTrapActorToSpawn);
}

void UTamingTraps::SpawnTrap(TrapType TrapTypeToSpawn, TSubclassOf<AActor> TrapToSpawn)
{
	FVector SpawnLocation = GetOwner()->GetActorLocation() + (150.0f * GetOwner()->GetActorForwardVector());
	SpawnLocation.Z = 35.0f;
	FRotator SpawnRotation = GetOwner()->GetActorRotation();
	AActor* SpawnedTrap = GetWorld()->SpawnActor(TrapToSpawn, &SpawnLocation, &SpawnRotation);
	SpawnedTrap->FindComponentByClass<UTrapBehaviour>()->SetTrapType(TrapTypeToSpawn);
	SpawnedTrap->FindComponentByClass<UBoxComponent>()->SetGenerateOverlapEvents(true);
}
void UTamingTraps::SpawnThrownTrap(TrapType TrapTypeToSpawn, TSubclassOf<AActor> TrapToSpawn)
{
	FVector SpawnLocation = GetOwner()->GetActorLocation() + (70.0f * GetOwner()->GetActorForwardVector());
	FRotator SpawnRotation = GetOwner()->GetActorRotation();
	AActor* SpawnedTrap = GetWorld()->SpawnActor(TrapToSpawn, &SpawnLocation, &SpawnRotation);
	SpawnedTrap->FindComponentByClass<UTrapBehaviour>()->SetTrapType(TrapTypeToSpawn);
	SpawnedTrap->FindComponentByClass<UTrapBehaviour>()->SetIsReadied(true);
	CurrentlyPreparedThrownTrap = SpawnedTrap;
}
void UTamingTraps::LaunchThrownTrap(AActor* SpawnedTrap)
{
	SpawnedTrap->FindComponentByClass<UTrapBehaviour>()->SetIsReadied(false);
	SpawnedTrap->FindComponentByClass<UMeshComponent>()->SetSimulatePhysics(true);
	SpawnedTrap->FindComponentByClass<UBoxComponent>()->SetGenerateOverlapEvents(true);
	float ForceStrength = 15000.0f;
	FVector ForceVector = GetOwner()->GetActorForwardVector();
	ForceVector.Z = 1.0f;
	ForceVector = ForceVector * ForceStrength * SpawnedTrap->FindComponentByClass<UStaticMeshComponent>()->GetMass();
	SpawnedTrap->FindComponentByClass<UStaticMeshComponent>()->AddForce(ForceVector);
}