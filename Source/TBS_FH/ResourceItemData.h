// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "ResourceItemData.generated.h"

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	Endurance,
	Mana
};

/**
 * 
 */
UCLASS()
class TBS_FH_API UResourceItemData : public UItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EResourceType type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int amount;
};
