// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "StaffData.generated.h"

/**
 * 
 */
UCLASS()
class TBS_FH_API UStaffData : public UWeaponData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FIntervalRange staffRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UParticleSystem* staffEffect;
};
