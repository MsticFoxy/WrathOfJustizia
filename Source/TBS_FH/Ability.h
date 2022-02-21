// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Ability.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UAbility : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString name;

public:
	UFUNCTION()
		static void Trigger();
};
