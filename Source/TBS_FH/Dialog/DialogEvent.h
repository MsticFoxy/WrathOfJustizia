// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogEvent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UDialogEvent : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		int eventID;
};
