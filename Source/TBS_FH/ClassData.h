// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Ability.h"
#include "WeaponData.h"
#include "Dialog/Person.h"
#include "GameplayTagContainer.h"
#include "UnitAnimationSetData.h"
#include "ClassData.generated.h"

/**
 * 
 */
UCLASS()
class TBS_FH_API UClassData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FGameplayTagContainer classTags;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UPerson* genericPerson;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UPerson* genericPerson_Override_Player;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UPerson* genericPerson_Override_Partner;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UPerson* genericPerson_Override_NPC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UPerson* genericPerson_Override_Enemy;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UPerson* genericPerson_Override_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UClassData* nextClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "3", UIMin = "1", UIMax = "3"))
		int tier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<UAnimInstance> animationBP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USkeletalMesh* genericClassSkin;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<UAnimInstance> mountAnimationBP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<UUnitAnimationSetData*> animations;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USkeletalMesh* mountSkin;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "5", UIMin = "1", UIMax = "3"))
		float scaleMultiplyer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "50", UIMin = "1", UIMax = "3"))
		float mountScaleMultiplyer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "3", UIMin = "1", UIMax = "3"))
		int placeTiles;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float zOffset;
	UPROPERTY(EditAnywhere, Meta = (Bitmask, BitmaskEnum = "ETraverseType"))
		int32 traverseType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category="Growth")
		int growth_HP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Growth")
		int growth_strength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Growth")
		int growth_magic;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Growth")
		int growth_endurance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Growth")
		int growth_mana;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Growth")
		int growth_defence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Growth")
		int growth_resistence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Growth")
		int growth_skill;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Growth")
		int growth_luck;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Growth")
		int growth_speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Base")
		int base_HP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Base")
		int base_strength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Base")
		int base_magic;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Base")
		int base_endurance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Base")
		int base_mana;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Base")
		int base_defence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Base")
		int base_resistence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Base")
		int base_skill;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Base")
		int base_luck;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Base")
		int base_speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Max")
		int max_HP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Max")
		int max_strength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Max")
		int max_magic;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Max")
		int max_endurance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Max")
		int max_mana;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Max")
		int max_defence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Max")
		int max_resistence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Max")
		int max_skill;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Max")
		int max_luck;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Max")
		int max_speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranks")
		EWeaponRank rank_sword;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranks")
		EWeaponRank rank_lance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranks")
		EWeaponRank rank_axe;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranks")
		EWeaponRank rank_bow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranks")
		EWeaponRank rank_dagger;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranks")
		EWeaponRank rank_ignis;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranks")
		EWeaponRank rank_terra;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranks")
		EWeaponRank rank_aer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranks")
		EWeaponRank rank_aqua;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranks")
		EWeaponRank rank_lux;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranks")
		EWeaponRank rank_umbra;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranks")
		EWeaponRank rank_staff;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranks")
		EWeaponRank rank_slash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<TSubclassOf<UAbility>> gainAbalitys;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "paletteOverrides")
		UTexture2D* genericPalette_Player;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "paletteOverrides")
		UTexture2D* genericPalette_Partner;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "paletteOverrides")
		UTexture2D* genericPalette_NPC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "paletteOverrides")
		UTexture2D* genericPalette_Enemy;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "paletteOverrides")
		UTexture2D* genericPalette_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SkinOverrides")
		USkeletalMesh* genericClassSkin_TeamPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkinOverrides")
		UMaterialInterface* genericClassSkinMaterial_TeamPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkinOverrides")
		USkeletalMesh* genericClassSkin_TeamPartner;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkinOverrides")
		UMaterialInterface* genericClassSkinMaterial_TeamPartner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkinOverrides")
		USkeletalMesh* genericClassSkin_TeamNPC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkinOverrides")
		UMaterialInterface* genericClassSkinMaterial_TeamNPC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkinOverrides")
		USkeletalMesh* genericClassSkin_TeamEnemy;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkinOverrides")
		UMaterialInterface* genericClassSkinMaterial_TeamEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkinOverrides")
		USkeletalMesh* genericClassSkin_TeamNone;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkinOverrides")
		UMaterialInterface* genericClassSkinMaterial_TeamNone;

	UFUNCTION(BlueprintCallable)
		int GetTotalStatValue();
};


