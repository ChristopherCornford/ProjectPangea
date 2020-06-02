// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "AnimalMotion.h"
#include "BreedAnimal.generated.h"


UENUM()
enum Gender
{
	Male UMETA(DisplayName = "Male"),
	Female UMETA(DisplayName = "Female")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTPANGEA_API UBreedAnimal : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBreedAnimal();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = Gender)
		TEnumAsByte<Gender> AnimalGender = Male;

	bool IsPregnant = false;
	
	//FOR SOME REASON THE TRIGGER BOX AND THE CHARACTER MESH SEEM TO COUNT AS TWO SEPARATE ACTORS
	//FIX THIS ISSUE OR MAKE A WORKAROUND
	bool IsValidMateWithinRange = false;
	//int ValidMatesWithinRangeNo = 0;

	FTimerHandle PregnancyTimerHandle;

	UPROPERTY(EditAnywhere)
		float PregnancyDuration = 5.0f;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> BabyToSpawn;

	ACharacter* SpawnedBaby;

	UAnimalMotion* AnimalMotion;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	Gender GetAnimalGender();
	void SetAnimalGender(Gender Gender);

	void UpdateIsPregnant();

	UFUNCTION()
		void SpawnBaby();

	void CheckActorIsValidMate(AActor* OtherActor, bool ShouldAddMate);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedActor,
			AActor* OtherActor,	UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedActor,
			AActor* OtherActor, UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex);

	//Temporary output log
	void PrintIsPregnant();
};
