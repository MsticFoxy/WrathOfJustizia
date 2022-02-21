// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleInitiator.h"
#include "BattleProtocol/BattleProtocolStep.h"
#include "BattleProtocol/BPS_Attack.h"
#include "AdjustedRandom.h"
#include "Abilities/Canto.h"
#include "Abilities/Advantage.h"
#include "Abilities/Boss.h"
#include "GameSlotData.h"

// Sets default values
ABattleInitiator::ABattleInitiator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABattleInitiator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABattleInitiator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABattleInitiator::InitiateBattle(AUnit * _offender, AUnit * _defender)
{
	

	protocol = NewObject<UBattleProtocol>();

	protocol->offender = _offender;
	protocol->defender = _defender;

	protocol->prevHP_Off = _offender->GetCurrentHP();
	protocol->prevHP_Def = _defender->GetCurrentHP();
	
	protocol->weaponType_Off = protocol->offender->GetEquippedWeapon()->type;
	if (protocol->defender->GetEquippedWeapon() != nullptr)
	{
		protocol->weaponType_Def = protocol->defender->GetEquippedWeapon()->type;
	}

	UNavTile* offT = protocol->offender->currentTile;
	UNavTile* defT = protocol->defender->currentTile;
	if (offT != nullptr && defT != nullptr)
	{
		protocol->range = FMath::Abs(offT->x - defT->x) + FMath::Abs(offT->y - defT->y);
	}
	else
	{
		protocol->range = 0;
	}
	
	// Offender Attack 1
	if (_offender->characterData.currentHP > 0)
	{
		UBPS_Attack* _offender_step1 = NewObject<UBPS_Attack>();
		_offender_step1->SetOwner(_offender);
		_offender_step1->damage = AUnit::GetOffenderDamage(_offender, _defender);
		if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderHit(_offender, _defender))
		{
			_offender_step1->hit = true;
		}
		if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderCrit(_offender, _defender))
		{
			_offender_step1->crit = true;
		}
		protocol->AddProtocolStep(_offender_step1);

		// Dual Attack
		if (_offender->GetEquippedWeapon()->dualAttack)
		{
			UBPS_Attack* _offender_step1_2 = NewObject<UBPS_Attack>();
			_offender_step1_2->SetOwner(_offender);
			_offender_step1_2->damage = AUnit::GetOffenderDamage(_offender, _defender);
			if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderHit(_offender, _defender))
			{
				_offender_step1_2->hit = true;
			}
			if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderCrit(_offender, _defender))
			{
				_offender_step1_2->crit = true;
			}
			protocol->AddProtocolStep(_offender_step1_2);
		}
	}

	// Defender Attack 1
	if (_defender->GetEquippedWeapon() != nullptr)
	{
		bool adv = false;
		if (_defender->GetAbilities().Contains(UAdvantage::StaticClass()))
		{
			if (AdjustedRandom::GetRandomReversed() < _defender->GetSkill())
			{
				adv = true;
			}
		}

		if (_defender->characterData.currentHP > 0)
		{
			UBPS_Attack* _defender_step1 = NewObject<UBPS_Attack>();
			_defender_step1->SetOwner(_defender);
			_defender_step1->damage = AUnit::GetOffenderDamage(_defender, _offender);
			if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderHit(_defender, _offender))
			{
				_defender_step1->hit = true;
			}
			if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderCrit(_defender, _offender))
			{
				_defender_step1->crit = true;
			}
			if (adv)
			{
				_defender_step1->executionAbility.Add(UAdvantage::StaticClass());
				protocol->AddProtocolStepAtID(_defender_step1, 0);
			}
			else
			{
				protocol->AddProtocolStep(_defender_step1);
			}


			// Dual Attack

			if (_defender->GetEquippedWeapon()->dualAttack)
			{
				UBPS_Attack* _defender_step1_2 = NewObject<UBPS_Attack>();
				_defender_step1_2->SetOwner(_defender);
				_defender_step1_2->damage = AUnit::GetOffenderDamage(_defender, _offender);
				if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderHit(_defender, _offender))
				{
					_defender_step1_2->hit = true;
				}
				if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderCrit(_defender, _offender))
				{
					_defender_step1_2->crit = true;
				}
				if (adv)
				{
					protocol->AddProtocolStepAtID(_defender_step1_2, 1);
				}
				else
				{
					protocol->AddProtocolStep(_defender_step1_2);
				}
			}
		}
	}

	if (AUnit::GetOffenderDouble(_offender, _defender))
	{
		// Offender Attack 2
		if (_offender->characterData.currentHP > 0)
		{
			UBPS_Attack* _offender_step2 = NewObject<UBPS_Attack>();
			_offender_step2->SetOwner(_offender);
			_offender_step2->damage = AUnit::GetOffenderDamage(_offender, _defender);
			if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderHit(_offender, _defender))
			{
				_offender_step2->hit = true;
			}
			if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderCrit(_offender, _defender))
			{
				_offender_step2->crit = true;
			}
			protocol->AddProtocolStep(_offender_step2);

			// Dual Attack
			if (_offender->GetEquippedWeapon()->dualAttack)
			{
				UBPS_Attack* _offender_step2_2 = NewObject<UBPS_Attack>();
				_offender_step2_2->SetOwner(_offender);
				_offender_step2_2->damage = AUnit::GetOffenderDamage(_offender, _defender);
				if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderHit(_offender, _defender))
				{
					_offender_step2_2->hit = true;
				}
				if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderCrit(_offender, _defender))
				{
					_offender_step2_2->crit = true;
				}
				protocol->AddProtocolStep(_offender_step2_2);
			}
		}
	}
	else if (AUnit::GetOffenderDouble(_defender, _offender))
	{
		// Defender Attack 2
		if (_defender->GetEquippedWeapon() != nullptr)
		{
			if (_defender->characterData.currentHP > 0)
			{
				UBPS_Attack* _defender_step2 = NewObject<UBPS_Attack>();
				_defender_step2->SetOwner(_defender);
				_defender_step2->damage = AUnit::GetOffenderDamage(_defender, _offender);
				if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderHit(_defender, _offender))
				{
					_defender_step2->hit = true;
				}
				if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderCrit(_defender, _offender))
				{
					_defender_step2->crit = true;
				}
				protocol->AddProtocolStep(_defender_step2);

				// Dual Attack
				if (_defender->GetEquippedWeapon()->dualAttack)
				{
					UBPS_Attack* _defender_step2_2 = NewObject<UBPS_Attack>();
					_defender_step2_2->SetOwner(_defender);
					_defender_step2_2->damage = AUnit::GetOffenderDamage(_defender, _offender);
					if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderHit(_defender, _offender))
					{
						_defender_step2_2->hit = true;
					}
					if (AdjustedRandom::GetRandomReversed() < AUnit::GetOffenderCrit(_defender, _offender))
					{
						_defender_step2_2->crit = true;
					}
					protocol->AddProtocolStep(_defender_step2_2);
				}
			}
		}
	}

}

void ABattleInitiator::ApplyAftermath()
{
	if (protocol->offender->characterData.currentHP <= 0)
	{
		protocol->offender->Die(protocol->defender);
	}
	if (protocol->defender->characterData.currentHP <= 0)
	{
		protocol->defender->Die(protocol->offender);
	}

	for (int i = 0; i < protocol->offender->afterBattleStatusEffects.Num(); i++)
	{
		protocol->offender->AddStatusEffect(protocol->offender->afterBattleStatusEffects[i]);
	}
	protocol->offender->afterBattleStatusEffects.Empty();

	for (int i = 0; i < protocol->defender->afterBattleStatusEffects.Num(); i++)
	{
		protocol->defender->AddStatusEffect(protocol->defender->afterBattleStatusEffects[i]);
	}
	protocol->defender->afterBattleStatusEffects.Empty();

	if (protocol->offender->IsAlive() && protocol->offender->team == EUnitTeam::E_Player)
	{
		
		int ep = GetUnitBattleEP(protocol->defender);
		int addEP = FMath::Abs(protocol->prevHP_Def - protocol->defender->GetCurrentHP());
		if (!protocol->defender->IsAlive())
		{
			ep += GetUnitKillEP(protocol->defender) + addEP;
		}
		if(addEP == 0)
		{
			protocol->offender->AddEP(FMath::CeilToInt(protocol->offender->GetEPToLevel(protocol->offender->GetCurrentLevel()+1) * 0.01f));
		}
		else
		{
			protocol->offender->AddEP(ep + FMath::CeilToInt(addEP * 0.5f));
		}
		protocol->offender->AddWeaponRankEP(protocol->weaponType_Off, protocol->wpEP_Off);
	}
	else if (protocol->defender->IsAlive() && protocol->defender->team == EUnitTeam::E_Player)
	{
		int ep = GetUnitBattleEP(protocol->offender);
		int addEP = FMath::Abs(protocol->prevHP_Off - protocol->offender->GetCurrentHP());
		if (!protocol->offender->IsAlive())
		{
			ep += GetUnitKillEP(protocol->offender) + addEP;
		}
		if (addEP == 0)
		{
			protocol->defender->AddEP(FMath::CeilToInt(protocol->defender->GetEPToLevel(protocol->defender->GetCurrentLevel() + 1) * 0.01f));
		}
		else
		{
			protocol->defender->AddEP(ep + FMath::CeilToInt(addEP * 0.5f));
		}
		protocol->defender->AddWeaponRankEP(protocol->weaponType_Def, protocol->wpEP_Def);
	}

	if (protocol->offender->GetAbilities().Contains(UCanto::StaticClass()) && protocol->offender->IsAlive())
	{
		protocol->offender->state = EUnitState::ECanto;
	}
	else
	{
		protocol->offender->state = EUnitState::EWaiting;
	}
	
}

int ABattleInitiator::GetUnitKillEP(AUnit * _unit)
{
	int mult = 1;
	if (_unit != nullptr)
	{
		if (_unit->characterData.characterClass != nullptr)
		{
			mult = _unit->characterData.characterClass->tier;
		}
	}

	float boss = 1;
	if (_unit->GetAbilities().Contains(UBoss::StaticClass()))
	{
		boss = 5;
	}

	return FMath::RoundToInt((FMath::RoundToInt(0.08f * _unit->GetEPToLevel(_unit->characterData.level + 1)) * boss) * AGameSlotData::GetGameSlotData(this)->difficulty.epMultiplier);
}

int ABattleInitiator::GetUnitBattleEP(AUnit * _unit)
{
	int mult = 1;
	if (_unit != nullptr)
	{
		if (_unit->characterData.characterClass != nullptr)
		{
			mult = _unit->characterData.characterClass->tier;
		}
	}

	float boss = 1;
	if (_unit->GetAbilities().Contains(UBoss::StaticClass()))
	{
		boss = 1.2f;
	}

	return FMath::RoundToInt((FMath::RoundToInt(0.16f * _unit->GetEPToLevel(_unit->characterData.level + 1)) * boss) * AGameSlotData::GetGameSlotData(this)->difficulty.epMultiplier);
}

