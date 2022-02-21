// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "TileData.generated.h"

UENUM(Meta = (Bitflags))
enum class ETraverseType
{
	TT_Flying,
	TT_Walking,
	TT_Swimming,

	NotValid
};

USTRUCT(BlueprintType)
struct FCustomCostByGameplayTag
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FGameplayTag tag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int cost;
};
#define TEST_BIT(Bitmask, Bit) (((Bitmask) & (1 << static_cast<uint32>(Bit))) > 0)
#define SET_BIT(Bitmask, Bit) (Bitmask |= 1 << static_cast<uint32>(Bit))
#define CLEAR_BIT(Bitmask, Bit) (Bitmask &= ~(1 << static_cast<uint32>(Bit)))

/**
 * 
 */
UCLASS(Blueprintable)
class TBS_FH_API UTileData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
		int id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
		int movementCosts = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
		int flyingCosts = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
		TArray<FCustomCostByGameplayTag> customCosts;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
		bool show = true;
	UPROPERTY(EditAnywhere, Meta = (Bitmask, BitmaskEnum = "ETraverseType"), Category = "General")
		int32 traverseType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		bool restable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		bool blocksAttacks;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		int healthRegeneration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		int defence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		int resistence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		int avoid;
};
