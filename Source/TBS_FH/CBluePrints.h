// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "NavMap.h"
#include "CBluePrints.generated.h"

class AGameSlotData;

UENUM(BlueprintType)
enum class EDifficulty : uint8
{
	DC_Normal,
	DC_Hard,
	DC_Maddening
};

USTRUCT(BlueprintType)
struct FDifficultySettings 
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
		bool isCustomDifficulty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
		EDifficulty mapDifficulty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
		float epMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EP")
		float growthMultiplier_Player = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EP")
		float growthMultiplier_Partner = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EP")
		float growthMultiplier_NPC = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EP")
		float growthMultiplier_Enemy = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EP")
		float growthMultiplier_None = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
		float goldMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
		float weaponRangEpMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
		float shopCostMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
		float bonusEpMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
		int startGold;

};

/**
 *
 */
UCLASS()
class TBS_FH_API UCBluePrints : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "CBluePrintLibrary")
		static TArray<UObject*>  LoadObjectLibrary(const FString& Path, TSubclassOf<UObject> ObjectClass);

	UFUNCTION(BlueprintCallable, Category = "CBluePrintLibrary")
		static int GetRangeMax(FIntervalRange _interval);
	UFUNCTION(BlueprintCallable, Category = "CBluePrintLibrary")
		static int GetRangeMin(FIntervalRange _interval);
	

};