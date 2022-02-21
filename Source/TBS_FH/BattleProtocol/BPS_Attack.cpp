// Fill out your copyright notice in the Description page of Project Settings.


#include "BPS_Attack.h"
#include "BattleProtocol.h"
#include "AdjustedRandom.h"
#include "Abilities/Adept.h"


void UBPS_Attack::RunStep()
{
	int _x = FMath::Abs(owner->currentTile->x - protocol->GetEnemyOf(owner)->currentTile->x);
	int _y = FMath::Abs(owner->currentTile->y - protocol->GetEnemyOf(owner)->currentTile->y);

	if (CanStillUseStep())
	{
		if (owner->GetAbilities().Contains(UAdept::StaticClass()))
		{
			if (AdjustedRandom::GetRandomReversed() < owner->GetSkill() && !canNotRunAbilities.Contains(UAdept::StaticClass()))
			{
				UBPS_Attack* atk = NewObject<UBPS_Attack>();
				atk->canNotRunAbilities.Add(UAdept::StaticClass());
				atk->SetOwner(owner);
				atk->damage = damage;
				if (AdjustedRandom::GetRandom() < AUnit::GetOffenderHit(owner, protocol->GetEnemyOf(owner)))
				{
					atk->hit = true;
				}
				if (AdjustedRandom::GetRandom() < AUnit::GetOffenderCrit(owner, protocol->GetEnemyOf(owner)))
				{
					atk->crit = true;
				}
				atk->executionAbility.Add(UAdept::StaticClass());
				executionAbility.Add(UAdept::StaticClass());
				protocol->AddProtocolStepAfterStep(this, atk);
			}
		}

		if (hit)
		{
			if (crit)
			{
				protocol->GetEnemyOf(owner)->Damage(damage * 3);
				owner->AddWeaponRankEP(owner->GetEquippedWeapon()->type, 10 * (uint8)owner->GetEquippedWeapon()->rank);
			}
			else
			{
				protocol->GetEnemyOf(owner)->Damage(damage);
				int wpep = 2 * (uint8)owner->GetEquippedWeapon()->rank;
				protocol->AddWeaponEPFor(owner, wpep);
			}

			if (owner->GetEquippedWeapon()->damageType == EDamageType::DT_Physical)
			{
				owner->UseEndurance(owner->GetEquippedWeapon()->cost);
			}
			else if (owner->GetEquippedWeapon()->damageType == EDamageType::DT_Magical)
			{
				owner->UseMana(owner->GetEquippedWeapon()->cost);
			}

			for (int i = 0; i < owner->GetEquippedWeapon()->statusEffects.Num(); i++)
			{
				protocol->GetEnemyOf(owner)->AddStatusEffectPropabilityAfterBattle(owner->GetEquippedWeapon()->statusEffects[i]);
			}

			owner->UseItem(owner->GetEquippedItemInstance());

			

		}
		else
		{
			protocol->GetEnemyOf(owner)->Avoid();
			protocol->AddWeaponEPFor(owner, 1);
		}
	}
}
