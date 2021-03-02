// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	bool bStackable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	class UTexture2D* ItemThumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	bool bIsConsumible;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	int32 MaxStackSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	float Durability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	class AActor* ItemToStore;
};

UCLASS()
class PROJECTPANGEA_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	//Should be a pure virtual function. 
	virtual void Interact();

public:	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Information")
	FItemInfo ItemInformation;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*Get item information*/
	FORCEINLINE FItemInfo GetItemInformation() const { return ItemInformation; };

};
