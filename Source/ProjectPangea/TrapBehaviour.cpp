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


	if (IsReadied)
	{
		if (Type == RopeNetThrow)
		{
			FVector PreppedTrapLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
			PreppedTrapLocation += (70.0f * GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorForwardVector());
			GetOwner()->SetActorLocation(PreppedTrapLocation);
			GetOwner()->SetActorRotation(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation());
			GetOwner()->SetActorRotation(InitRotation);
		}
	}

	if (Type == RopeNetThrow || Type == RopeLasso)
	{
		if (GetOwner()->GetActorLocation().Z < 37.0f)
		{
			GetOwner()->Destroy();
		}
	}

	if (IsTrapped)
	{
		if (TrappedActor->FindComponentByClass<UAnimalMotion>()->GetIsTamed())
		{
			IsTrapped = false;
			GetOwner()->Destroy();
		}
		if (Type == RopeLasso)
		{
			TrappedExhaustion(RopeLassoExhaustionIncr);
		}
		else if (Type == RopeNetTrap)
		{
			FVector RopeNetFinalPos = GetOwner()->GetActorLocation();
			RopeNetFinalPos.Z += 75.0f;
			TrappedActor->SetActorLocation(RopeNetFinalPos);
			TrappedExhaustion(RopeNetTrapExhaustionIncr);
		}
		else if (Type == RopeNetThrow)
		{
			TrappedExhaustion(RopeNetThrowExhaustionIncr);
		}
		else if (Type == RopeLegTrap)
		{
			TrappedExhaustion(RopeLegTrapExhaustionIncr);
		}
		else if (Type == WoodenCage)
		{
			TrappedExhaustion(WoodenCageExhaustionIncr);
		}
	}
}

void UTrapBehaviour::TrappedExhaustion(float ExhaustionIncrValue)
{
	if (TrappedActor->FindComponentByClass<UAnimalMotion>()->GetExhaustion() <
		TrappedActor->FindComponentByClass<UAnimalMotion>()->GetMaxExhaustion())
	{
		TrappedActor->FindComponentByClass<UAnimalMotion>()->IncrementExhaustion(ExhaustionIncrValue);
	}
	else
	{
		TrappedActor->FindComponentByClass<UAnimalMotion>()->SetIsAlerted(true);
		TrappedActor->FindComponentByClass<UAnimalMotion>()->SetIsExhausted(true);
	}
}

void UTrapBehaviour::SetTrapType(TrapType InType)
{
	Type = InType;
}

void UTrapBehaviour::SetIsReadied(bool InputBool)
{
	IsReadied = InputBool;
}
void UTrapBehaviour::SetStartingPoint(FVector InputStartingPoint)
{
	StartingPoint = InputStartingPoint;
}
void UTrapBehaviour::SetInitRotation(FRotator InputInitRotation)
{
	InitRotation = InputInitRotation;
}

void UTrapBehaviour::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Animal")))
	{
		if (!OtherActor->FindComponentByClass<UAnimalMotion>()->GetIsTamed())
		{
			if (!OtherActor->FindComponentByClass<UAnimalMotion>()->GetIsTrapped())
			{
				if (Type == RopeLasso)
				{
					UE_LOG(LogTemp, Log, TEXT("ANIMAL TRAPPED! - Rope Lasso Thrown"));
					IsTrapped = true;
					OtherActor->FindComponentByClass<UAnimalMotion>()->SetIsTrapped(true);
					TrappedActor = OtherActor;

					GetOwner()->FindComponentByClass<UMeshComponent>()->SetSimulatePhysics(false);

					FVector RopeLassoFinalPos = GetOwner()->GetActorLocation();
					RopeLassoFinalPos.Z = 150.0f;
					GetOwner()->SetActorLocation(RopeLassoFinalPos);
					RopeLassoFinalPos.Z = 70.0f;
					OtherActor->SetActorLocation(RopeLassoFinalPos);

					UCableComponent* Cable = GetOwner()->FindComponentByClass<UCableComponent>();
					USceneComponent* CharacterSkeletalMeshComponent = Cast<USceneComponent>(GetWorld()->GetFirstPlayerController()->
						GetPawn()->FindComponentByClass<USkeletalMeshComponent>());
					Cable->AttachToComponent(CharacterSkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Hand_Socket"));
				}
				else if (Type == RopeNetThrow)
				{
					UE_LOG(LogTemp, Log, TEXT("ANIMAL TRAPPED! - Rope Net Thrown"));
					IsTrapped = true;
					OtherActor->FindComponentByClass<UAnimalMotion>()->SetIsTrapped(true);
					TrappedActor = OtherActor;

					GetOwner()->FindComponentByClass<UMeshComponent>()->SetSimulatePhysics(false);

					FVector RopeNetThrownFinalPos = GetOwner()->GetActorLocation();
					RopeNetThrownFinalPos.Z = 40.0f;
					GetOwner()->SetActorLocation(RopeNetThrownFinalPos);
					RopeNetThrownFinalPos.Z += 70.0f;
					OtherActor->SetActorLocation(RopeNetThrownFinalPos);
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
				else if (Type == RopeLegTrap || Type == WoodenCage)
				{
					if (Type == RopeLegTrap)
					{
						UE_LOG(LogTemp, Log, TEXT("ANIMAL TRAPPED! - Rope Leg Trap"));
					}
					else if (Type == WoodenCage)
					{
						UE_LOG(LogTemp, Log, TEXT("ANIMAL TRAPPED! - Wooden Cage"));
					}
					IsTrapped = true;
					OtherActor->FindComponentByClass<UAnimalMotion>()->SetIsTrapped(true);
					TrappedActor = OtherActor;

					FVector TrappedActorFinalPos = GetOwner()->GetActorLocation();
					TrappedActorFinalPos.Z += 75.0f;
					OtherActor->SetActorLocation(TrappedActorFinalPos);
				}
			}
		}
	}
}