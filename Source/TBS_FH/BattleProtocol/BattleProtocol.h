// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BattleProtocolStep.h"
#include "BattleProtocol.generated.h"

/**
 * 
 */
UCLASS()
class TBS_FH_API UBattleProtocol : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UBattleProtocolStep*> protocolSteps;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		AUnit* offender;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		AUnit* defender;

	UPROPERTY(EditAnywhere)
		int prevHP_Off;
	UPROPERTY(EditAnywhere)
		int prevHP_Def;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int range;

	UPROPERTY(EditAnywhere)
		int wpEP_Off;
	UPROPERTY(EditAnywhere)
		EWeaponType weaponType_Off;
	UPROPERTY(EditAnywhere)
		int wpEP_Def;
	UPROPERTY(EditAnywhere)
		EWeaponType weaponType_Def;

	UFUNCTION(BlueprintCallable)
	void AddProtocolStep(UBattleProtocolStep* _step);

	UFUNCTION(BlueprintCallable)
		void AddProtocolStepAtID(UBattleProtocolStep* _step, int _id);

	UFUNCTION(BlueprintCallable)
		void AddProtocolStepAfterStep(UBattleProtocolStep * _startStep, UBattleProtocolStep * _newStep);

	UFUNCTION(BlueprintCallable)
		AUnit* GetEnemyOf(AUnit* _unit);

	UFUNCTION(BlueprintCallable)
		void AddWeaponEPFor(AUnit* reciever, int amount);

	UFUNCTION(BlueprintCallable)
		void RunProtocol();
};
