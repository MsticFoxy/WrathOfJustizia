// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Animation/AnimSequence.h"
#include "UnitAnimationSetData.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Sword,
	Lance,
	Axe,
	Bow,
	Dagger,
	Ignis,
	Terra,
	Aer,
	Aqua,
	Lux,
	Umbra,
	Staff,
	Slash
};

USTRUCT(BlueprintType)
struct FUnitAnimationSet
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EWeaponType type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimSequence* idle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimSequence* battleIdle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimSequence* walk;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimSequence* getHit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimSequence* block;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<UAnimSequence*> nearAttackSequence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<UAnimSequence*> farAttackSequence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimSequence* nearCrit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimSequence* farCrit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimSequence* die;
};

/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UUnitAnimationSetData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FUnitAnimationSet animSet;
};


