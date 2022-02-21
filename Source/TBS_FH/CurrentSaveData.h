// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CurrentSaveData.generated.h"

/**
 * 
 */
UCLASS()
class TBS_FH_API UCurrentSaveData : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName saveSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool useBookmark;
};
