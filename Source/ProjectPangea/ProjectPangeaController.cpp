// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectPangeaController.h"
#include "ProjectPangeaCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Inventory/InventoryComponent.h"
#include "BaseItem.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "ProjectPangeaCharacter.h"

AProjectPangeaController::AProjectPangeaController()
{

}

void AProjectPangeaController::BeginPlay()
{
	Super::BeginPlay();

	
	
	CreateMainWidget();
}

void AProjectPangeaController::OnPossess(APawn * aPawn)
{
	Super::OnPossess(aPawn);

	PlayerCharacter = Cast<AProjectPangeaCharacter>(GetCharacter());

	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character associated correctly with the player controller."));

		PlayerCharacter->PlayerController = this;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No associated characte found."));
	}
}

