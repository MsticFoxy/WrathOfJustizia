// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionOption.h"
#include "MapSpecials/Treasure.h"
#include "OpenTreasure.generated.h"

/**
 * 
 */
UCLASS()
class TBS_FH_API UOpenTreasure : public UActionOption
{
	GENERATED_BODY()
	
public:

	virtual FName GetActionName() override;

	virtual bool MeetsRequirements(AUnit* _unit) override;

	virtual void Run(AUnit* _unit) override;

	UPROPERTY(EditAnywhere)
	ATreasure* treasure;
};
