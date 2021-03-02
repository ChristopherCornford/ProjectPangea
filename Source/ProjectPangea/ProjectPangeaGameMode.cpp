// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjectPangeaGameMode.h"
#include "ProjectPangeaCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectPangeaGameMode::AProjectPangeaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//PlayerControllerClass = AProjectPangeaController::StaticClass();
	
 /*	static ConstructorHelpers::FClassFinder<APlayerController> ControllerClassAsset(TEXT("/Game/Blueprints/BP_ProjectPangeaController"));
	if (ControllerClassAsset.Succeeded())
	{
		PlayerControllerClass = ControllerClassAsset.Class;
		UE_LOG(LogTemp, Warning, TEXT("PlayerController set correctly"));
	}*/
	
}
