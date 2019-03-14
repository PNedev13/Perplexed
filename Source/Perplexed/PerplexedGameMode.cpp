// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "PerplexedGameMode.h"
#include "PerplexedCharacter.h"
#include "UObject/ConstructorHelpers.h"

APerplexedGameMode::APerplexedGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
