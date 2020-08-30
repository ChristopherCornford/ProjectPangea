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
	RidingPrecedence = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<URidingPrecedence>();
	PlayerRidingTracker = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UPlayerRidingTracker>();

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

	if (WasRideRequestSent)
	{
		//UE_LOG(LogTemp, Log, TEXT("RA. WasRideRequestSent = true"));
		if (CheckRideRequestProgress())
		{
			UE_LOG(LogTemp, Log, TEXT("RA. CheckRideRequestProgress = true"));
			if (RidingPrecedence->GetRideAnimal().X < GetOwner()->GetActorLocation().X + 0.05 &&
				RidingPrecedence->GetRideAnimal().X > GetOwner()->GetActorLocation().X - 0.05 &&
				RidingPrecedence->GetRideAnimal().Y < GetOwner()->GetActorLocation().Y + 0.05 &&
				RidingPrecedence->GetRideAnimal().Y > GetOwner()->GetActorLocation().Y - 0.05)
			{
				UE_LOG(LogTemp, Log, TEXT("RA. This is the animal that will be ridden!"));
				OnReceivedRequest();
				RidingPrecedence->ResetAll();
				UE_LOG(LogTemp, Log, TEXT("*********************************************************"))
				WasRideRequestSent = false;
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("RA. This animal is not the closest, so riding has been cancelled!"));
				WasRideRequestSent = false;
			}
		}
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
	if (AnimalMotion->GetIsTamed() && !AnimalMotion->GetIsRiding() && !PlayerRidingTracker->GetIsPlayerRiding())
	{
		if (!PlayerRidingTracker->GetDidJustDismount())
		{
			if (AnimalMotion->GetAnimalToPlayerVector().Size() < RidingRegionDistance)
			{
				if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("R"))
				{
					RidingPrecedence->AddRideRequest(GetOwner()->GetActorLocation(), AnimalMotion->GetAnimalToPlayerVector().Size());
					WasRideRequestSent = true;
				}
			}
		}
	}
	else if (AnimalMotion->GetIsTamed() && AnimalMotion->GetIsRiding() && AnimalMotion->GetIsGrounded())
	{
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("R"))
		{
			AnimalMotion->SetIsRiding(false);
			PlayerRidingTracker->SetIsPlayerRiding(false);
			PlayerRidingTracker->SetDidJustDismount(true);

			SetupDismountState();

			if (AnimalFlying == On)
			{
				GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->SetMovementMode(MOVE_Walking);
			}
		}
	}
}
void URideAnimal::OnReceivedRequest()
{
	AnimalMotion->SetIsRiding(true);
	PlayerRidingTracker->SetIsPlayerRiding(true);

	SetupMountState();

	if (AnimalFlying == On)
	{
		GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->SetMovementMode(MOVE_Flying);
	}
}
void URideAnimal::SetupMountState()
{
	//Save initial camera angle/location
	float DismountedCameraYaw = GetWorld()->GetFirstPlayerController()->GetPawn()->
		FindComponentByClass<UCameraComponent>()->GetComponentRotation().Yaw;

	//Unpossess and Possess
	PlayerCharacter->SetActorEnableCollision(false);
	GetWorld()->GetFirstPlayerController()->UnPossess();
	//MAYBE AI CONTROLLER NEEDS TO UNPOSSESS CHARACTER FIRST TOO ONCE AI IS ADDED?
	GetWorld()->GetFirstPlayerController()->Possess(Cast<APawn>(GetOwner()));

	//Setup new camera angle/location
	FRotator NewCameraRotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();
	NewCameraRotation.Yaw = DismountedCameraYaw;
	GetWorld()->GetFirstPlayerController()->SetControlRotation(NewCameraRotation);

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
	//Save initial camera angle/location
	float RidingCameraYaw = GetWorld()->GetFirstPlayerController()->GetPawn()->
		FindComponentByClass<UCameraComponent>()->GetComponentRotation().Yaw;

	//Unpossess and Possess
	PlayerCharacter->SetActorEnableCollision(true);
	GetWorld()->GetFirstPlayerController()->UnPossess();
	GetWorld()->GetFirstPlayerController()->Possess(PlayerPawn);

	//Setup new camera angle/location
	FRotator NewCameraRotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();
	NewCameraRotation.Yaw = RidingCameraYaw;
	GetWorld()->GetFirstPlayerController()->SetControlRotation(NewCameraRotation);

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
	if (!GetWorld()->GetFirstPlayerController()->IsInputKeyDown("up")
		&& !GetWorld()->GetFirstPlayerController()->IsInputKeyDown("down"))
	{
		GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->Velocity.X = 0.0f;
		GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->Velocity.Y = 0.0f;
	}
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown("F"))
	{
		GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->Velocity.Z = 150.0f;
	}
	else if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown("V"))
	{
		GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->Velocity.Z = -150.0f;
	}
	else
	{
		GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->Velocity.Z = 0.0f;
	}
}

//Other
bool URideAnimal::ExtGetIsRiding()
{
	return AnimalMotion->GetIsRiding();
}

bool URideAnimal::CheckRideRequestProgress()
{
	return RidingPrecedence->GetIsRequestCompleted();
}