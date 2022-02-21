// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitOption.h"
#include "Unit.h"

FName UWaitOption::GetActionName()
{
	return "Warten";
}

bool UWaitOption::MeetsRequirements(AUnit* _unit)
{
	if (_unit != nullptr)
	{
		return true;
	}
	return false;
}

void UWaitOption::Run(AUnit* _unit)
{
	if (_unit != nullptr)
	{
		_unit->state = EUnitState::EWaiting;
	}
}