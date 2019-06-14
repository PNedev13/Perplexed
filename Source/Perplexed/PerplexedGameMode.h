// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BPImplementablesLibrary.h"
#include "PerplexedGameMode.generated.h"

UCLASS(minimalapi)
class APerplexedGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APerplexedGameMode();

	UPROPERTY(BlueprintReadWrite)
	UBPImplementablesLibrary* BPImplementablesLibrary;

protected:

	// The blueprint class which is derived from the base floor piece class
	// Used to spawn the floor pieces
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BlueprintClasses")
	TSubclassOf<class AFloorPiece> FloorPieceBPClass;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BlueprintClasses")
	TArray< TSubclassOf<class AIncomingObject> > IncomingObjectClasses;

public:
	const TSubclassOf<class AFloorPiece> GetFloorPieceBPClass() const { return FloorPieceBPClass; };

	const TArray< TSubclassOf<class AIncomingObject> > GetIncomingObjectClasses() const { return IncomingObjectClasses; };

	/* 
	 * Called when the player falls below the killZ point
	 * inside the APerplexedCharacter::Tick method
	 *
	 * @param Player		The player character
	*/
	void HandlePlayerDeath(class APerplexedCharacter* Player);

	UFUNCTION(BlueprintCallable)
	void StartGame();

	void StartPlayerCharacter();
};



