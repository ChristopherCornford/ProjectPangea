// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AnimalBaseClass.generated.h"

UENUM()
enum class eGender : uint8 {
	Male,
	Female
};

UENUM()
enum class eDinoType : uint8 {
	LandHerbivore,
	LandCarnivore,
	WaterHerbivore,
	WaterCarnivore
};

UCLASS()
class PROJECTPANGEA_API AAnimalBaseClass : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAnimalBaseClass();
	//Basic attributes all dinos will derive from in game
	UPROPERTY(EditAnywhere, Category = "Species")
		FString fsSpeciesName;

	UPROPERTY(VisibleAnywhere, Category = "Gender")
		eGender Gender;

	UPROPERTY(VisibleAnywhere, Category = "Type")
		eDinoType DinoType;

	UPROPERTY(EditAnywhere, Category = "Stats")
		int nLevel;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float fDinoHealth;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float fDinoStamina;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float fDinoAttack;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float fDinoDefense;

	int nGenderSelector;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
