// Fill out your copyright notice in the Description page of Project Settings.

#include "FloorPiece.h"
#include "Engine/World.h"


// Sets default values
AFloorPiece::AFloorPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FloorType = static_cast<EFloorPieceTypeEnum>(FMath::RandRange((int)EFloorPieceTypeEnum::FPTE_MIN + 1, (int)EFloorPieceTypeEnum::FPTE_MAX - 1));
	
}

// Called when the game starts or when spawned
void AFloorPiece::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloorPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}