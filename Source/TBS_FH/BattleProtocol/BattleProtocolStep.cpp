// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleProtocolStep.h"
#include "BattleProtocol.h"

void UBattleProtocolStep::SetOwner(AUnit * _owner)
{
	if (_owner != nullptr)
	{
		owner = _owner;
	}
}

void UBattleProtocolStep::SetProtocol(UBattleProtocol * _protocol)
{
	if (_protocol != nullptr)
	{
		protocol = _protocol;
	}
}

void UBattleProtocolStep::AddProtocolStepAfterThis(UBattleProtocolStep * _step)
{
	if (_step != nullptr)
	{
		protocol->AddProtocolStepAfterStep(this, _step);
	}
}

void UBattleProtocolStep::RunStep()
{
}

bool UBattleProtocolStep::CanStillUseStep()
{
	if (owner->characterData.currentHP <= 0 )
	{
		return false;
	}
	if (owner->GetEquippedItemInstance()->broken)
	{
		return false;
	}
	if (!owner->IsInRangeOfCurrentWeapon(protocol->GetEnemyOf(owner)))
	{
		return false;
	}
	if (!owner->CanUseWeapon(owner->GetEquippedWeapon()))
	{
		return false;
	}
	if (protocol->GetEnemyOf(owner)->characterData.currentHP <= 0)
	{
		return false;
	}
	if (!owner->ItemIsUsable(owner->GetEquippedItemInstance()))
	{
		return false;
	}
	
	return true;
}
