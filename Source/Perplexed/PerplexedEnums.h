// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "PerplexedEnums.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EGameObjectGroup : uint8
{
	GOG_MIN 	,
	GOG_Blue 	UMETA(DisplayName = "Blue"),
	GOG_Red 	UMETA(DisplayName = "Red"),
	GOG_Purple 	UMETA(DisplayName = "Purple"),
	GOG_MAX
};



class GOGEnumFunctionLibrary
{
public:
	static EGameObjectGroup GetRandomEnum();
};