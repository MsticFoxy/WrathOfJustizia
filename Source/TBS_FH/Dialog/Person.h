// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "Person.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UPerson : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* portrait;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName name;
};
