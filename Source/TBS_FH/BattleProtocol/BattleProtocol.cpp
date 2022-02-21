// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleProtocol.h"

void UBattleProtocol::AddProtocolStep(UBattleProtocolStep * _step)
{
	if (_step != nullptr)
	{
		_step->SetProtocol(this);
		protocolSteps.Add(_step);
	}
}

void UBattleProtocol::AddProtocolStepAtID(UBattleProtocolStep * _step, int _id)
{
	if (_step != nullptr)
	{
		_step->SetProtocol(this);
		protocolSteps.EmplaceAt(_id, _step);
	}
}

void UBattleProtocol::AddProtocolStepAfterStep(UBattleProtocolStep * _startStep, UBattleProtocolStep * _newStep)
{
	if (_startStep != nullptr && _newStep != nullptr)
	{
		int id = 0;
		bool found = false;
		for (id = 0; id < protocolSteps.Num() && !found; id++)
		{
			if (protocolSteps[id] == _startStep)
			{
				found = true;
			}
		}
		if (found)
		{
			_newStep->SetProtocol(_startStep->protocol);
			protocolSteps.Insert(_newStep, id);
		}
	}
}

AUnit * UBattleProtocol::GetEnemyOf(AUnit * _unit)
{
	if (_unit == offender)
	{
		return defender;
	}
	else if (_unit == defender)
	{
		return offender;
	}
	return nullptr;
}

void UBattleProtocol::AddWeaponEPFor(AUnit * reciever, int amount)
{
	if (reciever == offender)
	{
		wpEP_Off += amount;
	}
	else if (reciever == defender)
	{
		wpEP_Def += amount;
	}
}

void UBattleProtocol::RunProtocol()
{
	for (int i = 0; i < protocolSteps.Num(); i++)
	{
		protocolSteps[i]->RunStep();
	}
}
