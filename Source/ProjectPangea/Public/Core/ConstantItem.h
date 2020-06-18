// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ItemLogic.h"
#include "ConstantItem.generated.h"

UCLASS()
class PROJECTPANGEA_API AConstantItem : public AActor, public IItemLogic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConstantItem();

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

};
