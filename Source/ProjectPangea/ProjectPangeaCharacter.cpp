// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjectPangeaCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
//For movement
#include "Components/SkeletalMeshComponent.h"
#include "BipedIKAnim.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

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
  dead = false;


  clothing.SetNumZeroed(num_cloth_elements_);
  //initialization
  sk_cape_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("sk_cape_"));
  sk_cape_->SetupAttachment(GetMesh());
  sk_chest_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("sk_chest_"));
  sk_chest_->SetupAttachment(GetMesh());
  sk_bracers_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("sk_bracers_"));
  sk_bracers_->SetupAttachment(GetMesh());
  sk_gloves_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("sk_gloves_"));
  sk_gloves_->SetupAttachment(GetMesh());
  sk_pants_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("sk_pants_"));
  sk_pants_->SetupAttachment(GetMesh());
  sk_shoes_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("sk_shoes_"));
  sk_shoes_->SetupAttachment(GetMesh());
  sk_eyebrows_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("sk_eyebrows_"));
  sk_eyebrows_->SetupAttachment(GetMesh());
  sk_beard_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("sk_beard_"));
  sk_beard_->SetupAttachment(GetMesh());

  //Movement vars
  walking_speed_ = 160.0f;
  crouching_speed_ = 160.0f;
  running_speed_ = 320.0f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjectPangeaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
  // Set up gameplay key bindings
  check(PlayerInputComponent);
  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
  PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

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

  //For movement
  PlayerInputComponent->BindAction("Focus", IE_Pressed, this, &AProjectPangeaCharacter::Focus);
  PlayerInputComponent->BindAction("Focus", IE_Released, this, &AProjectPangeaCharacter::UnFocus);

  PlayerInputComponent->BindAction("Die", IE_Pressed, this, &AProjectPangeaCharacter::Die);
  PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AProjectPangeaCharacter::Attack);
  PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AProjectPangeaCharacter::SwitchCrouched);
  
  PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AProjectPangeaCharacter::Run);
  PlayerInputComponent->BindAction("Run", IE_Released, this, &AProjectPangeaCharacter::StopRun);

}

void AProjectPangeaCharacter::BeginPlay(){
  Super::BeginPlay();

  //Add extra meshes
  clothing[0] = sk_cape_;
  clothing[1] = sk_chest_;
  clothing[2] = sk_bracers_;
  clothing[3] = sk_gloves_;
  clothing[4] = sk_pants_;
  clothing[5] = sk_shoes_;
  clothing[6] = sk_eyebrows_;
  clothing[7] = sk_beard_;
  for (int i = 0; i < num_cloth_elements_; ++i) {
    if(clothing[i] != nullptr)
      clothing[i]->SetMasterPoseComponent(GetMesh());
  }

  anim_instance_ = Cast<UBipedIKAnim>(GetMesh()->GetAnimInstance());
  attacking = false;
  running = false;
  GetCharacterMovement()->MaxWalkSpeed = walking_speed_;
  GetCharacterMovement()->MaxWalkSpeedCrouched = crouching_speed_;
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
  //if (focused_) {
  //  FRotator temp = FollowCamera->GetComponentRotation();
  //  SetActorRotation(FRotator(GetActorRotation().Pitch, temp.Yaw, GetActorRotation().Roll));
  //}
}

void AProjectPangeaCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AProjectPangeaCharacter::MoveForward(float Value)
{
  if (attacking) return;

  if ((Controller != NULL) && (Value != 0.0f))
  {
    // find out which way is forward
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    // get forward vector
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(Direction, Value);
  }

  FVector local_velocity = UKismetMathLibrary::InverseTransformDirection(GetTransform(), GetVelocity());
  forward_speed_ = local_velocity.X;
  side_speed_ = local_velocity.Y;
}

void AProjectPangeaCharacter::MoveRight(float Value)
{
  if (attacking) return;

  if ((Controller != NULL) && (Value != 0.0f))
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

void AProjectPangeaCharacter::Focus() {
  if (running) StopRun();
  GetCharacterMovement()->bOrientRotationToMovement = false;
  GetCharacterMovement()->bUseControllerDesiredRotation = true;
  focused_ = true;
}

void AProjectPangeaCharacter::UnFocus() {
  GetCharacterMovement()->bOrientRotationToMovement = true;
  GetCharacterMovement()->bUseControllerDesiredRotation = false;
  focused_ = false;
}

void AProjectPangeaCharacter::Die() {
  if (!dead) {

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetAllBodiesSimulatePhysics(true);
    GetMesh()->SetAllBodiesPhysicsBlendWeight(1.0f);
    GetMesh()->WakeAllRigidBodies();
  }
  else {
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetAllBodiesSimulatePhysics(false);
  }
}

void AProjectPangeaCharacter::Attack() {
  float time_remain = anim_instance_->Attack(0);
  if (time_remain != 0.0f) {
    attacking = true;
    GetWorld()->GetTimerManager().SetTimer(attack_timer_, this, &AProjectPangeaCharacter::EndAttack, time_remain, false);
  }
}


void AProjectPangeaCharacter::EndAttack() {
  attacking = false;
}

void AProjectPangeaCharacter::SwitchCrouched() {
  if (!GetCharacterMovement()->IsCrouching())
    GetCharacterMovement()->bWantsToCrouch = true;
  else GetCharacterMovement()->bWantsToCrouch = false;
  GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, "yee");

}

void AProjectPangeaCharacter::Run() {
  if (focused_) return;
  running = true;
  GetCharacterMovement()->MaxWalkSpeed = running_speed_;
}

void AProjectPangeaCharacter::StopRun() {
  if (focused_) return;
  running = false;
  GetCharacterMovement()->MaxWalkSpeed = walking_speed_;
}