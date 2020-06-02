// Fill out your copyright notice in the Description page of Project Settings.


#include "BreedAnimal.h"

// Sets default values for this component's properties
UBreedAnimal::UBreedAnimal()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UBreedAnimal::BeginPlay()
{
	Super::BeginPlay();

	AnimalMotion = GetOwner()->FindComponentByClass<UAnimalMotion>();

	GetOwner()->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &UBreedAnimal::OnOverlapBegin);
	GetOwner()->FindComponentByClass<UBoxComponent>()->OnComponentEndOverlap.AddDynamic(this, &UBreedAnimal::OnOverlapEnd);
}


// Called every frame
void UBreedAnimal::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Check for nearby male and start pregnancy timer
	UpdateIsPregnant();

	if (GetAnimalGender() == Female)
	{
		/*if (IsPregnant)
		{
			UE_LOG(LogTemp, Log, TEXT("Pregnant"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Not Pregnant"));
		}*/
		if (IsValidMateWithinRange)
		{
			UE_LOG(LogTemp, Log, TEXT("Mate Within Range"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("NO Mate Within Range"));
		}
	}
}

void UBreedAnimal::UpdateIsPregnant()
{
	if ((GetAnimalGender() == Female) && !IsPregnant && IsValidMateWithinRange)//(ValidMatesWithinRangeNo > 0))
	{
		IsPregnant = true;
		//GetWorld()->GetTimerManager().SetTimer(PregnancyTimerHandle, this, &UBreedAnimal::SpawnBaby, PregnancyDuration);
	}
}
void UBreedAnimal::SpawnBaby()
{
	//TODO If that location is not available, need an alternate location so that spawn isn't cancelled?
	//Maybe do via a check to see if the SpawnedBaby Character is NULL or not?
	FVector LocationTemp = GetOwner()->GetActorLocation() + (100.0f * GetOwner()->GetActorRightVector());
	FRotator RotationTemp = GetOwner()->GetActorRotation();
	SpawnedBaby = Cast<ACharacter>(GetWorld()->SpawnActor(BabyToSpawn, &LocationTemp, &RotationTemp));
	SpawnedBaby->SetActorRelativeScale3D(SpawnedBaby->GetActorRelativeScale3D() / 1.5f);
	SpawnedBaby->SetActorLocation(SpawnedBaby->GetActorLocation() - FVector(0.0f, 0.0f, 30.0f));
	int NUM_TEMP = FMath::FRandRange(0, 2);
	if (NUM_TEMP == 0)
	{
		SpawnedBaby->FindComponentByClass<UBreedAnimal>()->SetAnimalGender(Male);
		UE_LOG(LogTemp, Log, TEXT("Baby spawned - Male"));
	}
	else
	{
		SpawnedBaby->FindComponentByClass<UBreedAnimal>()->SetAnimalGender(Female);
		UE_LOG(LogTemp, Log, TEXT("Baby spawned - Female"));
	}
	//BABY'S STATS ARE AVERAGE OF BOTH PARENTS
	//DO THIS BY RANDOMLY GETTING THE FATHER IF THERE IS MORE THAN ONE VALID MATE PRESENT
	//LIKELY DO THIS BY REPLACING VALIDMATESWITHINRANGENO WITH AN ARRAY OF ALL MATES WITHIN RANGE,
	//AND USING THE COUNT OF THIS VECTOR IN THE PLACE OF VALIDMATESWITHINRANGENO
	IsPregnant = false;
	GetWorld()->GetTimerManager().ClearTimer(PregnancyTimerHandle);
}

void UBreedAnimal::CheckActorIsValidMate(AActor* OtherActor, bool ShouldAddMate)
{
	if (OtherActor->FindComponentByClass<UBreedAnimal>() != NULL)
	{
		//ADD A CHECK TO MAKE SURE IT IS THE SAME SPECIES?
		if (OtherActor->FindComponentByClass<UBreedAnimal>()->GetAnimalGender() == Male)
		{
			//if (ShouldAddMate)
			//{
			//	ValidMatesWithinRangeNo++;
			//}
			//else
			//{
			//	ValidMatesWithinRangeNo--;
			//}
			IsValidMateWithinRange = ShouldAddMate;
		}
	}
}

void UBreedAnimal::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckActorIsValidMate(OtherActor, true);
}

void UBreedAnimal::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex)
{
	//THIS WILL CAUSE PROBLEMS IF THERE ARE TWO VALID MATES WITHIN RANGE
	//IF ONE LEAVES THEN THE CODE WILL THINK THERE ARE NO VALID MATES WITHIN RANGE
	CheckActorIsValidMate(OtherActor, false);
}

Gender UBreedAnimal::GetAnimalGender()
{
	return AnimalGender;
}
void UBreedAnimal::SetAnimalGender(Gender Gender)
{
	AnimalGender = Gender;
}