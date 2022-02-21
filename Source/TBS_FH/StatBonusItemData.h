// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "StatBonusItemData.generated.h"

/**
 * 
 */
UCLASS()
class TBS_FH_API UStatBonusItemData : public UItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int hp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int endurence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int mana;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int strength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int magic;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int defense;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int resistence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int skill;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int luck;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int speed;
};
