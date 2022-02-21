// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CBluePrints.h"
#include "DifficultiyPreset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UDifficultiyPreset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FDifficultySettings settings;
};
