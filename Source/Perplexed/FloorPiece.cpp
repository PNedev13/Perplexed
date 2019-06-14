// Fill out your copyright notice in the Description page of Project Settings.

#include "FloorPiece.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "PerplexedCharacter.h"
#include "PerplexedGameMode.h"
#include "BPImplementablesLibrary.h"

float AFloorPiece::LastFloorXPosition = 0;
TWeakObjectPtr<AFloorPiece> AFloorPiece::DataHolderInstance;
uint8 AFloorPiece::StartingFloorPieceArrayIndex = 0;
const uint8 AFloorPiece::NumberOfFloorPiecesInARow = 3;
const uint8 AFloorPiece::FloorRows = 30;
const uint8 AFloorPiece::MaxFloorPieces = AFloorPiece::FloorRows * AFloorPiece::NumberOfFloorPiecesInARow;
const float AFloorPiece::FloorPieceYPlacingMargin = 300.f;

// Sets default values
AFloorPiece::AFloorPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFloorPiece::BeginPlay()
{
	Super::BeginPlay();
	RandomizeFloorPieceGroup();
}

// Called every frame
void AFloorPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFloorPiece::SpawnAllFloorPieces(UWorld* World)
{
	if (!IsValid(World) || !IsValid((APerplexedGameMode*)World->GetAuthGameMode()))
		return;

	for (uint8 row_cnt = 0; row_cnt < FloorRows; ++row_cnt)
	{
		LastFloorXPosition = row_cnt * FloorSize;
		for (uint8 col_cnt = 0; col_cnt < NumberOfFloorPiecesInARow; ++col_cnt)
		{
			AFloorPiece* NewObject = World->SpawnActor<AFloorPiece>(
				((APerplexedGameMode*)World->GetAuthGameMode())->GetFloorPieceBPClass(),
				{ LastFloorXPosition, (FloorSize + FloorPieceYPlacingMargin) * col_cnt, 0.f },
				FRotator::ZeroRotator);

			if (!DataHolderInstance.IsValid())
				DataHolderInstance = NewObject;

			DataHolderInstance->SpawnedFloorPiecesArray.Add(NewObject);
		}
	}
	StartingFloorPieceArrayIndex = 0;
}

void AFloorPiece::PlaceFirstRowAtEnd()
{
	if (!DataHolderInstance.IsValid())
		return;

	// Set the new floor row X position
	LastFloorXPosition += FloorSize;

	for (uint8 cnt = 0; cnt < NumberOfFloorPiecesInARow; ++cnt)
	{
		if (DataHolderInstance->SpawnedFloorPiecesArray.IsValidIndex(StartingFloorPieceArrayIndex))
		{
			DataHolderInstance->SpawnedFloorPiecesArray[StartingFloorPieceArrayIndex]->RandomizeFloorPieceGroup();
			DataHolderInstance->SpawnedFloorPiecesArray[StartingFloorPieceArrayIndex]->SetActorLocation({ LastFloorXPosition, (FloorSize + FloorPieceYPlacingMargin) * cnt, 0.f });
		}
		StartingFloorPieceArrayIndex++;
	}

	if (!DataHolderInstance->SpawnedFloorPiecesArray.IsValidIndex(StartingFloorPieceArrayIndex))
		StartingFloorPieceArrayIndex = 0;
}
void AFloorPiece::ManageFloorPiecesPosition(UWorld* World)
{
	if (!IsValid(World))
		return;

	APerplexedCharacter* PlayerCharacter = Cast<APerplexedCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	if (IsValid(PlayerCharacter))
	{
		if (PlayerCharacter->GetActorLocation().X >
			(LastFloorXPosition - (FloorRows / 2 * FloorSize)))
		{
			PlaceFirstRowAtEnd();
		}
	}
}

inline void AFloorPiece::RandomizeFloorPieceGroup()
{
	FloorPieceGroup = GOGEnumFunctionLibrary::GetRandomEnum();

	if (UBPImplementablesLibrary::Instance != nullptr)
		UBPImplementablesLibrary::Instance->UpdateStaticMeshMaterial(this, FloorPieceGroup);

}