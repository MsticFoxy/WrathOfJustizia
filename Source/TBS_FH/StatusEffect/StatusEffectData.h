// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatusEffectData.generated.h"


UENUM(BlueprintType)
enum class EExpandRythm : uint8
{
	EStart,
	EEnd
};
/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UStatusEffectData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Data")
		UTexture2D* sprite;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Data")
		UParticleSystem* triggerEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Data")
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Data")
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Data")
		bool infinite;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Data")
		int duration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Data")
		EExpandRythm expandRythm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Data")
		bool showResourceChangeOnStart;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Data")
		bool showResourceChangeOnEnd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="On Turn Start")
		int onTurnStart_heal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Turn Start")
		int onTurnStart_damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Turn Start")
		int onTurnStart_gainEndurence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Turn Start")
		int onTurnStart_looseEndurence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Turn Start")
		int onTurnStart_gainMana;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Turn Start")
		int onTurnStart_looseMana;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Turn End")
		int onTurnEnd_heal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Turn End")
		int onTurnEnd_damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Turn End")
		int onTurnEnd_gainEndurence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Turn End")
		int onTurnEnd_looseEndurence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Turn End")
		int onTurnEnd_gainMana;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Turn End")
		int onTurnEnd_looseMana;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Duration of Effect")
		int onDuration_movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Duration of Effect")
		int onDuration_strength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Duration of Effect")
		int onDuration_magic;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Duration of Effect")
		int onDuration_defense;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Duration of Effect")
		int onDuration_resistence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Duration of Effect")
		int onDuration_skill;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Duration of Effect")
		int onDuration_luck;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "On Duration of Effect")
		int onDuration_speed;
};
