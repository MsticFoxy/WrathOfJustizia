// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectInstance.h"
#include "Unit.h"

void UStatusEffectInstance::RunEffectStart(AUnit* _unit)
{
	_unit->Damage(statusEffect->onTurnStart_damage);
	_unit->UseEndurance(statusEffect->onTurnStart_looseEndurence);
	_unit->UseMana(statusEffect->onTurnStart_looseMana);
	_unit->Heal(statusEffect->onTurnStart_heal);
	_unit->GainEndurance(statusEffect->onTurnStart_gainEndurence);
	_unit->GainMana(statusEffect->onTurnStart_gainMana);
}

void UStatusEffectInstance::RunEffectEnd(AUnit* _unit)
{
	_unit->Damage(statusEffect->onTurnEnd_damage);
	_unit->UseEndurance(statusEffect->onTurnEnd_looseEndurence);
	_unit->UseMana(statusEffect->onTurnEnd_looseMana);
	_unit->Heal(statusEffect->onTurnEnd_heal);
	_unit->GainEndurance(statusEffect->onTurnEnd_gainEndurence);
	_unit->GainMana(statusEffect->onTurnEnd_gainMana);
}
