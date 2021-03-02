// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Name of the inventory
	InventoryName = FText::FromString("Inventory");

	//Number of Slots Inside the Backpack
	NumberOfSlots = 16;

}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	PrepareInventory();
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryComponent::AddToInventory(FSlotInfo NewContent)
{
	// TODO: At the moment it will always return true. 
	if (NewContent.ItemStructure.bStackable)
	{

	}
	else
	{
		CreateStack(NewContent);
	}

	return true;
}

void UInventoryComponent::PrepareInventory()
{
	Inventory.Reserve(NumberOfSlots);

	UE_LOG(LogTemp, Warning, TEXT("Inventory Elements: %i"), Inventory.Num());

}

void UInventoryComponent::CreateStack(const FSlotInfo NewContent)
{
	FSlotInfo EmptySlot;
	int32 ItemIndex = 0;
	
	Inventory.Add(NewContent);
	/*for (auto Item : Inventory)
	{
		if(Item.ItemStructure)
	}
	if (Inventory.Find(EmptySlot, ItemIndex))
	{
		Inventory.Insert(NewContent, ItemIndex);
	}*/

	

	
}

