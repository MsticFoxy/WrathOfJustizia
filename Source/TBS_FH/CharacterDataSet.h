// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ClassData.h"
#include "ItemData.h"
#include "WeaponData.h"
#include "GameplayTagContainer.h"
#include "CharacterDataSet.generated.h"

USTRUCT(BlueprintType)
struct FItemContainer
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UItemData* _item;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int customDurability;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool drop;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bound;
};

USTRUCT(BlueprintType)
struct FAnimaData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		UClassData* characterClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		EDamageType animaResource;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		USkeletalMesh* customSkinTier_1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		USkeletalMesh* customSkinTier_2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		USkeletalMesh* customSkinTier_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		UPerson* person;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		UWeaponData* boundWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<TSubclassOf<UAbility>> characterAbilitys;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_HP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_strength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_magic;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_endurance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_mana;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_defence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_resistence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_skill;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_luck;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_sword;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_lance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_axe;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_bow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_dagger;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_ignis;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_terra;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_aer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_aqua;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_lux;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_umbra;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_staff;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_slash;
};

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		UClassData* characterClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		FGameplayTagContainer additionalTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		UTexture2D* characterPalette;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		USkeletalMesh* customSkinTier_1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		USkeletalMesh* customSkinTier_2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		USkeletalMesh* customSkinTier_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		TArray<FItemContainer> startItems;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		bool inventoryInitiated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		TArray<UItemInstance*> inventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		TArray<FItemInstanceSave> inventorySave;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		UPerson* person;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		FString characterName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		FAnimaData anima;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		int currentHP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		int currentEndurance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		int currentMana;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (ClampMin = "1", ClampMax = "21.0", UIMin = "1.0", UIMax = "20.0"))
		int level;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		int ep;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
		bool dead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<TSubclassOf<UAbility>> characterAbilitys;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_HP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_strength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_magic;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_endurance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_mana;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_defence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_resistence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_skill;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_luck;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int stat_speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int growth_HP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int growth_strength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int growth_magic;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int growth_endurance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int growth_mana;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int growth_defence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int growth_resistence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int growth_skill;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int growth_luck;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Stats")
		int growth_speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_sword;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_lance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_axe;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_bow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_dagger;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_ignis;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_terra;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_aer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_aqua;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_lux;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_umbra;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_staff;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/Ranks")
		EWeaponRank current_rank_slash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/RankEP")
		int current_rankEP_sword;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/RankEP")
		int current_rankEP_lance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/RankEP")
		int current_rankEP_axe;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/RankEP")
		int current_rankEP_bow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/RankEP")
		int current_rankEP_dagger;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/RankEP")
		int current_rankEP_ignis;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/RankEP")
		int current_rankEP_terra;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/RankEP")
		int current_rankEP_aer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/RankEP")
		int current_rankEP_aqua;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/RankEP")
		int current_rankEP_lux;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/RankEP")
		int current_rankEP_umbra;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/RankEP")
		int current_rankEP_staff;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/RankEP")
		int current_rankEP_slash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/track")
		int track_kills;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/track")
		int track_damageCaused;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character/track")
		int track_damageTaken;

		int GetTotalStatValue();

		int GetHP();
		int GetStrength();
		int GetMagic();
		int GetEndurance();
		int GetMana();
		int GetDefence();
		int GetResistence();
		int GetSkill();
		int GetLuck();
		int GetSpeed();

		int GetTotalGrowthValue();

		int GetGrowthHP();
		int GetGrowthStrength();
		int GetGrowthMagic();
		int GetGrowthEndurance();
		int GetGrowthMana();
		int GetGrowthDefence();
		int GetGrowthResistence();
		int GetGrowthSkill();
		int GetGrowthLuck();
		int GetGrowthSpeed();
};
/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UCharacterDataSet : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FCharacterData characterSet;
};
