// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "PerplexedLevelScriptActor.generated.h"

class AFloorPiece;
/**
 * 
 */
UCLASS()
class PERPLEXED_API APerplexedLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

	

public:
	// Sets default values for this actor's properties
	APerplexedLevelScriptActor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BlueprintClasses")
	TSubclassOf<class AFloorPiece> FloorPieceBPClass;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnAllFloorPieces();
};
