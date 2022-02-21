// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleProtocol/BattleProtocolStep.h"
#include "BPS_Attack.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UBPS_Attack : public UBattleProtocolStep
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool hit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool crit;

	virtual void RunStep() override;

	
		
};
