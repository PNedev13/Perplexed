// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PerplexedEnums.h"
#include "IncomingObject.generated.h"

UCLASS()
class PERPLEXED_API AIncomingObject : public AActor
{
	GENERATED_BODY()

	// The interpolation speed
	static float InterpSpeed;

	// The size of the object pool
	static uint8 MaxNumberOfObjects;

	// Time between every incoming object spawn
	static float TimeBetweenObjectSpawn;

	// Object's StaticMesh width
	static const uint32 ObjectWidth;

	static const FVector LaunchVelocityOnHit;

	/* Pointer to the first created object of this class,
	 * holding class related (static) data
	 * Used to avoid having static attributes which can not be
	 * declared as UPROPERTY()
	 */
	static TWeakObjectPtr<AIncomingObject> DataHolderInstance;

	// All objects placed in front of the player are
	// stored here until needed again
	UPROPERTY()
	TArray<AIncomingObject*> ActiveIncomingObjects;

	// Stores all the objects who may be placed
	// again in front of the player
	UPROPERTY()
	TArray<AIncomingObject*> InactiveIncomingObjects;

	// Every currently available Y spawn position
	static TArray<int32> AvailableYSpawnPositions;

	// The X distance between the spawned object and the Player
	static const float SpawnDistanceFromPlayer;

	// The X distance between the player and the object's target moving point
	static const float InterpolationDistance;

	// The Z spawn position of the object
	static const float SpawnZPosition;

	// The X distance needed between the object and the player 
	// in order to mark the block as inactive
	static const float InactiveDistanceThreshold;

	// The value used as an interpolation target for moving the object
	float InterpolationTargetX;

	// Creates a pool with the maximum number of objects
	static void SpawnAllIncomingObjects(UWorld* World);

	// Checks which of the currently active objects are behind the player
	// and transfers them to the InactiveIncomingObjects array
	static void CheckForInactiveObjects(float PlayerXPosition);

	// Calculate the possible spawn locations on the Y axis
	static void CalculateYSpawnLocations();

	/* 
	 * Adds the object to the pool (inactive object's array)
	 *
	 * @param ActiveObjectsArrayIndex   The object's index in the ActiveIncomingObjects array
	 */
	void SetInactive(int32 ActiveObjectsArrayIndex);

	/** Called when an overlap occures between the player
	 * and the hit trigger volume attached to the object. (See BP)
	 * Handles what happens to the player thereafter.
	 **/ 
	UFUNCTION(BlueprintCallable)
	void HitPlayer(AActor* Player);

	/*
	 * Gets a random position from the available ones in the array
	 */
	static float GetRandomYSpawnPosition();

	/*
	* Sets a new random object group and updates the object's static mesh
	*/
	void RandomizeProperties();

public:	
	// Sets default values for this actor's properties
	AIncomingObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	EGameObjectGroup IncomingObjectGroup;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Starts the creation of incoming objects
	static void StartSpawningIncomingObjects(UWorld* World);

	// Places a single incoming object in front of player
	static void PlaceIncomingObject(UWorld* World);

	// Enables or disables the actor's tick based on the object's group
	UFUNCTION(BlueprintCallable)
	static void ChangeObjectMovement(EGameObjectGroup CurrentActiveGroup);

	/*
	 * Relocates all objects away from track
	 * Note : Does not stop them from spawning again
	 */
	static void RemoveAllObjectsFromTrack();
};
