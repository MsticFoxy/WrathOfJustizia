// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Unit.h"
#include "Ability.h"
#include "BattleProtocolStep.generated.h"

class UBattleProtocol;

/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UBattleProtocolStep : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		FName stepName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		AUnit* owner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UBattleProtocol* protocol;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<TSubclassOf<UAbility>> canNotRunAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<TSubclassOf<UAbility>> executionAbility;

	UFUNCTION(BlueprintCallable)
		void SetOwner(AUnit* _owner);

	UFUNCTION(BlueprintCallable)
		void SetProtocol(UBattleProtocol* _protocol);

	UFUNCTION(BlueprintCallable)
		void AddProtocolStepAfterThis(UBattleProtocolStep* _step);

	UFUNCTION(BlueprintCallable)
		virtual void RunStep();

	UFUNCTION(BlueprintCallable)
		bool CanStillUseStep();

	
};
