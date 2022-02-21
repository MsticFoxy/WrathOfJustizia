// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TileData.h"
#include "stdlib.h"
#include <vector>
#include "Engine/Engine.h"
#include "UnrealMathUtility.h"
#include "NavTile.generated.h"

class AUnit;
/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UNavTile : public UObject
{
	GENERATED_BODY()
public:
	UNavTile();
	~UNavTile();

	void init(int _x, int _y);

	int x;
	int y;

	int splitWS_x;
	int splitWS_y;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTileData* tileData;
	int height;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		AUnit* unit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int currentWeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int> tempWeights;
	
};
