// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Unit.h"
#include "MapSpecials/Treasure.h"
#include "Leaver.h"
#include "BookmarkSave.generated.h"

USTRUCT(BlueprintType)
struct FStatusEffectSave
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStatusEffectData* statusEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int remaining;
};

USTRUCT(BlueprintType)
struct FUnitDataSave
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		EUnitTeam team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool autoGenerateStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool addManualStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool blocksShadow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool blocksAttacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		TArray<FStatusEffectSave> statusEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		AActor* spawnedBy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool canMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ANavMap* tileMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UNavTile* currentTile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UNavTile* lastTile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FRotator lookRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EUnitState state;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int alreadyMoved;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBehaviorTree* behavior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int spawnedByUnitID;
};

/**
 * 
 */
UCLASS()
class TBS_FH_API UBookmarkSave : public USaveGame
{
	GENERATED_BODY()
public:
	UBookmarkSave();
	~UBookmarkSave();

	UFUNCTION(BlueprintCallable)
		void RegisterUnits(TArray<AUnit*> _units);

	UFUNCTION(BlueprintCallable)
		void ResolveUnits();

	UFUNCTION(BlueprintCallable)
		void RegisterTreasures(TArray<ATreasure*> _treasures);

	UFUNCTION(BlueprintCallable)
		void ResolveTreasures();

	UFUNCTION(BlueprintCallable)
		void RegisterLeavers(TArray<ALeaver*> _leavers);

	UFUNCTION(BlueprintCallable)
		void ResolveLeavers();

	UFUNCTION(BlueprintCallable)
		FUnitDataSave GetUnitData(AUnit* _unit);

	UFUNCTION(BlueprintCallable)
		void ResolveUnitData(AUnit* _unit, FUnitDataSave unitDataSave);

	UFUNCTION(BlueprintCallable)
		void WriteUnitInventoryData(AUnit* _unit);

	UFUNCTION(BlueprintCallable)
		void ResolveUnitInventoryData(AUnit* _unit);

	UFUNCTION(BlueprintCallable)
		void WriteTreasureData(ATreasure* _treasure);

	UFUNCTION(BlueprintCallable)
		void ResolveTreasureData(ATreasure* _treasure, FTreasureSave _tresureSave);

	UFUNCTION(BlueprintCallable)
		void SpawnUnit(int _unitID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units")
		TArray<FCharacterData> characters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units")
		TArray<AUnit*> unitRefs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units")
		TArray<FUnitDataSave> unitData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
		int turn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
		TArray<ATreasure*> treasures;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
		TArray<FTreasureSave> treasureSaves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
		TArray<ALeaver*> leavers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
		TArray<bool> leaverSaves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
		ANavMap* _navMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
		UNavTile* cursorTile;
};
