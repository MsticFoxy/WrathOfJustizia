// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Ability.h"
#include "WeaponData.h"
#include "AbilityData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UAbilityData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<UAbility> ability;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* abilitySprite;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ERarity rarity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText description;
};
