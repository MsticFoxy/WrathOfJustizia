// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatusEffectData.h"
#include "StatusEffectInstance.generated.h"

class AUnit;
/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UStatusEffectInstance : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStatusEffectData* statusEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int remaining;

	UFUNCTION(BlueprintCallable)
		void RunEffectStart(AUnit* _unit);
	UFUNCTION(BlueprintCallable)
		void RunEffectEnd(AUnit* _unit);
};


