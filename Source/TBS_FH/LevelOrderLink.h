// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ShopItemList.h"
#include "LevelOrderLink.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API ULevelOrderLink : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName chapterName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FName> levelOrder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ULevelOrderLink* nextChapter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UShopItemList* shop;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int teamID;
};
