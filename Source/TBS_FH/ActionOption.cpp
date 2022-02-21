// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionOption.h"
#include "Unit.h"

FName UActionOption::GetActionName()
{
	return FName();
}

bool UActionOption::MeetsRequirements(AUnit * _unit)
{
	return false;
}

void UActionOption::Run(AUnit* _unit)
{
}
