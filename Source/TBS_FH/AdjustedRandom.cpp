// Fill out your copyright notice in the Description page of Project Settings.


#include "AdjustedRandom.h"

AdjustedRandom::AdjustedRandom()
{
}

AdjustedRandom::~AdjustedRandom()
{
}

int AdjustedRandom::GetRandom()
{
	int r1 = FMath::RandRange(0, 99);
	int r2 = FMath::RandRange(0, 99);
	int ret = FMath::RoundToInt(((float)r1 + (float)r2) / 2.0f);
	return ret;
}

int AdjustedRandom::GetRandomReversed()
{
	return 99 - GetRandom();
}
