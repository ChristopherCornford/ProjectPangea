// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ItemLogic.h"
#include "MutableItem.generated.h"

//Abstrac struct to save general item data (item manages positions)
struct FItemData {
  int integer_values_[5];
  float decimal_values_[5];//Temporal
};

UCLASS()
class PROJECTPANGEA_API AMutableItem : public AActor, public IItemLogic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMutableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
    bool Use();
  //Override this function for item use logic
  virtual bool Use_Implementation();

  //Override this function to populate abstract struct with item data
  virtual FItemData getItemData();

  //Override this function to populate item with data from struct
  virtual void setItemData(FItemData data);

};
