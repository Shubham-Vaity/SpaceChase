// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceChaseGameMode.h"
#include "SpaceChaseCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASpaceChaseGameMode::ASpaceChaseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
