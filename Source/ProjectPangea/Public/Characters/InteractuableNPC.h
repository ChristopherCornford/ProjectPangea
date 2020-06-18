// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/BaseNPC.h"
#include "Interfaces/Interactuable.h"
#include "InteractuableNPC.generated.h"

/**
 * 
 */

class UBoxComponent;

UCLASS()
class PROJECTPANGEA_API AInteractuableNPC : public ABaseNPC, public IInteractuable
{
	GENERATED_BODY()
public:
  AInteractuableNPC();

  UPROPERTY(VisibleAnywhere, category = "Components")
    UBoxComponent *trigger_;

  virtual void BeginPlay() override;
  // Called every frame
  virtual void Tick(float DeltaTime) override;


  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
    bool Interact();
  bool Interact_Implementation();
protected:


};
