// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PerplexedEnums.h"
#include "BPImplementablesLibrary.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PERPLEXED_API UBPImplementablesLibrary : public UObject
{
	GENERATED_BODY()

	
public:
	UBPImplementablesLibrary();
	
	static TWeakObjectPtr<UBPImplementablesLibrary> Instance;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateStaticMeshMaterial(AActor* MeshOwner, EGameObjectGroup GameGroup);

	UFUNCTION(BlueprintCallable)
		static void SetInstance(UBPImplementablesLibrary* NewInstance) { Instance = NewInstance; };
};
