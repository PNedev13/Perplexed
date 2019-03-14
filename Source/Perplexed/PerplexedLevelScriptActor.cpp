// Fill out your copyright notice in the Description page of Project Settings.

#include "PerplexedLevelScriptActor.h"
#include "FloorPiece.h"
#include "Engine/World.h"

int32 AFloorPiece::LastFloorXPosition;

APerplexedLevelScriptActor::APerplexedLevelScriptActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APerplexedLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	SpawnAllFloorPieces();

}

// Called every frame
void APerplexedLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APerplexedLevelScriptActor::SpawnAllFloorPieces()
{
	AFloorPiece::LastFloorXPosition = 0;
	FVector SpawnLocation(AFloorPiece::LastFloorXPosition, 0.f, 0.f);

	for (int32 cnt = 0; cnt < AFloorPiece::MaxNumberOfPieces; ++cnt)
	{
		GetWorld()->SpawnActor<AFloorPiece>(FloorPieceBPClass, SpawnLocation, FRotator::ZeroRotator);

		SpawnLocation.Y = AFloorPiece::FloorSize / 2;
		GetWorld()->SpawnActor<AFloorPiece>(FloorPieceBPClass, SpawnLocation, FRotator::ZeroRotator);

		AFloorPiece::LastFloorXPosition += AFloorPiece::FloorSize/2;
		SpawnLocation.X = AFloorPiece::LastFloorXPosition;
		SpawnLocation.Y = 0;
	}


}
