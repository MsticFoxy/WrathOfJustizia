// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TBS_FH_API AdjustedRandom
{
public:
	AdjustedRandom();
	~AdjustedRandom();

	static int GetRandom();
	static int GetRandomReversed();
};
