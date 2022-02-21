// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatusEffect/StatusEffectData.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class ERarity : uint8
{
	Common,
	Uncommon,
	Rare,
	Epic,
	Legendary,
	Holy
};

USTRUCT(BlueprintType)
struct FStatusEffectPropapility
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStatusEffectData* effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int percent;
};
/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UItemData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ERarity rarity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int durability;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* itemSprite;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int shopCost;

};

USTRUCT(BlueprintType)
struct FItemInstanceSave
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UItemData* item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int durability;
	UPROPERTY(EditAnywhere)
		bool broken;
	UPROPERTY(EditAnywhere)
		bool drop;
	UPROPERTY(EditAnywhere)
		bool bound;
};

UCLASS(BlueprintType)
class TBS_FH_API UItemInstance : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UItemData* item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int durability;
	UPROPERTY(EditAnywhere)
		bool broken;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool drop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bound;
};

USTRUCT(BlueprintType)
struct FShopItem
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UItemData* item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int customCost;
};
