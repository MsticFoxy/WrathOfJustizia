// Fill out your copyright notice in the Description page of Project Settings.


#include "RestOption.h"
#include "Unit.h"
#include "Abilities/Canto.h"

FName URestOption::GetActionName()
{
	return "Ausruhen";
}

bool URestOption::MeetsRequirements(AUnit * _unit)
{
	bool canRun = false;
	if (_unit->GetCurrentMana() < FMath::FloorToInt(_unit->GetMana() *0.5f))
	{
		canRun = true;
	}
	if (_unit->GetCurrentEndurance() < FMath::FloorToInt(_unit->GetEndurance() *0.5f))
	{
		canRun = true;
	}
	return canRun;
}

void URestOption::Run(AUnit * _unit)
{
	if (_unit->GetCurrentMana() < (_unit->GetMana() *0.5f) - 5)
	{
		int gain = FMath::FloorToInt(_unit->GetMana() *0.5f) - _unit->GetCurrentMana();
		if (gain > 0)
		{
			_unit->GainMana(gain);
		}
	}
	else
	{
		_unit->GainMana(5);
	}
	if (_unit->GetCurrentEndurance() < (_unit->GetEndurance() *0.5f)-5)
	{
		int gain = FMath::FloorToInt(_unit->GetEndurance() *0.5f) - _unit->GetCurrentEndurance();
		if (gain > 0)
		{
			_unit->GainEndurance(gain);
		}
	}
	else
	{
		_unit->GainEndurance(5);
	}
	_unit->OnRest();

	if (_unit->GetAbilities().Contains(UCanto::StaticClass()))
	{
		_unit->state = EUnitState::ECanto;
	}
	else
	{
		_unit->state = EUnitState::EWaiting;
	}
}
