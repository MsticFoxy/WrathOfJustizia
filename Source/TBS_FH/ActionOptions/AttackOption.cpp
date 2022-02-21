// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackOption.h"
#include "Unit.h"
#include "TileMap.h"

FName UAttackOption::GetActionName()
{
	return "Angreifen";
}

bool UAttackOption::MeetsRequirements(AUnit * _unit)
{
	if (_unit != nullptr)
	{
		if (_unit->state != EUnitState::ECantoEnd)
		{
			TArray<AUnit*> enemys = _unit->tileMap->GetAttackableUnits(_unit);
			if (enemys.Num() > 0)
			{
				return true;
			}
		}
	}

	return false;
}

void UAttackOption::Run(AUnit * _unit)
{
	if (_unit != nullptr)
	{
		_unit->state = EUnitState::EAttacking;
	}
}
