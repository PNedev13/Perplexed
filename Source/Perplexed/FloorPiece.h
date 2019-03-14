// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Engine Includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//Custom Includes
#include "PerplexedEnums.h"

#include "FloorPiece.generated.h"

UCLASS()
class PERPLEXED_API AFloorPiece : public AActor
{
	GENERATED_BODY()

	EFloorPieceTypeEnum FloorType;

	static TArray<AFloorPiece*> SpawnedFloorPiecesArray;
public:	
	// Sets default values for this actor's properties
	AFloorPiece();

	static int32 LastFloorXPosition;

	static const int8 MaxNumberOfPieces = 30;

	static const int32 FloorSize = 6000;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
