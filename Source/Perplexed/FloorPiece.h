// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PerplexedEnums.h"
#include "FloorPiece.generated.h"

UCLASS()
class PERPLEXED_API AFloorPiece : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AFloorPiece*> SpawnedFloorPiecesArray;

	/* Pointer to the first created object of this class,
	 * holding class related (static) data
	 * Used to avoid having static attributes which can not be
	 * declared as UPROPERTY() 
	 */
	static TWeakObjectPtr<AFloorPiece> DataHolderInstance;

	void RandomizeFloorPieceGroup();
	
public:	
	// Sets default values for this actor's properties
	AFloorPiece();

	// The X world coordinates of the last placed/moved floor piece
	static float LastFloorXPosition;

	// The size of the floor static mesh
	static constexpr float FloorSize = 2000.f;

	// The array index of the floor piece closest to the start point
	static uint8 StartingFloorPieceArrayIndex;

	// The number of floor pieces in one row
	static const uint8 NumberOfFloorPiecesInARow;

	// The number of floor rows spawned in the world
	static const uint8 FloorRows;

	// The number of pieces spawned in the world
	static const uint8 MaxFloorPieces;

	// The number of pieces spawned in the world
	static const float FloorPieceYPlacingMargin;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	EGameObjectGroup FloorPieceGroup;

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "HandleFloorGroupChange"))
	void BPHandleFloorGroupChange();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Places the starting floor row next to the last placed one
	static void PlaceFirstRowAtEnd();

	// Called when the game starts to spawn all the floor pieces
	static void SpawnAllFloorPieces(UWorld* World);

	// Checks if the player has passed more than half of the floor pieces
	// and calls PlaceStartingPiecesAtEnd to rearrange them
	static void ManageFloorPiecesPosition(UWorld* World);
};
