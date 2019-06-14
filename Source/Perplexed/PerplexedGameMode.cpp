// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "PerplexedGameMode.h"
#include "PerplexedCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "BPImplementablesLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "IncomingObject.h"

APerplexedGameMode::APerplexedGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void APerplexedGameMode::HandlePlayerDeath(APerplexedCharacter* Player)
{
	if (!IsValid(Player))
		return;
	Player->SetActorLocation({ Player->GetActorLocation().X, 2500.f, 2000.f });
}

void APerplexedGameMode::StartGame()
{
	AIncomingObject::StartSpawningIncomingObjects(GetWorld());
	StartPlayerCharacter();
}

void APerplexedGameMode::StartPlayerCharacter()
{
	APerplexedCharacter* Player = Cast<APerplexedCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!IsValid(Player))
		return;
	Player->EnableMovement();
}