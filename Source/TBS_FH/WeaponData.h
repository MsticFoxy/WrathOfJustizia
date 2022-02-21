// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.h"
#include "NavMap.h"
#include "WeaponActor.h"
#include "GameplayTagContainer.h"
#include "UnitAnimationSetData.h"
#include "WeaponData.generated.h"


UENUM(BlueprintType)
enum class EWeaponRank : uint8
{
	None = 0,
	E = 1,
	D = 2,
	C = 3,
	B = 4,
	A = 5,
	S = 6,
	SS = 7
};


UENUM(BlueprintType)
enum class EDamageType : uint8
{
	DT_Physical,
	DT_Magical
};

UENUM(BlueprintType)
enum class EWeaponAdvantages : uint8
{
	WA_Advantage,
	WA_None,
	WA_Disadvantage
};

/**
 * 
 */
UCLASS()
class TBS_FH_API UWeaponData : public UItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EWeaponType type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UUnitAnimationSetData* customAnimationSet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EDamageType damageType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FGameplayTagContainer effectiveness;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FStatusEffectPropapility> statusEffects;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int cost;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EWeaponRank rank;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int strength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int weight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int hit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int crit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool dualAttack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FIntervalRange range;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMesh* weaponModel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<AWeaponActor> weaponBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UParticleSystem* AttackStartEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UParticleSystem* AttackTargetEffect;

	UFUNCTION(BlueprintCallable)
		static EWeaponAdvantages GetWeaponAdvantage(EWeaponType offender, EWeaponType defender);
};
