// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Class.h"
#include "Materials/MaterialInstance.h"

/**
 * 
 */
class UISettings : public UClass
{
public:
	UISettings();
	~UISettings();

	UPROPERTY(EditAnywhere)
		UMaterialInstance* material;
};
