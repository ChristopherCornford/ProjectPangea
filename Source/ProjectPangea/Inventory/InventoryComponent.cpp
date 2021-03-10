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

	//PrepareInventory();
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
		FPartialStackCheck PartialStackCheck = HasPartialStack(NewContent);
		if(PartialStackCheck.bHasPartialCheck)
		{
			AddToStack(NewContent, PartialStackCheck.StackFoundIndex);
		}
		else
		{
			CreateStack(NewContent);
		}
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

FPartialStackCheck UInventoryComponent::HasPartialStack(FSlotInfo NewContent)
{
	FPartialStackCheck returnValue;

	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		//If they have the same item type and they aren't full
		if (Inventory[i].ItemStructure.ItemToStore->GetClass() == NewContent.ItemStructure.ItemToStore->GetClass()
			&& Inventory[i].Quantity < NewContent.ItemStructure.MaxStackSize)
		{
			//Item can be stacked

			returnValue.bHasPartialCheck = true;
			returnValue.StackFoundIndex = i;
			break; 
		}
	}
	return returnValue;
}

void UInventoryComponent::CreateStack(const FSlotInfo NewContent)
{
	Inventory.Add(NewContent);
}

bool UInventoryComponent::AddToStack(FSlotInfo NewContent, int32 SlotIndex)
{
	int32 iSlotIndex = SlotIndex;

	int32 CurrentQuantity = Inventory[SlotIndex].Quantity;
	
	CurrentQuantity += NewContent.Quantity;

	if (CurrentQuantity > Inventory[SlotIndex].ItemStructure.MaxStackSize)
	{
		//Create a new stack to add. Quantity is bigger than the max stack ammount.
		//We fill the existing item with the max quantity available
		Inventory[SlotIndex].Quantity = NewContent.ItemStructure.MaxStackSize;

		int32 RemainingQuantity = CurrentQuantity - NewContent.ItemStructure.MaxStackSize;

		//We add another item with the remain quantity
		FSlotInfo NewItemToAdd;
		NewItemToAdd.ItemStructure = NewContent.ItemStructure;
		NewItemToAdd.Quantity = RemainingQuantity;

		return AddToInventory(NewItemToAdd);
	}
	else
	{
		//Add the quantity to the existing item
		Inventory[SlotIndex].Quantity = CurrentQuantity;
		return true;
	}

	return false;
}

