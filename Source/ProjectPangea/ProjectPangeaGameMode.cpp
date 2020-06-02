// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjectPangeaGameMode.h"
#include "ProjectPangeaCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectPangeaGameMode::AProjectPangeaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/_PangeaAssets/Blueprints/PlayerCharacter_Blueprint"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
