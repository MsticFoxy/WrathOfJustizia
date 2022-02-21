// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionOption.generated.h"

class AUnit;

/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UActionOption : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		virtual FName GetActionName();
	UFUNCTION(BlueprintCallable)
		virtual bool MeetsRequirements(AUnit* _unit);
	UFUNCTION(BlueprintCallable)
		virtual void Run(AUnit* _unit);
};
