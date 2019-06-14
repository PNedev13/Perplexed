// Fill out your copyright notice in the Description page of Project Settings.

#include "IncomingObject.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "PerplexedGameMode.h"
#include "PerplexedCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "FloorPiece.h"
#include "Components/StaticMeshComponent.h"
#include "BPImplementablesLibrary.h"

uint8 AIncomingObject::MaxNumberOfObjects = 20;
const float AIncomingObject::SpawnDistanceFromPlayer = 20000.f;
const float AIncomingObject::SpawnZPosition = 500.f;
const float AIncomingObject::InactiveDistanceThreshold = 2000.f;
const uint32 AIncomingObject::ObjectWidth = 1000;
float AIncomingObject::TimeBetweenObjectSpawn = 0.2f;
TWeakObjectPtr<AIncomingObject> AIncomingObject::DataHolderInstance;
TArray<int32> AIncomingObject::AvailableYSpawnPositions;


// Sets default values
AIncomingObject::AIncomingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IncomingObjectGroup = GOGEnumFunctionLibrary::GetRandomEnum();
	
}

// Called when the game starts or when spawned
void AIncomingObject::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	
	UStaticMeshComponent* comp = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (comp != nullptr)
	{
		comp->OnComponentHit.AddDynamic(this, &AIncomingObject::OnHit);
		comp->OnComponentBeginOverlap.AddDynamic(this, &AIncomingObject::OnOverlap);
	}

	
}

// Called every frame
void AIncomingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move object
	float XInterp = FMath::FInterpTo(GetActorLocation().X, InterpolationTargetX, DeltaTime, 0.5f);
	SetActorLocation({ XInterp, GetActorLocation().Y, GetActorLocation().Z }, true);
}

void AIncomingObject::StartSpawningIncomingObjects(UWorld* World)
{
	if (World == nullptr)
		return;

	SpawnAllIncomingObjects(World);

	CalculateYSpawnLocations();
	
	// Setup a timer for the "PlaceIncomingObject" method
	FTimerDelegate TimerDel = FTimerDelegate::CreateStatic(&AIncomingObject::PlaceIncomingObject, World);
	FTimerHandle TimerHandle;
	//Calling "PlaceIncomingObject" every "TimeBetweenObjectSpawn" seconds
	World->GetTimerManager().SetTimer(TimerHandle, TimerDel, TimeBetweenObjectSpawn, true);
}

void AIncomingObject::CalculateYSpawnLocations()
{
	// Calculate the maximum number of objects that fit in one row
	int32 MaxIncomingObjectsInARow = AFloorPiece::NumberOfFloorPiecesInARow * (int32)((AFloorPiece::FloorSize) / ObjectWidth);
	for (int32 cnt = 0; cnt < MaxIncomingObjectsInARow; ++cnt)
	{
		int32 YPosition = cnt * ObjectWidth - ObjectWidth / 2;
		// Add the floor pieces margin in the placing position
		YPosition += ((cnt * ObjectWidth) / (int32)AFloorPiece::FloorSize) * AFloorPiece::FloorPieceYPlacingMargin;
		AvailableYSpawnPositions.Emplace(YPosition);
	}
}

void AIncomingObject::SpawnAllIncomingObjects(UWorld* World)
{
	APerplexedGameMode* GameMode = Cast<APerplexedGameMode>(World->GetAuthGameMode());
	if (GameMode == nullptr)
		return;
	if (GameMode->GetIncomingObjectClasses().Num() == 0)
		return;

	for (int32 cnt = 0; cnt < MaxNumberOfObjects; ++cnt)
	{
		int32 RandomClassArrayIndex = FMath::RandRange(0, GameMode->GetIncomingObjectClasses().Num() - 1);

		AIncomingObject* NewIncomingObject = World->SpawnActor<AIncomingObject>(
			GameMode->GetIncomingObjectClasses()[RandomClassArrayIndex], {-100000.f, 0.f, 0.f}, FRotator::ZeroRotator);

		if (!DataHolderInstance.IsValid())
			DataHolderInstance = NewIncomingObject;
		DataHolderInstance->InactiveIncomingObjects.Add(NewIncomingObject);
	}
}

void AIncomingObject::PlaceIncomingObject(UWorld* World)
{
	APerplexedGameMode* GameMode = Cast<APerplexedGameMode>(World->GetAuthGameMode());
	APerplexedCharacter* PlayerCharacter = Cast<APerplexedCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));

	if (GameMode == nullptr || PlayerCharacter == nullptr)
		return;
	if (!DataHolderInstance.IsValid())
		return;
	if (AvailableYSpawnPositions.Num() == 0)
	{
		CheckForInactiveObjects(PlayerCharacter->GetActorLocation().X);
		return;
	}

	int32 RandomIncomingObjectIndex = FMath::RandRange(0, DataHolderInstance->InactiveIncomingObjects.Num() - 1);
	if (DataHolderInstance->InactiveIncomingObjects.IsValidIndex(RandomIncomingObjectIndex))
	{
		AIncomingObject* RandomIncomingObject = DataHolderInstance->InactiveIncomingObjects[RandomIncomingObjectIndex];
		if (RandomIncomingObject == nullptr)
			return;

		if (AvailableYSpawnPositions.Num() == 0)
			return;

		DataHolderInstance->InactiveIncomingObjects.RemoveAt(RandomIncomingObjectIndex, 1, false);

		RandomIncomingObject->IncomingObjectGroup = GOGEnumFunctionLibrary::GetRandomEnum();
		if (UBPImplementablesLibrary::Instance.IsValid())
			UBPImplementablesLibrary::Instance->UpdateStaticMeshMaterial(RandomIncomingObject, RandomIncomingObject->IncomingObjectGroup);


		float RandomYSpawnPosition = 0.f;
		int32 RandomArrayIndex = FMath::RandRange(0, AvailableYSpawnPositions.Num() - 1);
		if (AvailableYSpawnPositions.IsValidIndex(RandomArrayIndex))
		{
			RandomYSpawnPosition = AvailableYSpawnPositions[RandomArrayIndex];
			AvailableYSpawnPositions.RemoveAt(RandomArrayIndex, 1, false);
		}

		RandomIncomingObject->SetActorLocation({ PlayerCharacter->GetActorLocation().X + SpawnDistanceFromPlayer, RandomYSpawnPosition, SpawnZPosition });
		RandomIncomingObject->InterpolationTargetX = PlayerCharacter->GetActorLocation().X - SpawnDistanceFromPlayer;

		DataHolderInstance->ActiveIncomingObjects.Add(RandomIncomingObject);
	}
}

void AIncomingObject::ChangeObjectMovement(EGameObjectGroup CurrentActiveGroup)
{
	if (!DataHolderInstance.IsValid())
		return;
	for (auto IncomingObject : DataHolderInstance->ActiveIncomingObjects)
	{
		if (IncomingObject->IncomingObjectGroup == CurrentActiveGroup)
			IncomingObject->SetActorTickEnabled(true);
		else
			IncomingObject->SetActorTickEnabled(false);
	}
}

void AIncomingObject::CheckForInactiveObjects(float PlayerXPosition)
{
	if (!DataHolderInstance.IsValid())
		return;
	for (int32 index = 0; index < DataHolderInstance->ActiveIncomingObjects.Num(); ++index)
	{
		if (PlayerXPosition - DataHolderInstance->ActiveIncomingObjects[index]->GetActorLocation().X > InactiveDistanceThreshold)
		{
			AIncomingObject* TempObjPointer = DataHolderInstance->ActiveIncomingObjects[index];
			TempObjPointer->SetActorTickEnabled(false);
			DataHolderInstance->ActiveIncomingObjects.RemoveAt(index, 1, false);
			AvailableYSpawnPositions.Emplace(TempObjPointer->GetActorLocation().Y);
			DataHolderInstance->InactiveIncomingObjects.Add(TempObjPointer);
		}
	}
}

void AIncomingObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr)
		return;

	// Do nothing if the player hits the side or back of the cube
	if (GetActorLocation().X < OtherActor->GetActorLocation().X)
		return;

	HitPlayer(OtherActor);
}
void AIncomingObject::OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
		return;

	if (OtherActor->IsA(AIncomingObject::StaticClass()))
	{
		SetActorLocation({ 0.f,GetActorLocation().Y,0.f });
		return;
	}

	HitPlayer(OtherActor);
}

void AIncomingObject::HitPlayer(AActor* Player)
{
	APerplexedCharacter* PlayerCharacter = Cast<APerplexedCharacter>(Player);

	if (IsActorTickEnabled() == false || !IsValid(PlayerCharacter))
		return;

	SetActorTickEnabled(false);
	SetActorLocation({ 0.f,GetActorLocation().Y,0.f });
	PlayerCharacter->LaunchCharacter({ -10000.f, 0.f, 300.f }, true, false);
}