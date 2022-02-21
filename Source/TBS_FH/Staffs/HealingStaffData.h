// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StaffData.h"
#include "HealingStaffData.generated.h"

/**
 * 
 */
UCLASS()
class TBS_FH_API UHealingStaffData : public UStaffData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int healAmount;
};
