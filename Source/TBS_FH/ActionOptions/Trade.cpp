// Fill out your copyright notice in the Description page of Project Settings.


#include "Trade.h"
#include "Unit.h"

FName UTrade::GetActionName()
{
	return "Tausch";
}

bool UTrade::MeetsRequirements(AUnit * _unit)
{
	if (_unit != nullptr)
	{
		if (_unit->state != EUnitState::ECantoEnd)
		{
			TArray<AUnit*> units = _unit->tileMap->GetTeamNeightbors(_unit);

			if (units.Num() > 0)
			{
				return true;
			}
		}
	}

	return false;
}

void UTrade::Run(AUnit * _unit)
{
	if (_unit != nullptr)
	{
		_unit->state = EUnitState::ETradeSelect;
	}
}