// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapBehaviour.h"

// Sets default values for this component's properties
UTrapBehaviour::UTrapBehaviour()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTrapBehaviour::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &UTrapBehaviour::OnOverlapBegin);
}


// Called every frame
void UTrapBehaviour::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (IsTrapped)
	{
		if (TrappedActor->FindComponentByClass<UAnimalMotion>()->GetIsTamed())
		{
			IsTrapped = false;
			GetOwner()->Destroy();
		}
	//	if (Type == RopeNetThrow)
	//	{
			//TrappedActor->SetActorLocation(NewTrappedPos);//TrappedPos);
	//	}
		if (Type == RopeNetTrap)
		{
			FVector RopeNetFinalPos = GetOwner()->GetActorLocation();
			RopeNetFinalPos.Z += 75.0f;
			TrappedActor->SetActorLocation(RopeNetFinalPos);
			if (TrappedActor->FindComponentByClass<UAnimalMotion>()->GetExhaustion() <
				TrappedActor->FindComponentByClass<UAnimalMotion>()->GetMaxExhaustion())
			{
				TrappedActor->FindComponentByClass<UAnimalMotion>()->IncrementExhaustion(RopeNetTrapExhaustionIncr);
			}
			else
			{
				TrappedActor->FindComponentByClass<UAnimalMotion>()->SetIsAlerted(true);
				TrappedActor->FindComponentByClass<UAnimalMotion>()->SetIsExhausted(true);
			}
		}
		else if (Type == WoodenCage)
		{
			if (TrappedActor->FindComponentByClass<UAnimalMotion>()->GetExhaustion() <
				TrappedActor->FindComponentByClass<UAnimalMotion>()->GetMaxExhaustion())
			{
				TrappedActor->FindComponentByClass<UAnimalMotion>()->IncrementExhaustion(WoodenCageExhaustionIncr);
			}
			else
			{
				TrappedActor->FindComponentByClass<UAnimalMotion>()->SetIsAlerted(true);
				TrappedActor->FindComponentByClass<UAnimalMotion>()->SetIsExhausted(true);
			}
		}
	}
}

void UTrapBehaviour::SetTrapType(TrapType InType)
{
	Type = InType;
}

void UTrapBehaviour::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Animal")))
	{
		if (!OtherActor->FindComponentByClass<UAnimalMotion>()->GetIsTamed())
		{
			if (Type == RopeNetThrow)
			{
				//UE_LOG(LogTemp, Log, TEXT("ANIMAL TRAPPED! - Rope Net Thrown"));
				//IsTrapped = true;
				//TrappedActor = OtherActor;
				//TrappedPos = OtherActor->GetActorLocation();
			}
			else if (Type == RopeNetTrap)
			{
				UE_LOG(LogTemp, Log, TEXT("ANIMAL TRAPPED! - Rope Net Trap"));
				IsTrapped = true;
				OtherActor->FindComponentByClass<UAnimalMotion>()->SetIsTrapped(true);
				TrappedActor = OtherActor;

				FVector RopeNetFinalPos = GetOwner()->GetActorLocation();
				RopeNetFinalPos.Z = 200.0f;
				GetOwner()->SetActorLocation(RopeNetFinalPos);
				RopeNetFinalPos.Z += 75.0f;
				OtherActor->SetActorLocation(RopeNetFinalPos);
			}
			else if (Type == WoodenCage)
			{
				UE_LOG(LogTemp, Log, TEXT("ANIMAL TRAPPED! - Wooden Cage"));
				IsTrapped = true;
				OtherActor->FindComponentByClass<UAnimalMotion>()->SetIsTrapped(true);
				TrappedActor = OtherActor;

				FVector RopeNetFinalPos = GetOwner()->GetActorLocation();
				RopeNetFinalPos.Z += 75.0f;
				OtherActor->SetActorLocation(RopeNetFinalPos);
			}
		}
	}
}