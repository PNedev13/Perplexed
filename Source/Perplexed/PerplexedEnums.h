// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "PerplexedEnums.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EFloorPieceTypeEnum : uint8
{
	FPTE_MIN 	,
	FPTE_Blue 	UMETA(DisplayName = "Blue"),
	FPTE_Red 	UMETA(DisplayName = "Red"),
	FPTE_MAX
};