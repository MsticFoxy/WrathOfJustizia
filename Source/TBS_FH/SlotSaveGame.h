// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Unit.h"
#include "LevelOrderLink.h"
#include "CBluePrints.h"
#include "GameSlotData.h"
#include "SlotSaveGame.generated.h"


USTRUCT(BlueprintType)
struct FTeamConvoy
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
		int gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
		int bonusEP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
		int genericUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
		TArray<FItemInstanceSave> convoy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
		TArray<TSubclassOf<UAbility>> abilityScrolls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
		TArray<FCharacterData> characters;
};
/**
 * 
 */
UCLASS()
class TBS_FH_API USlotSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame")
		ULevelOrderLink* chapterLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame")
		int currentLevelIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame")
		int run;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame")
		bool hasBookmark;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame")
		FDifficultySettings difficulty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
		TArray<FTeamConvoy> teams;


	UFUNCTION(BlueprintCallable)
		void WriteConvoy(TArray<UItemInstance*> _items, int _team);

	UFUNCTION(BlueprintCallable)
		TArray<UItemInstance*> ReadConvoy(int _team);

	UFUNCTION(BlueprintCallable)
		void WriteCharacters(TArray<FCharacterData> _chars, int _team);

	UFUNCTION(BlueprintCallable)
		TArray<FCharacterData> ReadCharacters(int _team);

	UFUNCTION(BlueprintCallable)
		void WriteSaveGameData(ULevelOrderLink* level, int levelID, int _run, FDifficultySettings _diff);


	UFUNCTION(BlueprintCallable)
		void WriteAll(AGameSlotData* _gameData);

	UFUNCTION(BlueprintCallable)
		void ReadAll(AGameSlotData* _gameData);
};
