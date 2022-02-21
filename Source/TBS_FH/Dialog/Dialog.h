// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Person.h"
#include "DialogEvent.h"
#include "Dialog.generated.h"

USTRUCT(BlueprintType)
struct FSpeakEvent
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UPerson* person;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool showSpeaker;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool showName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int speakerPosition;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText text;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<UDialogEvent*> postSpeakEvent;
};

/**
 * 
 */
UCLASS(BlueprintType)
class TBS_FH_API UDialog : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FSpeakEvent> dialog;
};
