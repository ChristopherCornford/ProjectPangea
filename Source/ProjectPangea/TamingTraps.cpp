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

	//IF ANIMAL IS WITHIN REACH
	//{
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("One"))
		{
			//ADD AN INVENTORY CHECK TO MAKE SURE THE PLAYER HAS ENOUGH OF THIS TRAP TYPE
			//{
				//UseRopeLasso();
			//}
		}
	//}
	
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("Two"))
	{
		//ADD AN INVENTORY CHECK TO MAKE SURE THE PLAYER HAS ENOUGH OF THIS TRAP TYPE
		//{
			//UseRopeNetThrow();
		//}
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("Three"))
	{
		//ADD AN INVENTORY CHECK TO MAKE SURE THE PLAYER HAS ENOUGH OF THIS TRAP TYPE
		//{
			UseRopeNetTrap();
		//}
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("Four"))
	{
		//ADD AN INVENTORY CHECK TO MAKE SURE THE PLAYER HAS ENOUGH OF THIS TRAP TYPE
		//{
			UseWoodenCage();
		//}
	}
}

//MAYBE CHECK IF CHOSEN LOCATION IS FREE FIRST, OR ELSE, NOT ALLOWED TO SPAWN?
void UTamingTraps::UseRopeLasso()
{
	//UE_LOG(LogTemp, Log, TEXT("Using Rope Lasso"));
	//TypeOfTrapToSpawn = RopeLasso;
	//SpawnTrap(TypeOfTrapToSpawn, CableTrapActorToSpawn);
}
void UTamingTraps::UseRopeNetThrow()
{
	//UE_LOG(LogTemp, Log, TEXT("Throwing Rope Net"));
	//TypeOfTrapToSpawn = RopeNetThrow;
	//SpawnTrapNew(TypeOfTrapToSpawn, BasicTrapActorToSpawn);
}
void UTamingTraps::UseRopeNetTrap()
{
	UE_LOG(LogTemp, Log, TEXT("Using Rope Net Trap"));
	TypeOfTrapToSpawn = RopeNetTrap;
	SpawnTrapNew(TypeOfTrapToSpawn, BasicTrapActorToSpawn);
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
	SpawnLocation.Z = 1000.0f;
	FRotator SpawnRotation = GetOwner()->GetActorRotation();
	FTransform SpawnTransform = GetOwner()->GetActorTransform();
	AActor* SpawnedTrap = GetWorld()->SpawnActor(TrapToSpawn, &SpawnLocation, &SpawnRotation);
	SpawnedTrap->FindComponentByClass<UTrapBehaviour>()->SetTrapType(TrapTypeToSpawn);
	SpawnLocation.Z = 35.0f;
	SpawnedTrap->SetActorLocation(SpawnLocation);
	if (TrapTypeToSpawn == RopeLasso)
	{
		if (GetOwner()->FindComponentByClass<UScanForNearbyAnimals>()->GetCurrentlyOverlappedAnimal() != NULL)
		{
			SpawnedTrap->FindComponentByClass<UCableComponent>()->SetAttachEndTo(
				GetOwner()->FindComponentByClass<UScanForNearbyAnimals>()->GetCurrentlyOverlappedAnimal(), TEXT("BreedingRegionTriggerBox"));//, TEXT("SaddleSocket"));
		}
		else
		{
			//THIS NEEDS REMOVING AND A CHECK TO SEE IF ANY ANIMALS ARE CURRENTLY OVERLAPPED NEEDS TO BE INTRODUCED - USING THE BOOL ISOVERLAPPED?
			SpawnedTrap->FindComponentByClass<UCableComponent>()->SetWorldLocation(
				GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 50.0f));
		}
	}
	//SpawnedTrap->FindComponentByClass<UTrapBehaviour>()->SetTrapType(TrapTypeToSpawn);
	//SpawnedTrapList.Add(TrapClass(TrapTypeToSpawn, SpawnedTrap, SpawnedTrapList.Num()));
}

void UTamingTraps::SpawnTrapNew(TrapType TrapTypeToSpawn, TSubclassOf<AActor> TrapToSpawn)
{
	FVector SpawnLocation = GetOwner()->GetActorLocation() + (150.0f * GetOwner()->GetActorForwardVector());
	SpawnLocation.Z = 35.0f;
	FRotator SpawnRotation = GetOwner()->GetActorRotation();
	//FTransform SpawnTransform = GetOwner()->GetActorTransform();
	AActor* SpawnedTrap = GetWorld()->SpawnActor(TrapToSpawn, &SpawnLocation, &SpawnRotation);
	SpawnedTrap->FindComponentByClass<UTrapBehaviour>()->SetTrapType(TrapTypeToSpawn);
	//SpawnedTrap->SetActorLocation(SpawnLocation);
}