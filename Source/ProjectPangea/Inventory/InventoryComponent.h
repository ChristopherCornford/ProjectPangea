// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseItem.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FSlotInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	FItemInfo ItemStructure;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 Quantity;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPANGEA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/*This name will appear as the inventory name*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FText InventoryName;

	/*Number of slots the inventory is going to have*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 NumberOfSlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FSlotInfo> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FVector2D ScreenOffsetPosition;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
