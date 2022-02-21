// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.h"
#include "ShopItemList.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UShopItemList : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FShopItem> items;
};
