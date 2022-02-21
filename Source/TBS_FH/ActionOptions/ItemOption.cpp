// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemOption.h"
#include "Unit.h"

FName UItemOption::GetActionName()
{
	return "Items";
}

bool UItemOption::MeetsRequirements(AUnit * _unit)
{
	if (_unit != nullptr)
	{
		if (_unit->state != EUnitState::ECantoEnd)
		{
			return true;
		}
	}

	return false;
}

void UItemOption::Run(AUnit * _unit)
{
	if (_unit != nullptr)
	{
		_unit->state = EUnitState::EItems;
	}
}