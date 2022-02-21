// Fill out your copyright notice in the Description page of Project Settings.


#include "HealOption.h"
#include "Unit.h"

FName UHealOption::GetActionName()
{
	return "Stab";
}

bool UHealOption::MeetsRequirements(AUnit * _unit)
{
	if (_unit != nullptr)
	{
		if (_unit->state != EUnitState::ECantoEnd)
		{
			TArray<AUnit*> enemys = _unit->tileMap->GetUnitsInStaffRange(_unit);
			if (enemys.Num() > 0)
			{
				return true;
			}
		}
	}

	return false;
}

void UHealOption::Run(AUnit * _unit)
{
	if (_unit != nullptr)
	{
		_unit->selectionPurpose = ESelectionPurpose::EStaff;
		_unit->state = EUnitState::ESelectInteractable;
	}
}
