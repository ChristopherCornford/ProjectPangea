// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectPangeaController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPANGEA_API AProjectPangeaController : public APlayerController
{
	GENERATED_BODY()

public:
	AProjectPangeaController();

protected: 

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	class AProjectPangeaCharacter* PlayerCharacter;

protected: 
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* aPawn) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void CreateMainWidget();

public: 

	/*Create/Show the inventory*/
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void ToggleInventory();

	/*Removes the inventory UI*/
	//UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	//void CloseInventory();
};
