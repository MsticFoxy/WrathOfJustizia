// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimaSoulOption.h"
#include "Unit.h"
#include "Abilities/Canto.h"
#include "Abilities/AnimaSoul.h"

FName UAnimaSoulOption::GetActionName()
{
	return "Anima Seele";
}

bool UAnimaSoulOption::MeetsRequirements(AUnit * _unit)
{
	if (_unit != nullptr)
	{
		if (_unit->state != EUnitState::ECantoEnd)
		{
			if (_unit->GetAbilities().Contains(UAnimaSoul::StaticClass()))
			{
				if (_unit->characterData.anima.animaResource == EDamageType::DT_Magical)
				{
					if (_unit->GetCurrentMana() >= FMath::FloorToInt(0.5f* _unit->GetMana()))
					{
						return true;
					}
				}
				else if (_unit->characterData.anima.animaResource == EDamageType::DT_Physical)
				{
					if (_unit->GetCurrentEndurance() >= FMath::FloorToInt(0.5f* _unit->GetEndurance()))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

void UAnimaSoulOption::Run(AUnit * _unit)
{
	if (_unit != nullptr)
	{
		_unit->SpawnAnima(true);
		if (_unit->characterData.anima.animaResource == EDamageType::DT_Magical)
		{
			_unit->UseMana(FMath::FloorToInt(_unit->GetMana()*0.25f));
		}
		else if (_unit->characterData.anima.animaResource == EDamageType::DT_Physical)
		{
			_unit->UseEndurance(FMath::FloorToInt(_unit->GetEndurance()*0.25f));
		}
		if (_unit->GetAbilities().Contains(UCanto::StaticClass()))
		{
			_unit->state = EUnitState::ECanto;
		}
		else
		{
			_unit->state = EUnitState::EWaiting;
		}
	}
}