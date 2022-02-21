// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpecialDialogEvent.generated.h"

/**
 * 
 */
UCLASS()
class TBS_FH_API USpecialDialogEvent : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		int eventID;
};
