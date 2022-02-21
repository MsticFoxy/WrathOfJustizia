// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ActionOption.h"
#include "ActionOptionData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UActionOptionData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<UActionOption> actionOption;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText name;
};
