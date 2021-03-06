// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjectPangeaCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AProjectPangeaCharacter

AProjectPangeaCharacter::AProjectPangeaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjectPangeaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("EnterRoom", IE_Pressed, this, &AProjectPangeaCharacter::EnterRoom); 
    //PlayerInputComponent->BindAction("EnterRoom", IE_Released, this, &AProjectPangeaCharacter::ExitRoom);
    PlayerInputComponent->BindAction("ExitRoom", IE_Pressed, this, &AProjectPangeaCharacter::ExitRoom);

	PlayerInputComponent->BindAxis("MoveForward", this, &AProjectPangeaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProjectPangeaCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AProjectPangeaCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AProjectPangeaCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AProjectPangeaCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AProjectPangeaCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AProjectPangeaCharacter::OnResetVR);
}


void AProjectPangeaCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AProjectPangeaCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AProjectPangeaCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AProjectPangeaCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AProjectPangeaCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AProjectPangeaCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AProjectPangeaCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}



void AProjectPangeaCharacter::EnterRoom()
{
    // logging
    FString debugStr = FString(TEXT("Loading next room!"));
    if (LevelStreamingActor == NULL)
    {
        debugStr += FString(TEXT("\n\t")) + FString(TEXT("Note: NO STREAMING VOLUME SET!"));
    }
    if (streamingVolumeData == NULL)
    {
        debugStr += FString(TEXT("\n\t")) + FString(TEXT("Note: VOLUME DATA CLASS NOT SET!"));
    }
    UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, debugStr);

    // loading using volume data
    /*
    if (LevelStreamingActor != NULL && LevelStreamingActor->isInZone && LevelStreamingActor->LevelToLoad != "")
    {
    }
    */

    // loading using streaming data class
    if (streamingVolumeData != NULL)
    {
        // loading
        if (streamingVolumeData->levelToLoad != "")
        {
            FLatentActionInfo LatentInfo;
            UGameplayStatics::LoadStreamLevel(this, streamingVolumeData->levelToLoad, true, true, LatentInfo);

            // position change
            FVector pos_room2 = FVector(-160.0f, 90.0f, 267.8f);
            FRotator rot_room2 = FRotator(0.0f, 0.0f, 0.0f);
            //SetActorLocation(pos_room2);
            //SetActorRotation(rot_room2);
        }

        // unloading
        if (streamingVolumeData->levelToUnload != "")
        {
            // queue the unload. cannot unload immediately after load/another unload
            streamingVolumeData->queueUnload(streamingVolumeData->levelToUnload);
        }
    }
}



void AProjectPangeaCharacter::ExitRoom()
{

    /*
    FString debugStr = FString(TEXT("Closing room!"));
    if (LevelStreamingActor != NULL) {
        debugStr += FString(TEXT("\n\t")) + FString(TEXT("LevelStreamingActor->isInZone: ")) + (LevelStreamingActor->isInZone ? TEXT("true") : TEXT("false"));
    }
    else {
        debugStr += FString(TEXT("\n\t")) + FString(TEXT("Note: NO STREAMING VOLUME SET!"));
    }
    UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, debugStr);

    if (LevelStreamingActor != NULL && !LevelStreamingActor->isInZone && LevelStreamingActor->LevelToLoad != "")
    {
        // unloading
        FLatentActionInfo LatentInfo;
        //UGameplayStatics::UnloadStreamLevel(this, LevelStreamingActor->LevelToLoad, LatentInfo);
        UGameplayStatics::UnloadStreamLevel(this, LevelStreamingActor->LevelToLoad, LatentInfo, true);

        /////////// test
        debugStr = FString(TEXT("So here it works...."));
        debugStr += FString(TEXT("\n\t")) + FString(TEXT("LevelStreamingActor->LevelToLoad: ")) + LevelStreamingActor->LevelToLoad.ToString();
        UE_LOG(LogClass, Log, TEXT("%s"), *debugStr);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, debugStr);

        // position change
        FVector pos_room1 = FVector(-700.0f, 90.0f, 267.8f);
        FRotator rot_room1 = FRotator(0.0f, 180.0f, 0.0f);
        //SetActorLocation(pos_room1);
        //SetActorRotation(rot_room1);
    }
    */
}
