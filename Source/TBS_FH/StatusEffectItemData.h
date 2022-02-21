// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "StatusEffectItemData.generated.h"

/**
 * 
 */
UCLASS()
class TBS_FH_API UStatusEffectItemData : public UItemData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FStatusEffectPropapility> statusEffects;
};
