// Fill out your copyright notice in the Description page of Project Settings.


#include "RideAnimal.h"

// Sets default values for this component's properties
URideAnimal::URideAnimal()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UAnimSequence> StandingAnim(TEXT("/Game/_PangeaAssets/Meshes/stand_anim"));
	StandingAnimation = StandingAnim.Object;
	ConstructorHelpers::FObjectFinder<UAnimSequence> RidingAnim(TEXT("/Game/_PangeaAssets/Meshes/ride_anim"));
	RidingAnimation = RidingAnim.Object;
}


// Called when the game starts
void URideAnimal::BeginPlay()
{
	Super::BeginPlay();

	AnimalMotion = GetOwner()->FindComponentByClass<UAnimalMotion>();
	//RidingRegionTriggerBox = AnimalMotion->GetInteractionRegionTriggerBox();

	SaveInitialStates(); //Only called once

	if (AnimalFlying == Off)
	{
		AnimalMotion->SetIsGrounded(true);
	}
}


// Called every frame
void URideAnimal::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Riding
	UpdateIsRiding();

	//Flying
	if ((AnimalFlying == On) && AnimalMotion->GetIsRiding())
	{
		UpdateIsGrounded();
		FlyingSpecificMotion();
	}
}

void URideAnimal::SaveInitialStates()
{
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();
	PlayerPawnInitialPos = PlayerPawn->GetActorLocation();
}

//Riding general
void URideAnimal::UpdateIsRiding()
{
	if (AnimalMotion->GetIsTamed() && !AnimalMotion->GetIsRiding())
	{
		if (AnimalMotion->GetAnimalToPlayerVector().Size() < RidingRegionDistance)
		{
			if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("R"))
			{
				AnimalMotion->SetIsRiding(true);

				SetupMountState();

				if (AnimalFlying == On)
				{
					GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->GravityScale = 0.0f;
					GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->SetMovementMode(MOVE_Flying);
				}
			}
		}
	}
	else if (AnimalMotion->GetIsTamed() && AnimalMotion->GetIsRiding() && AnimalMotion->GetIsGrounded())
	{
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("R"))
		{
			AnimalMotion->SetIsRiding(false);

			SetupDismountState();

			//CAMERA SHOULDN'T CHANGE DIRECTION ON DISMOUNT

			if (AnimalFlying == On)
			{
				GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->GravityScale = 1.0f;
				GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->SetMovementMode(MOVE_Walking);
			}
		}
	}
}
void URideAnimal::SetupMountState()
{
	PlayerCharacter->SetActorEnableCollision(false);
	GetWorld()->GetFirstPlayerController()->UnPossess();
	//MAYBE AI CONTROLLER NEEDS TO UNPOSSESS CHARACTER FIRST TOO ONCE AI IS ADDED?
	GetWorld()->GetFirstPlayerController()->Possess(Cast<APawn>(GetOwner()));

	//Mounting Player Animation
	PlayerCharacter->GetMesh()->PlayAnimation(Cast<UAnimationAsset>(RidingAnimation), true);

	//Attaching Player from Socket
	FName SocketName = "Saddle Socket";
	PlayerCharacter->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

	//Adjust position of rider
	FVector MountedPlayerPos = FVector(
		GetOwner()->GetActorLocation().X,
		GetOwner()->GetActorLocation().Y,
		GetOwner()->GetActorLocation().Z + RidingHeight);
	MountedPlayerPos -= GetOwner()->GetActorForwardVector() * 20.0f;
	PlayerPawn->SetActorLocation(MountedPlayerPos);
}
void URideAnimal::SetupDismountState()
{
	PlayerCharacter->SetActorEnableCollision(true);
	GetWorld()->GetFirstPlayerController()->UnPossess();
	GetWorld()->GetFirstPlayerController()->Possess(PlayerPawn);

	//Dismounting Player Animation
	PlayerCharacter->GetMesh()->PlayAnimation(Cast<UAnimationAsset>(StandingAnimation), true);

	//Detaching Player from Socket
	PlayerCharacter->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);

	SetDismountedPositions();
}
void URideAnimal::SetDismountedPositions()
{
	//Set Dismounting Animal Position (included for when the player dismounts mid-jump).
	FVector UnmountedAnimalPos = FVector(
		GetOwner()->GetActorLocation().X,
		GetOwner()->GetActorLocation().Y,
		PlayerPawnInitialPos.Z);
	GetOwner()->SetActorLocation(UnmountedAnimalPos);

	//Set Dismounting Player Position
	FVector DismountedPlayerPos = FVector(
		GetOwner()->GetActorLocation().X,
		GetOwner()->GetActorLocation().Y,
		GetOwner()->GetActorLocation().Z);
	DismountedPlayerPos += GetOwner()->GetActorRightVector() * 100.0f;
	PlayerPawn->SetActorLocation(DismountedPlayerPos);
}

//Flying specific
void URideAnimal::UpdateIsGrounded()
{
	if (GetOwner()->GetActorLocation().Z < MaximumFlyingDismountHeight)
	{
		AnimalMotion->SetIsGrounded(true);
	}
	else
	{
		AnimalMotion->SetIsGrounded(false);
	}
}
void URideAnimal::FlyingSpecificMotion()
{
	UStaticMeshComponent* AnimalStaticMeshComponent = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown("up") ||
		GetWorld()->GetFirstPlayerController()->IsInputKeyDown("down"))
	{
		//GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + (AnimalFlyingSpeed * GetOwner()->GetActorForwardVector()));
	}
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown("F"))
	{
		//AnimalStaticMeshComponent->AddForce(FVector(0.0f, 0.0f, 1.0f) * AnimalStaticMeshComponent->GetMass());
		//GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + FVector(0.0f, 0.0f, 5.0f));
	}
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown("V"))
	{
		//AnimalStaticMeshComponent->AddForce(FVector(0.0f, 0.0f, -1.0f) * AnimalStaticMeshComponent->GetMass());
		//GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() - FVector(0.0f, 0.0f, 5.0f));
	}
}