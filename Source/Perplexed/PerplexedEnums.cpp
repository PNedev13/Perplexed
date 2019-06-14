// Fill out your copyright notice in the Description page of Project Settings.

#include "PerplexedEnums.h"

EGameObjectGroup GOGEnumFunctionLibrary::GetRandomEnum()
{ 
	int32 NewRandomEnum = FMath::RandRange((int32)EGameObjectGroup::GOG_MIN + 1, (int32)EGameObjectGroup::GOG_MAX - 1);
	return static_cast<EGameObjectGroup>(NewRandomEnum);
}