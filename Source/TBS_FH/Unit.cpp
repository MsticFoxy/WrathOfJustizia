// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "WeaponData.h"
#include "ActionOptions/WaitOption.h"
#include "ActionOptions/AttackOption.h"
#include "ActionOptions/ItemOption.h"
#include "ActionOptions/OpenTreasure.h"
#include "ActionOptions/Trade.h"
#include "AdjustedRandom.h"
#include "Abilities/Canto.h"
#include "Abilities/Crit_5.h"
#include "Abilities/Crit_10.h"
#include "Abilities/Crit_20.h"
#include "Abilities/Shadow.h"
#include "Abilities/Boss.h"
#include "Abilities/Sprint.h"
#include "StatBonusItemData.h"
#include "StatusEffectItemData.h"
#include "ActionOptions/TalkOption.h"
#include "ActionOptions/AnimaSoulOption.h"
#include "Abilities/Penetration.h"
#include "Abilities/Elite.h"
#include "ActionOptions/HealOption.h"
#include "StaffData.h"
#include "Staffs/HealingStaffData.h"
#include "GameplayTagContainer.h"
#include "ActionOptions/PullLeverOption.h"
#include "ActionOptions/RestOption.h"



// Sets default values
AUnit::AUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	forwardArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Forward"));
	SetRootComponent(forwardArrowComponent);
	unitMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Unit Mesh"));
	//unitMesh->AttachToComponent(forwardArrowComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	unitMesh->SetupAttachment(forwardArrowComponent);
	worldSteadyArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Steady"));
	worldSteadyArrowComponent->SetupAttachment(forwardArrowComponent);
	currentTile = nullptr;
	lastTile = nullptr;
	init = false;
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANavMap::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		if (dynamic_cast<ANavMap*>(FoundActors[0]))
		{
			tileMap = dynamic_cast<ANavMap*>(FoundActors[0]);
		}
	}
	if (tileMap != nullptr)
	{
		state = EUnitState::EReady;

		lookRotation = GetActorRotation();

		if (autoGenerateStats)
		{
			AutoGenerateStatsByLevel();

		}
		characterData.currentHP = GetHP();
		characterData.currentEndurance = GetEndurance();
		characterData.currentMana = GetMana();

		if (characterData.person == nullptr)
		{
			switch (team)
			{
			case EUnitTeam::E_Player:
				if (characterData.characterClass->genericPerson_Override_Player != nullptr)
				{
					characterData.person = characterData.characterClass->genericPerson_Override_Player;
				}
				break;
			case EUnitTeam::E_NPC:
				if (characterData.characterClass->genericPerson_Override_NPC != nullptr)
				{
					characterData.person = characterData.characterClass->genericPerson_Override_NPC;
				}
				break;
			case EUnitTeam::E_Partner:
				if (characterData.characterClass->genericPerson_Override_Partner != nullptr)
				{
					characterData.person = characterData.characterClass->genericPerson_Override_Partner;
				}
				break;
			case EUnitTeam::E_Enemy:
				if (characterData.characterClass->genericPerson_Override_Enemy != nullptr)
				{
					characterData.person = characterData.characterClass->genericPerson_Override_Enemy;
				}
				break;
			case EUnitTeam::E_None:
				if (characterData.characterClass->genericPerson_Override_None != nullptr)
				{
					characterData.person = characterData.characterClass->genericPerson_Override_None;
				}
				break;
			default:

				break;
			}
			if (characterData.person == nullptr)
			{
				if (characterData.characterClass->genericPerson != nullptr)
				{
					characterData.person = characterData.characterClass->genericPerson;
				}
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Unit Class: Navmap Not Found! Unit could be in the wrong Level"));
	}
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (tileMap != nullptr)
	{
		if (!init)
		{
			if (IsAlive())
			{
				SetCurrentTile(tileMap->GetTileAtLocation(GetActorLocation()));
			}
			if (currentTile != nullptr)
			{
				SetActorLocation(tileMap->GetLocationOfTile(currentTile));
			}
			else
			{
				SetActorLocation(FVector(0, 0, -10000));
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("INIT"));
			init = true;
		}

		ApplyUnitStates(DeltaTime);

		SetActorRotation(FMath::Lerp(GetActorRotation(), lookRotation, 0.15f));
		worldSteadyArrowComponent->SetWorldRotation(FRotator(0, 0, 0));
	}
}

void AUnit::NotifyItemAdditionToInventory_Implementation(UItemInstance * _item, bool inventoryFull)
{
}

void AUnit::StartFieldDialogBP_Implementation(UDialog * _dialog)
{
}

void AUnit::SpawnAnima_Implementation(bool getEP)
{
}

FCharacterData AUnit::GetAnimaCharacterData()
{
	FCharacterData ret = FCharacterData();
	ret.characterClass = characterData.anima.characterClass;
	ret.customSkinTier_1 = characterData.anima.customSkinTier_1;
	ret.customSkinTier_2 = characterData.anima.customSkinTier_2;
	ret.customSkinTier_3 = characterData.anima.customSkinTier_3;
	ret.person = characterData.anima.person;
	ret.level = GetCurrentLevel();
	if (characterData.anima.animaResource == EDamageType::DT_Magical)
	{
		ret.stat_HP = FMath::FloorToInt(0.25f* GetMana());
	}
	else if (characterData.anima.animaResource == EDamageType::DT_Physical)
	{
		ret.stat_HP = FMath::FloorToInt(0.25f* GetEndurance());
	}
	
	UItemInstance* wp = NewObject<UItemInstance>();
	wp->item = characterData.anima.boundWeapon;
	wp->bound = true;
	wp->durability = wp->item->durability;
	ret.inventory.Add(wp);

	bool adding = true;
	if (ret.characterClass != nullptr)
	{

		int _str = FMath::Clamp(FMath::RoundToInt(
			(ret.level + 20 * (ret.characterClass->tier - 1) - 1) *
			(float)ret.characterClass->growth_strength / 100.0f), 0,
			ret.characterClass->max_strength - ret.characterClass->base_strength);

		if (adding)
		{
			ret.stat_strength += _str;
		}
		else
		{
			ret.stat_strength = _str;
		}

		int _mag = FMath::Clamp(FMath::RoundToInt(
			(ret.level + 20 * (ret.characterClass->tier - 1) - 1) *
			(float)ret.characterClass->growth_magic / 100.0f), 0,
			ret.characterClass->max_magic - ret.characterClass->base_magic);

		if (adding)
		{
			ret.stat_magic += _mag;
		}
		else
		{
			ret.stat_magic = _mag;
		}

		int _end = FMath::Clamp(FMath::RoundToInt(
			(ret.level + 20 * (ret.characterClass->tier - 1) - 1) *
			(float)ret.characterClass->growth_endurance / 100.0f), 0,
			ret.characterClass->max_endurance - ret.characterClass->base_endurance);

		if (adding)
		{
			ret.stat_endurance += _end;
		}
		else
		{
			ret.stat_endurance = _end;
		}

		int _man = FMath::Clamp(FMath::RoundToInt(
			(ret.level + 20 * (ret.characterClass->tier - 1) - 1) *
			(float)ret.characterClass->growth_mana / 100.0f), 0,
			ret.characterClass->max_mana - ret.characterClass->base_mana);

		if (adding)
		{
			ret.stat_mana += _man;
		}
		else
		{
			ret.stat_mana = _man;
		}

		int _def = FMath::Clamp(FMath::RoundToInt(
			(ret.level + 20 * (ret.characterClass->tier - 1) - 1) *
			(float)ret.characterClass->growth_defence / 100.0f), 0,
			ret.characterClass->max_defence - ret.characterClass->base_defence);

		if (adding)
		{
			ret.stat_defence += _def;
		}
		else
		{
			ret.stat_defence = _def;
		}

		int _res = FMath::Clamp(FMath::RoundToInt(
			(ret.level + 20 * (ret.characterClass->tier - 1) - 1) *
			(float)ret.characterClass->growth_resistence / 100.0f), 0,
			ret.characterClass->max_resistence - ret.characterClass->base_resistence);

		if (adding)
		{
			ret.stat_resistence += _res;
		}
		else
		{
			ret.stat_resistence = _res;
		}

		int _skl = FMath::Clamp(FMath::RoundToInt(
			(ret.level + 20 * (ret.characterClass->tier - 1) - 1) *
			(float)ret.characterClass->growth_skill / 100.0f), 0,
			ret.characterClass->max_skill - ret.characterClass->base_skill);

		if (adding)
		{
			ret.stat_skill += _skl;
		}
		else
		{
			ret.stat_skill = _skl;
		}

		int _spd = FMath::Clamp(FMath::RoundToInt(
			(ret.level + 20 * (ret.characterClass->tier - 1) - 1) *
			(float)ret.characterClass->growth_speed / 100.0f), 0,
			ret.characterClass->max_speed - ret.characterClass->base_speed);

		if (adding)
		{
			ret.stat_speed += _spd;
		}
		else
		{
			ret.stat_speed = _spd;
		}

		int _lck = FMath::Clamp(FMath::RoundToInt(
			(ret.level + 20 * (ret.characterClass->tier - 1) - 1) *
			(float)ret.characterClass->growth_luck / 100.0f), 0,
			ret.characterClass->max_luck - ret.characterClass->base_luck);

		if (adding)
		{
			ret.stat_luck += _lck;
		}
		else
		{
			ret.stat_luck = _lck;
		}

		struct ILocal
		{
			static EWeaponRank Rank(EWeaponRank _max, int level)
			{
				float per = (float)level / 20;
				int8 rnk = FMath::CeilToInt((float)((int8)_max) * per);
				if (rnk <= 0)
				{
					rnk = 1;
				}
				return (EWeaponRank)rnk;
			}
		};


		if (ret.characterClass->rank_sword > EWeaponRank::None)
		{
			if (ret.current_rank_sword == EWeaponRank::None)
			{
				ret.current_rank_sword = ILocal::Rank(ret.characterClass->rank_sword, GetCurrentLevel());
			}
		}

		if (ret.characterClass->rank_lance > EWeaponRank::None)
		{
			if (ret.current_rank_lance == EWeaponRank::None)
			{
				ret.current_rank_lance = ILocal::Rank(ret.characterClass->rank_lance, GetCurrentLevel());
			}
		}

		if (ret.characterClass->rank_axe > EWeaponRank::None)
		{
			if (ret.current_rank_axe == EWeaponRank::None)
			{
				ret.current_rank_axe = ILocal::Rank(ret.characterClass->rank_axe, GetCurrentLevel());
			}
		}

		if (ret.characterClass->rank_bow > EWeaponRank::None)
		{
			if (ret.current_rank_bow == EWeaponRank::None)
			{
				ret.current_rank_bow = ILocal::Rank(ret.characterClass->rank_bow, GetCurrentLevel());
			}
		}

		if (ret.characterClass->rank_dagger > EWeaponRank::None)
		{
			if (ret.current_rank_dagger == EWeaponRank::None)
			{
				ret.current_rank_dagger = ILocal::Rank(ret.characterClass->rank_dagger, GetCurrentLevel());
			}
		}

		if (ret.characterClass->rank_ignis > EWeaponRank::None)
		{
			if (ret.current_rank_ignis == EWeaponRank::None)
			{
				ret.current_rank_ignis = ILocal::Rank(ret.characterClass->rank_ignis, GetCurrentLevel());
			}
		}

		if (ret.characterClass->rank_terra > EWeaponRank::None)
		{
			if (ret.current_rank_terra == EWeaponRank::None)
			{
				ret.current_rank_terra = ILocal::Rank(ret.characterClass->rank_terra, GetCurrentLevel());
			}
		}

		if (ret.characterClass->rank_aer > EWeaponRank::None)
		{
			if (ret.current_rank_aer == EWeaponRank::None)
			{
				ret.current_rank_aer = ILocal::Rank(ret.characterClass->rank_aer, GetCurrentLevel());
			}
		}

		if (ret.characterClass->rank_aqua > EWeaponRank::None)
		{
			if (ret.current_rank_aqua == EWeaponRank::None)
			{
				ret.current_rank_aqua = ILocal::Rank(ret.characterClass->rank_aqua, GetCurrentLevel());
			}
		}

		if (ret.characterClass->rank_lux > EWeaponRank::None)
		{
			if (ret.current_rank_lux == EWeaponRank::None)
			{
				ret.current_rank_lux = ILocal::Rank(ret.characterClass->rank_lux, GetCurrentLevel());
			}
		}

		if (ret.characterClass->rank_umbra > EWeaponRank::None)
		{
			if (ret.current_rank_umbra == EWeaponRank::None)
			{
				ret.current_rank_umbra = ILocal::Rank(ret.characterClass->rank_umbra, GetCurrentLevel());
			}
		}

		if (ret.characterClass->rank_staff > EWeaponRank::None)
		{
			if (ret.current_rank_staff == EWeaponRank::None)
			{
				ret.current_rank_staff = ILocal::Rank(ret.characterClass->rank_staff, GetCurrentLevel());
			}
		}

		if (ret.characterClass->rank_slash > EWeaponRank::None)
		{
			if (ret.current_rank_slash == EWeaponRank::None)
			{
				ret.current_rank_slash = ILocal::Rank(ret.characterClass->rank_slash, GetCurrentLevel());
			}
		}
	}
	return ret;
}

void AUnit::StartFieldDialog(UDialog * _dialog, int removeID)
{
	fieldDialogs.RemoveAt(removeID);
	StartFieldDialogBP(_dialog);
}

void AUnit::initiateCharacterSet()
{
	if (characterSet != nullptr)
	{
		characterData = characterSet->characterSet;
	}
}

bool AUnit::IsStartInventoryInitiated()
{
	return characterData.inventoryInitiated;
}

void AUnit::SetStartInventoryInitiated(bool val)
{
	characterData.inventoryInitiated = true;
}

void AUnit::WeightMapFromHere()
{
	if (currentTile != nullptr)
	{
		tileMap->WeightMap(currentTile, GetTraverseType(), team, this);
		tileMap->UpdateMovementMeshVertexColor(GetMovement(), GetAttackRange(), GetStaffRange());
	}
}

ANavMap * AUnit::GetMap()
{
	return tileMap;
}

void AUnit::WeightMapFromHereWithRange(int _range)
{
	if (currentTile != nullptr)
	{
		tileMap->WeightMap(currentTile, GetTraverseType(), team, this);
		tileMap->UpdateMovementMeshVertexColor(_range, GetAttackRange(), GetStaffRange());
	}
}

void AUnit::WeightMapFromHereWithRangeAndSpecificWeapon(int _range, UWeaponData* _weapon)
{
	if (currentTile != nullptr)
	{
		tileMap->WeightMap(currentTile, GetTraverseType(), team, this);
		TArray<FIntervalRange> ranges;
		TArray<FIntervalRange> hl_ranges;
		if (_weapon != nullptr)
		{
			ranges.Add(_weapon->range);

			if (dynamic_cast<UStaffData*>(_weapon))
			{
				hl_ranges.Add(dynamic_cast<UStaffData*>(_weapon)->staffRange);
			}
		}

		tileMap->UpdateMovementMeshVertexColor(_range, ranges, hl_ranges);
	}
}

void AUnit::SetCurrentTile(UNavTile * _tile)
{
	if (_tile != nullptr && _tile->unit == nullptr)
	{
		if (currentTile != nullptr && currentTile->unit == this)
		{
			currentTile->unit = nullptr;
		}
		currentTile = _tile;
		_tile->unit = this;
	}
	else if (_tile == nullptr)
	{
		if (currentTile != nullptr && currentTile->unit == this)
		{
			currentTile->unit = nullptr;
		}
		_tile = nullptr;
	}
}

void AUnit::ForceCurrentTile(UNavTile * _tile)
{
	if (_tile != nullptr)
	{
		if (currentTile != nullptr && currentTile->unit == this)
		{
			currentTile->unit = nullptr;
		}
		currentTile = _tile;
		_tile->unit = this;
	}
	else if (_tile == nullptr)
	{
		if (currentTile != nullptr && currentTile->unit == this)
		{
			currentTile->unit = nullptr;
		}
		_tile = nullptr;
	}
}

bool AUnit::MoveToTile(UNavTile * _tile)
{
	bool unitBool = false;
	if (_tile != nullptr)
	{
		if (_tile->unit == nullptr || (_tile->unit != nullptr && _tile->unit == this))
		{
			unitBool = true;
		}
	}

	if (_tile != nullptr && unitBool && _tile->currentWeight <= GetMovement() && _tile->tileData != nullptr && _tile->tileData->restable)
	{
		lastTile = currentTile;
		SetCurrentTile(_tile);

		TArray<UNavTile*> tls;
		tls.Add(lastTile);
		tls.Add(currentTile);
		tileMap->NotifyEnemyRangeTiles(tls);

		//SetActorLocation(tileMap->GetLocationOfTile(currentTile));
		if (state != EUnitState::ECanto)
		{
			state = EUnitState::EMoving;
		}
		else
		{
			state = EUnitState::ECantoMove;
		}
		moveStep = 0;
		tileMap->WeightMap(lastTile, GetTraverseType(), team, this);
		if (moveTilePath.Num() <= 0)
		{
			moveTilePath = tileMap->GetPath(lastTile, currentTile);
		}
		moveVectorPath = tileMap->TilePathToVectorPathSmoothed(moveTilePath);

		if (state == EUnitState::EMoving && GetAbilities().Contains(UCanto::StaticClass()))
		{
			alreadyMoved = tileMap->GetCostOfPath(moveTilePath, this);
		}

		moveTilePath.Empty();
		tileMap->ClearMovementMesh();
		tileMap->UpdateEnemyRangeMeshVertexColor();
		
		return true;
	}
	return false;
}

void AUnit::ResetToLastTile()
{
	if (lastTile != nullptr)
	{
		TArray<UNavTile*> noti;
		noti.Add(currentTile);
		noti.Add(lastTile);

		SetCurrentTile(lastTile);
		lastTile = nullptr;
		SetActorLocation(tileMap->GetLocationOfTile(currentTile));

		tileMap->NotifyEnemyRangeTiles(noti);
		tileMap->UpdateEnemyRangeMeshVertexColor();
	}
}

void AUnit::VisualMoving(float deltaTime)
{
	if (moveStep < moveVectorPath.Num()-1)
	{
		FVector n = moveVectorPath[FMath::Clamp(FMath::FloorToInt(moveStep) + 1,0,moveVectorPath.Num()-1)];
		FVector c = moveVectorPath[FMath::FloorToInt(moveStep)];
		float lerp = (moveStep) - FMath::FloorToInt(moveStep);
		FVector loc = FMath::Lerp(c, n, lerp);
		SetActorLocation(loc);
		LookTo(n - c);

		moveStep += deltaTime * (((float)GetSpeed() + 60.0f) / 15.0f) * 2;
	}
	else
	{
		SetActorLocation(moveVectorPath[moveVectorPath.Num()-1]);
		WeightMapFromHereWithRange(0);
		TArray<UNavTile*> noti;
		noti.Add(currentTile);
		noti.Add(lastTile);
		tileMap->NotifyEnemyRangeTiles(noti);
		tileMap->UpdateEnemyRangeMeshVertexColor();
		if (state == EUnitState::ECantoMove)
		{
			state = EUnitState::ECantoEnd;
		}
		else
		{
			state = EUnitState::EActing;
		}
	}
}

void AUnit::ApplyUnitStates(float deltaTime)
{
	if (state == EUnitState::EMoving)
	{
		VisualMoving(deltaTime);
	}
	else if (state == EUnitState::ECantoMove)
	{
		VisualMoving(deltaTime);
	}
}

void AUnit::StatusEffectsOnStartTurnTick_Implementation()
{
	
}

void AUnit::StatusEffectsOnEndTurnTick_Implementation()
{
}

void AUnit::AddStatusEffect(UStatusEffectData * _data)
{
	if (_data != nullptr)
	{
		for (int i = 0; i < statusEffects.Num(); i++)
		{
			if (statusEffects[i]->statusEffect == _data)
			{
				statusEffects[i]->remaining = _data->duration;
				return;
			}
		}
		UStatusEffectInstance* ef = NewObject<UStatusEffectInstance>();
		ef->statusEffect = _data;
		ef->remaining = _data->duration;
		statusEffects.Add(ef);
		if (ef->statusEffect->onDuration_movement != 0)
		{
			TArray<UNavTile*> tls;
			tls.Add(currentTile);
			tileMap->NotifyEnemyRangeTiles(tls);
			tileMap->UpdateEnemyRangeMeshVertexColor();
		}
	}
}

void AUnit::AddStatusEffectAfterBattle(UStatusEffectData * _data)
{
	if (_data != nullptr)
	{
		afterBattleStatusEffects.Add(_data);
	}
}

void AUnit::AddStatusEffectPropability(FStatusEffectPropapility _prop)
{
	if (AdjustedRandom::GetRandomReversed() < _prop.percent)
	{
		if (_prop.effect != nullptr)
		{
			UStatusEffectData* _data = _prop.effect;
			for (int i = 0; i < statusEffects.Num(); i++)
			{
				if (statusEffects[i]->statusEffect == _data)
				{
					statusEffects[i]->remaining = _data->duration;
					return;
				}
			}
			UStatusEffectInstance* ef = NewObject<UStatusEffectInstance>();
			ef->statusEffect = _data;
			ef->remaining = _data->duration;
			statusEffects.Add(ef);

			if (ef->statusEffect->onDuration_movement != 0)
			{
				TArray<UNavTile*> tls;
				tls.Add(currentTile);
				tileMap->NotifyEnemyRangeTiles(tls);
				tileMap->UpdateEnemyRangeMeshVertexColor();
			}
		}
	}
}

void AUnit::AddStatusEffectPropabilityAfterBattle(FStatusEffectPropapility _prop)
{
	if (AdjustedRandom::GetRandomReversed() < _prop.percent)
	{
		if (_prop.effect != nullptr)
		{
			afterBattleStatusEffects.Add(_prop.effect);
		}
	}
}

void AUnit::RemoveStatusEffect(UStatusEffectData * _data)
{
	if (_data != nullptr)
	{
		int id = -1;
		for (int i = 0; i < statusEffects.Num(); i++)
		{
			if (statusEffects[i]->statusEffect == _data)
			{
				id = i;
				break;
			}
		}

		bool notify = false;
		if (statusEffects[id]->statusEffect->onDuration_movement != 0)
		{
			notify = true;
		}

		statusEffects.RemoveAt(id);

		if (notify)
		{
			TArray<UNavTile*> tls;
			tls.Add(currentTile);
			tileMap->NotifyEnemyRangeTiles(tls);
			tileMap->UpdateEnemyRangeMeshVertexColor();
		}
	}
}

void AUnit::LookTo(FVector _direction)
{
	FRotator rot = UKismetMathLibrary::FindLookAtRotation(FVector(0, 0, 0), FVector(_direction.X, _direction.Y, 0));
	lookRotation = rot;
}

int AUnit::GetCurrentEP()
{
	if (characterData.characterClass == nullptr)
	{
		return 0;
	}
	return characterData.ep;
}

int AUnit::GetCurrentLevel()
{
	if (characterData.characterClass == nullptr)
	{
		return 0;
	}
	return characterData.level;
}

int AUnit::GetTotalStatValue()
{
	return characterData.GetTotalStatValue();
}

int AUnit::GetHP()
{
	return characterData.GetHP();
}

int AUnit::GetStrength()
{
	int effects = 0;
	for (int i = 0; i < statusEffects.Num(); i++)
	{
		UStatusEffectInstance* se = statusEffects[i];
		effects += se->statusEffect->onDuration_strength;
	}
	return characterData.GetStrength() + effects;
}

int AUnit::GetMagic()
{
	int effects = 0;
	for (int i = 0; i < statusEffects.Num(); i++)
	{
		UStatusEffectInstance* se = statusEffects[i];
		effects += se->statusEffect->onDuration_magic;
	}
	return characterData.GetMagic() + effects;
}

int AUnit::GetEndurance()
{
	return characterData.GetEndurance();
}

int AUnit::GetMana()
{
	return characterData.GetMana();
}

int AUnit::GetDefence()
{
	int effects = 0;
	for (int i = 0; i < statusEffects.Num(); i++)
	{
		UStatusEffectInstance* se = statusEffects[i];
		effects += se->statusEffect->onDuration_defense;
	}
	return characterData.GetDefence() + effects;
}

int AUnit::GetResistence()
{
	int effects = 0;
	for (int i = 0; i < statusEffects.Num(); i++)
	{
		UStatusEffectInstance* se = statusEffects[i];
		effects += se->statusEffect->onDuration_resistence;
	}
	return characterData.GetResistence() + effects;
}

int AUnit::GetSkill()
{
	int effects = 0;
	for (int i = 0; i < statusEffects.Num(); i++)
	{
		UStatusEffectInstance* se = statusEffects[i];
		effects += se->statusEffect->onDuration_skill;
	}
	return characterData.GetSkill() + effects;
}

int AUnit::GetLuck()
{
	int effects = 0;
	for (int i = 0; i < statusEffects.Num(); i++)
	{
		UStatusEffectInstance* se = statusEffects[i];
		effects += se->statusEffect->onDuration_luck;
	}
	return characterData.GetLuck() + effects;
}

int AUnit::GetSpeed()
{
	int effects = 0;
	for (int i = 0; i < statusEffects.Num(); i++)
	{
		UStatusEffectInstance* se = statusEffects[i];
		effects += se->statusEffect->onDuration_speed;
	}
	return characterData.GetSpeed() + effects;
}

int AUnit::GetMovement()
{
	int effects = 0;
	for (int i = 0; i < statusEffects.Num(); i++)
	{
		UStatusEffectInstance* se = statusEffects[i];
		effects += se->statusEffect->onDuration_movement;
	}
	if (characterData.characterClass == nullptr)
	{
		return 0;
	}
	if (!canMove)
	{
		return 0;
	}

	int add = 0;
	if (GetAbilities().Contains(USprint::StaticClass()))
	{
		if (GetCurrentEndurance() >= GetEndurance())
		{
			add += 2;
		}
	}

	int mv = characterData.characterClass->movement + add + effects - alreadyMoved;
	if (mv < 0)
	{
		mv = 0;
	}
	return mv;
}

int32 AUnit::GetTraverseType()
{
	if (characterData.characterClass == nullptr)
	{
		return 0;
	}
	return characterData.characterClass->traverseType;
}

int AUnit::GetCurrentHP()
{
	return characterData.currentHP;
}

int AUnit::GetCurrentEndurance()
{
	return characterData.currentEndurance;
}

int AUnit::GetCurrentMana()
{
	return characterData.currentMana;
}

int AUnit::GetTotalGrowthValue()
{
	return characterData.GetTotalGrowthValue();
}

int AUnit::GetGrowthHP()
{
	return characterData.GetGrowthHP();
}

int AUnit::GetGrowthStrength()
{
	return characterData.GetGrowthStrength();
}

int AUnit::GetGrowthMagic()
{
	return characterData.GetGrowthMagic();
}

int AUnit::GetGrowthEndurance()
{
	return characterData.GetGrowthEndurance();
}

int AUnit::GetGrowthMana()
{
	return characterData.GetGrowthMana();
}

int AUnit::GetGrowthDefence()
{
	return characterData.GetGrowthDefence();
}

int AUnit::GetGrowthResistence()
{
	return characterData.GetGrowthResistence();
}

int AUnit::GetGrowthSkill()
{
	return characterData.GetGrowthSkill();
}

int AUnit::GetGrowthLuck()
{
	return characterData.GetGrowthLuck();
}

int AUnit::GetGrowthSpeed()
{
	return characterData.GetGrowthSpeed();
}

int AUnit::GetRootStrength()
{
	if (characterData.characterClass == nullptr)
	{
		return 0;
	}
	return characterData.characterClass->base_strength + characterData.stat_strength;
}

int AUnit::GetRootMagic()
{
	if (characterData.characterClass == nullptr)
	{
		return 0;
	}
	return characterData.characterClass->base_magic + characterData.stat_magic;
}

int AUnit::GetRootDefence()
{
	if (characterData.characterClass == nullptr)
	{
		return 0;
	}
	return characterData.characterClass->base_defence + characterData.stat_defence;
}

int AUnit::GetRootResistence()
{
	if (characterData.characterClass == nullptr)
	{
		return 0;
	}
	return characterData.characterClass->base_resistence + characterData.stat_resistence;
}

int AUnit::GetRootSkill()
{
	if (characterData.characterClass == nullptr)
	{
		return 0;
	}
	return characterData.characterClass->base_skill + characterData.stat_skill;
}

int AUnit::GetRootLuck()
{
	if (characterData.characterClass == nullptr)
	{
		return 0;
	}
	return characterData.characterClass->base_luck + characterData.stat_luck;
}

int AUnit::GetRootSpeed()
{
	if (characterData.characterClass == nullptr)
	{
		return 0;
	}
	return characterData.characterClass->base_speed + characterData.stat_speed;
}

UWeaponData * AUnit::GetEquippedWeapon()
{
	if (characterData.inventory.Num() > 0)
	{
		if (dynamic_cast<UWeaponData*>(characterData.inventory[0]->item))
		{
			return dynamic_cast<UWeaponData*>(characterData.inventory[0]->item);
		}
	}
	return nullptr;
}

void AUnit::EquippWeapon(UItemInstance * _item)
{
	if (_item != nullptr && _item->item != nullptr)
	{
		if (dynamic_cast<UWeaponData*>(_item->item))
		{
			if (CanUseWeapon(dynamic_cast<UWeaponData*>(_item->item)))
			{
				if (characterData.inventory.Contains(_item))
				{

					characterData.inventory.Remove(_item);
					characterData.inventory.Insert(_item, 0);
				}
			}
		}
	}
}

UItemInstance * AUnit::GetEquippedItemInstance()
{
	if (characterData.inventory.Num() > 0)
	{
		return characterData.inventory[0];
	}
	return nullptr;
}

TArray<UItemInstance*> AUnit::GetStaffs()
{
	TArray<UItemInstance*> staffs = TArray<UItemInstance*>();

	for (int i = 0; i < characterData.inventory.Num(); i++)
	{
		UItemData* dt = characterData.inventory[i]->item;
		if (dynamic_cast<UStaffData*>(dt))
		{
			staffs.Add(characterData.inventory[i]);
		}
	}
	return staffs;
}

TArray<UItemInstance*> AUnit::GetStaffsWithUnitsInRage()
{
	TArray<UItemInstance*> staffs = TArray<UItemInstance*>();

	for (int i = 0; i < characterData.inventory.Num(); i++)
	{
		UItemData* dt = characterData.inventory[i]->item;
		if (dynamic_cast<UStaffData*>(dt))
		{
			if (CanUseWeapon(dynamic_cast<UStaffData*>(dt)))
			{
				if (tileMap->GetUnitsInSpecificStaffRange(this, dynamic_cast<UStaffData*>(dt)).Num() > 0)
				{

					staffs.Add(characterData.inventory[i]);
				}
			}
		}
	}
	return staffs;
}

int AUnit::GetPhysicalAttack()
{
	if (GetEquippedWeapon() != nullptr)
	{
		int add = ((uint8)GetWeaponRank(GetEquippedWeapon()->type) - (uint8)GetEquippedWeapon()->rank);
		if (GetAbilities().Contains(UPenetration::StaticClass()))
		{
			if (GetEquippedWeapon()->type == EWeaponType::Bow)
			{
				add += 5;
			}
		}
		return GetStrength() + GetEquippedWeapon()->strength + add;
	}
	return GetStrength();
}

int AUnit::GetMagicalAttack()
{
	if (GetEquippedWeapon() != nullptr)
	{
		int add = ((uint8)GetWeaponRank(GetEquippedWeapon()->type) - (uint8)GetEquippedWeapon()->rank);
		return GetMagic() + GetEquippedWeapon()->strength + add;
	}
	return GetStrength();
}

int AUnit::GetPhysicalDefence()
{
	int terr = 0;
	if (currentTile != nullptr)
	{
		terr = currentTile->tileData->defence;
	}

	int boss = 0;
	if (GetAbilities().Contains(UBoss::StaticClass()))
	{
		boss += 5;
	}

	return GetDefence() + terr + boss;
}

int AUnit::GetMagicalDefence()
{
	int terr = 0;
	if (currentTile != nullptr)
	{
		terr = currentTile->tileData->resistence;
	}

	int boss = 0;
	if (GetAbilities().Contains(UBoss::StaticClass()))
	{
		boss += 5;
	}

	return GetResistence() + terr + boss;
}

int AUnit::GetHitChance()
{
	if (GetEquippedWeapon() != nullptr)
	{
		return GetLuck() + GetSkill() + GetEquippedWeapon()->hit;
	}
	return GetLuck() + GetSkill() + 50;
}

int AUnit::GetAvoidChance()
{
	int terr = 0;
	if (currentTile != nullptr)
	{
		terr = currentTile->tileData->avoid;
	}
	if (GetEquippedWeapon() != nullptr)
	{
		return GetSpeed() + GetLuck() - GetEquippedWeapon()->weight + terr;
	}
	return GetSpeed() + GetLuck() + terr;
}

int AUnit::GetCritChance()
{
	if (GetEquippedWeapon() != nullptr)
	{
		int critAdd = 0;
		if (GetAbilities().Contains(UCrit_5::StaticClass()))
		{
			critAdd += 5;
		}
		if (GetAbilities().Contains(UCrit_10::StaticClass()))
		{
			critAdd += 10;
		}
		if (GetAbilities().Contains(UCrit_20::StaticClass()))
		{
			critAdd += 20;
		}

		return GetSkill() + FMath::FloorToInt((float)GetLuck() * 0.5f) + GetEquippedWeapon()->crit + critAdd;
	}
	return 0;
}

int AUnit::GetCritDefence()
{

	int boss = 0;
	if (GetAbilities().Contains(UBoss::StaticClass()))
	{
		boss += 5;
	}

	return GetLuck() + FMath::FloorToInt((float)GetSkill() * 0.5f) + boss;
}

int AUnit::GetDoubleCompare()
{
	if (GetEquippedWeapon() != nullptr)
	{
		int sub = GetEquippedWeapon()->weight - ((uint8)GetWeaponRank(GetEquippedWeapon()->type) - (uint8)GetEquippedWeapon()->rank);
		return GetSpeed() - sub;
	}
	return GetSpeed();
}

bool AUnit::CanUseHealing(UHealingItemData * _healing)
{
	if (_healing != nullptr)
	{
		if (GetCurrentHP() < GetHP())
		{
			return true;
		}
	}
	return false;
}

bool AUnit::CanUseWeapon(UWeaponData * _weapon)
{
	if (_weapon->type == EWeaponType::Sword)
	{
		if (_weapon->rank <= characterData.current_rank_sword)
		{
			return true;
		}
	}
	else if (_weapon->type == EWeaponType::Lance)
	{
		if (_weapon->rank <= characterData.current_rank_lance)
		{
			return true;
		}
	}
	else if (_weapon->type == EWeaponType::Axe)
	{
		if (_weapon->rank <= characterData.current_rank_axe)
		{
			return true;
		}
	}
	else if (_weapon->type == EWeaponType::Bow)
	{
		if (_weapon->rank <= characterData.current_rank_bow)
		{
			return true;
		}
	}
	else if (_weapon->type == EWeaponType::Dagger)
	{
		if (_weapon->rank <= characterData.current_rank_dagger)
		{
			return true;
		}
	}
	else if (_weapon->type == EWeaponType::Ignis)
	{
		if (_weapon->rank <= characterData.current_rank_ignis)
		{
			return true;
		}
	}
	else if (_weapon->type == EWeaponType::Terra)
	{
		if (_weapon->rank <= characterData.current_rank_terra)
		{
			return true;
		}
	}
	else if (_weapon->type == EWeaponType::Aer)
	{
		if (_weapon->rank <= characterData.current_rank_aer)
		{
			return true;
		}
	}
	else if (_weapon->type == EWeaponType::Aqua)
	{
		if (_weapon->rank <= characterData.current_rank_aqua)
		{
			return true;
		}
	}
	else if (_weapon->type == EWeaponType::Lux)
	{
		if (_weapon->rank <= characterData.current_rank_lux)
		{
			return true;
		}
	}
	else if (_weapon->type == EWeaponType::Umbra)
	{
		if (_weapon->rank <= characterData.current_rank_umbra)
		{
			return true;
		}
	}
	else if (_weapon->type == EWeaponType::Staff)
	{
		if (_weapon->rank <= characterData.current_rank_staff)
		{
			return true;
		}
	}
	else if (_weapon->type == EWeaponType::Slash)
	{
		if (_weapon->rank <= characterData.current_rank_slash)
		{
			return true;
		}
	}
	return false;
}

bool AUnit::CanUseResourceItem(UResourceItemData * _res)
{
	if (_res->type == EResourceType::Endurance)
	{
		if (GetCurrentEndurance() < GetEndurance())
		{
			return true;
		}
	}
	else if (_res->type == EResourceType::Mana)
	{
		if (GetCurrentMana() < GetMana())
		{
			return true;
		}
	}
	return false;
}

void AUnit::UseStaff(UItemInstance * _staff, AUnit* _reciever)
{
	if (dynamic_cast<UStaffData*>(_staff->item))
	{
		_reciever->SpawnEffectAtUnit(dynamic_cast<UStaffData*>(_staff->item)->staffEffect);
		if (dynamic_cast<UHealingStaffData*>(_staff->item))
		{
			UHealingStaffData* h_stf = dynamic_cast<UHealingStaffData*>(_staff->item);
			_reciever->StartResourceChangeWindow();
			_reciever->Heal(h_stf->healAmount + GetMagic());
			UseItem(_staff);
			if (h_stf->damageType == EDamageType::DT_Magical)
			{
				UseMana(h_stf->cost);
			}
			else if (h_stf->damageType == EDamageType::DT_Physical)
			{
				UseEndurance(h_stf->cost);
			}
			int _ep = (uint8)h_stf->rank * 45;
			AddEP(_ep);
		}


		if (GetAbilities().Contains(UCanto::StaticClass()))
		{
			state = EUnitState::ECanto;
		}
		else
		{
			state = EUnitState::EWaiting;
		}
	}
}

TArray<FIntervalRange> AUnit::GetAttackRange()
{
	TArray<FIntervalRange> intervals;

	for (int i = 0; i < characterData.inventory.Num(); i++)
	{
		if (ItemIsUsable(characterData.inventory[i]))
		{
			if (dynamic_cast<UWeaponData*>(characterData.inventory[i]->item))
			{
				if (CanUseWeapon(dynamic_cast<UWeaponData*>(characterData.inventory[i]->item)))
				{
					intervals.Add(dynamic_cast<UWeaponData*>(characterData.inventory[i]->item)->range);
				}
			}
		}
	}

	return intervals;
}

TArray<FIntervalRange> AUnit::GetStaffRange()
{
	TArray<FIntervalRange> intervals;

	for (int i = 0; i < characterData.inventory.Num(); i++)
	{
		if (ItemIsUsable(characterData.inventory[i]))
		{
			if (dynamic_cast<UStaffData*>(characterData.inventory[i]->item))
			{
				if (CanUseWeapon(dynamic_cast<UStaffData*>(characterData.inventory[i]->item)))
				{
					intervals.Add(dynamic_cast<UStaffData*>(characterData.inventory[i]->item)->staffRange);
				}
			}
		}
	}

	return intervals;
}

bool AUnit::BlocksUnitOfTeam(EUnitTeam _team, AUnit* _other)
{
	if (_other != nullptr)
	{
		if (_other->GetAbilities().Contains(UShadow::StaticClass()))
		{
			if (!blocksShadow)
			{
				return false;
			}
		}
	}

	switch (team)
	{
	case EUnitTeam::E_Player:
		if (tileMap->player_Blocks.Contains(_team))
		{
			return true;
		}
		break;
	case EUnitTeam::E_NPC:
		if (tileMap->npc_Blocks.Contains(_team))
		{
			return true;
		}
		break;
	case EUnitTeam::E_Partner:
		if (tileMap->partner_Blocks.Contains(_team))
		{
			return true;
		}
		break;
	case EUnitTeam::E_Enemy:
		if (tileMap->enemy_Blocks.Contains(_team))
		{
			return true;
		}
		break;
	case EUnitTeam::E_None:
		if (tileMap->none_Blocks.Contains(_team))
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

bool AUnit::CanAttackUnitOfTeam(EUnitTeam _team)
{
	switch (team)
	{
	case EUnitTeam::E_Player:
		if (tileMap->player_CanAttack.Contains(_team))
		{
			return true;
		}
		break;
	case EUnitTeam::E_NPC:
		if (tileMap->npc_CanAttack.Contains(_team))
		{
			return true;
		}
		break;
	case EUnitTeam::E_Partner:
		if (tileMap->partner_CanAttack.Contains(_team))
		{
			return true;
		}
		break;
	case EUnitTeam::E_Enemy:
		if (tileMap->enemy_CanAttack.Contains(_team))
		{
			return true;
		}
		break;
	case EUnitTeam::E_None:
		if (tileMap->none_CanAttack.Contains(_team))
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

bool AUnit::CanReachWithEquippedWeapon(int _range)
{
	if (GetEquippedWeapon() != nullptr && GetEquippedWeapon()->range.Min <= _range && GetEquippedWeapon()->range.Max >= _range)
	{
		return true;
	}
	return false;
}

bool AUnit::AddItem(UItemInstance * _item)
{
	if (_item != nullptr)
	{
		if (characterData.inventory.Num() < 8)
		{
			characterData.inventory.Add(_item);
			return true;
		}
	}
	return false;
}

void AUnit::AddAbility(TSubclassOf<UAbility> _abil)
{
	if (GetAbilities().Num() < 8)
	{
		if (!GetAbilities().Contains(_abil))
		{
			characterData.characterAbilitys.Add(_abil);
		}
	}
}

void AUnit::RemoveAbility(TSubclassOf<UAbility> _abil)
{
	if (characterData.characterAbilitys.Contains(_abil))
	{
		characterData.characterAbilitys.Remove(_abil);
	}
}

bool AUnit::AddItemWithNotification(UItemInstance * _item)
{
	if (_item != nullptr)
	{
		if (dynamic_cast<AUnit*>(spawnedBy))
		{
			AUnit* sp = dynamic_cast<AUnit*>(spawnedBy);
			return sp->AddItemWithNotification(_item);
		}
		else
		{
			if (characterData.inventory.Num() < 8)
			{
				characterData.inventory.Add(_item);
				NotifyItemAdditionToInventory(_item, false);
				return true;
			}
			else
			{
				NotifyItemAdditionToInventory(_item, true);
			}
		}
	}
	return false;
}

int AUnit::GetEPToLevel(int _level)
{
	int ret = 100;

	if (characterData.characterClass != nullptr)
	{
		ret = 100 + (_level + (20 * (characterData.characterClass->tier - 1))) * 20;
	}

	return ret;
}

void AUnit::AddEP(int _ep)
{
	if (characterData.characterClass != nullptr)
	{
		if (GetAbilities().Contains(UElite::StaticClass()))
		{
			_ep = FMath::CeilToInt((float)_ep * 1.25f);
		}
		bool canGetEp = true;
		bool hasNextClass = true;
		if (characterData.characterClass->nextClass == nullptr)
		{
			hasNextClass = false;
			if (characterData.level >= 20)
			{
				canGetEp = false;
			}
		}
		if (_ep > 0 && canGetEp)
		{
			StartEPGain(_ep);
			_ep = FMath::Clamp(_ep, 0, GetEPToLevel(characterData.level + 1));
			int ep_count = characterData.ep;
			characterData.ep += _ep;
			
			if (characterData.ep >= GetEPToLevel(characterData.level + 1))
			{
				if (hasNextClass && characterData.level+1 >= 20)
				{
					characterData.ep = 0;
				}
				else
				{
					characterData.ep = characterData.ep - GetEPToLevel(characterData.level);
					if (characterData.ep > ep_count)
					{
						characterData.ep = ep_count;
					}
				}
				
				LevelUp();
			}
		}
	}
}

void AUnit::LevelUp_Implementation()
{
	characterData.level += 1;
	if (characterData.level > 20)
	{
		if (characterData.characterClass != nullptr && characterData.characterClass->nextClass != nullptr)
		{
			characterData.characterClass = characterData.characterClass->nextClass;
			characterData.level = 1;
		}
	}
}

void AUnit::AddValueToStat(EStatEnumerator _stat, int amount)
{
	switch (_stat)
	{
	case EStatEnumerator::HP:
		characterData.stat_HP += amount;
		break;
	case EStatEnumerator::Strength:
		characterData.stat_strength += amount;
		break;
	case EStatEnumerator::Magic:
		characterData.stat_magic += amount;
		break;
	case EStatEnumerator::Endurance:
		characterData.stat_endurance += amount;
		break;
	case EStatEnumerator::Mana:
		characterData.stat_mana += amount;
		break;
	case EStatEnumerator::Defence:
		characterData.stat_defence += amount;
		break;
	case EStatEnumerator::Resistence:
		characterData.stat_resistence += amount;
		break;
	case EStatEnumerator::Skill:
		characterData.stat_skill += amount;
		break;
	case EStatEnumerator::Luck:
		characterData.stat_luck += amount;
		break;
	case EStatEnumerator::Speed:
		characterData.stat_speed += amount;
		break;
	}
}

int AUnit::GetStatUpWithPropability(EStatEnumerator _stat)
{
	switch (_stat)
	{
	case EStatEnumerator::HP:
		if (AdjustedRandom::GetRandomReversed() < GetGrowthHP())
		{
			return 1;
		}
		break;
	case EStatEnumerator::Strength:
		if (AdjustedRandom::GetRandomReversed() < GetGrowthStrength())
		{
			return 1;
		}
		break;
	case EStatEnumerator::Magic:
		if (AdjustedRandom::GetRandomReversed() < GetGrowthMagic())
		{
			return 1;
		}
		break;
	case EStatEnumerator::Endurance:
		if (AdjustedRandom::GetRandomReversed() < GetGrowthEndurance())
		{
			return 1;
		}
		break;
	case EStatEnumerator::Mana:
		if (AdjustedRandom::GetRandomReversed() < GetGrowthMana())
		{
			return 1;
		}
		break;
	case EStatEnumerator::Defence:
		if (AdjustedRandom::GetRandomReversed() < GetGrowthDefence())
		{
			return 1;
		}
		break;
	case EStatEnumerator::Resistence:
		if (AdjustedRandom::GetRandomReversed() < GetGrowthResistence())
		{
			return 1;
		}
		break;
	case EStatEnumerator::Skill:
		if (AdjustedRandom::GetRandomReversed() < GetGrowthSkill())
		{
			return 1;
		}
		break;
	case EStatEnumerator::Luck:
		if (AdjustedRandom::GetRandomReversed() < GetGrowthLuck())
		{
			return 1;
		}
		break;
	case EStatEnumerator::Speed:
		if (AdjustedRandom::GetRandomReversed() < GetGrowthSpeed())
		{
			return 1;
		}
		break;
	}

	return 0;
}

void AUnit::AutoGenerateStatsByLevel()
{
	if (characterData.characterClass != nullptr)
	{
		
		
		int _hp = FMath::Clamp( FMath::RoundToInt(
			(characterData.level + 20 * (characterData.characterClass->tier-1) - 1) *
			(float)GetGrowthHP() / 100.0f), 0,
			characterData.characterClass->max_HP - characterData.characterClass->base_HP);

		if (addManualStats)
		{
			characterData.stat_HP += _hp;
		}
		else
		{
			characterData.stat_HP = _hp;
		}


		int _str = FMath::Clamp(FMath::RoundToInt(
			(characterData.level + 20 * (characterData.characterClass->tier - 1) - 1) *
			(float)GetGrowthStrength() / 100.0f), 0,
			characterData.characterClass->max_strength - characterData.characterClass->base_strength);

		if (addManualStats)
		{
			characterData.stat_strength += _str;
		}
		else
		{
			characterData.stat_strength = _str;
		}

		int _mag = FMath::Clamp(FMath::RoundToInt(
			(characterData.level + 20 * (characterData.characterClass->tier - 1) - 1) *
			(float)GetGrowthMagic() / 100.0f), 0,
			characterData.characterClass->max_magic - characterData.characterClass->base_magic);

		if (addManualStats)
		{
			characterData.stat_magic += _mag;
		}
		else
		{
			characterData.stat_magic = _mag;
		}

		int _end = FMath::Clamp(FMath::RoundToInt(
			(characterData.level + 20 * (characterData.characterClass->tier - 1) - 1) *
			(float)GetGrowthEndurance() / 100.0f), 0,
			characterData.characterClass->max_endurance - characterData.characterClass->base_endurance);

		if (addManualStats)
		{
			characterData.stat_endurance += _end;
		}
		else
		{
			characterData.stat_endurance = _end;
		}

		int _man = FMath::Clamp(FMath::RoundToInt(
			(characterData.level + 20 * (characterData.characterClass->tier - 1) - 1) *
			(float)GetGrowthMana() / 100.0f), 0,
			characterData.characterClass->max_mana - characterData.characterClass->base_mana);

		if (addManualStats)
		{
			characterData.stat_mana += _man;
		}
		else
		{
			characterData.stat_mana = _man;
		}

		int _def = FMath::Clamp(FMath::RoundToInt(
			(characterData.level + 20 * (characterData.characterClass->tier - 1) - 1) *
			(float)GetGrowthDefence() / 100.0f), 0,
			characterData.characterClass->max_defence - characterData.characterClass->base_defence);

		if (addManualStats)
		{
			characterData.stat_defence += _def;
		}
		else
		{
			characterData.stat_defence = _def;
		}

		int _res = FMath::Clamp(FMath::RoundToInt(
			(characterData.level + 20 * (characterData.characterClass->tier - 1) - 1) *
			(float)GetGrowthResistence() / 100.0f), 0,
			characterData.characterClass->max_resistence - characterData.characterClass->base_resistence);

		if (addManualStats)
		{
			characterData.stat_resistence += _res;
		}
		else
		{
			characterData.stat_resistence = _res;
		}

		int _skl = FMath::Clamp(FMath::RoundToInt(
			(characterData.level + 20 * (characterData.characterClass->tier - 1) -1) *
			(float)GetGrowthSkill() / 100.0f), 0,
			characterData.characterClass->max_skill - characterData.characterClass->base_skill);

		if (addManualStats)
		{
			characterData.stat_skill += _skl;
		}
		else
		{
			characterData.stat_skill = _skl;
		}

		int _spd = FMath::Clamp(FMath::RoundToInt(
			(characterData.level + 20 * (characterData.characterClass->tier - 1) - 1) *
			(float)GetGrowthSpeed() / 100.0f), 0,
			characterData.characterClass->max_speed - characterData.characterClass->base_speed);

		if (addManualStats)
		{
			characterData.stat_speed += _spd;
		}
		else
		{
			characterData.stat_speed = _spd;
		}

		int _lck = FMath::Clamp(FMath::RoundToInt(
			(characterData.level + 20 * (characterData.characterClass->tier - 1) - 1) *
			(float)GetGrowthLuck() / 100.0f), 0,
			characterData.characterClass->max_luck - characterData.characterClass->base_luck);

		if (addManualStats)
		{
			characterData.stat_luck += _lck;
		}
		else
		{
			characterData.stat_luck = _lck;
		}

		struct ILocal
		{
			static EWeaponRank Rank(EWeaponRank _max, int level)
			{
				float per = (float)level / 20;
				int8 rnk = FMath::CeilToInt((float)((int8)_max) * per);
				if (rnk <= 0)
				{
					rnk = 1;
				}
				return (EWeaponRank)rnk;
			}
		};


		if (characterData.characterClass->rank_sword > EWeaponRank::None)
		{
			if (characterData.current_rank_sword == EWeaponRank::None)
			{
				characterData.current_rank_sword = ILocal::Rank(characterData.characterClass->rank_sword, GetCurrentLevel());
			}
		}

		if (characterData.characterClass->rank_lance > EWeaponRank::None)
		{
			if (characterData.current_rank_lance == EWeaponRank::None)
			{
				characterData.current_rank_lance = ILocal::Rank(characterData.characterClass->rank_lance, GetCurrentLevel());
			}
		}

		if (characterData.characterClass->rank_axe > EWeaponRank::None)
		{
			if (characterData.current_rank_axe == EWeaponRank::None)
			{
				characterData.current_rank_axe = ILocal::Rank(characterData.characterClass->rank_axe, GetCurrentLevel());
			}
		}

		if (characterData.characterClass->rank_bow > EWeaponRank::None)
		{
			if (characterData.current_rank_bow == EWeaponRank::None)
			{
				characterData.current_rank_bow = ILocal::Rank(characterData.characterClass->rank_bow, GetCurrentLevel());
			}
		}

		if (characterData.characterClass->rank_dagger > EWeaponRank::None)
		{
			if (characterData.current_rank_dagger == EWeaponRank::None)
			{
				characterData.current_rank_dagger = ILocal::Rank(characterData.characterClass->rank_dagger, GetCurrentLevel());
			}
		}

		if (characterData.characterClass->rank_ignis > EWeaponRank::None)
		{
			if (characterData.current_rank_ignis == EWeaponRank::None)
			{
				characterData.current_rank_ignis = ILocal::Rank(characterData.characterClass->rank_ignis, GetCurrentLevel());
			}
		}

		if (characterData.characterClass->rank_terra > EWeaponRank::None)
		{
			if (characterData.current_rank_terra == EWeaponRank::None)
			{
				characterData.current_rank_terra = ILocal::Rank(characterData.characterClass->rank_terra, GetCurrentLevel());
			}
		}

		if (characterData.characterClass->rank_aer > EWeaponRank::None)
		{
			if (characterData.current_rank_aer == EWeaponRank::None)
			{
				characterData.current_rank_aer = ILocal::Rank(characterData.characterClass->rank_aer, GetCurrentLevel());
			}
		}

		if (characterData.characterClass->rank_aqua > EWeaponRank::None)
		{
			if (characterData.current_rank_aqua == EWeaponRank::None)
			{
				characterData.current_rank_aqua = ILocal::Rank(characterData.characterClass->rank_aqua, GetCurrentLevel());
			}
		}

		if (characterData.characterClass->rank_lux > EWeaponRank::None)
		{
			if (characterData.current_rank_lux == EWeaponRank::None)
			{
				characterData.current_rank_lux = ILocal::Rank(characterData.characterClass->rank_lux, GetCurrentLevel());
			}
		}

		if (characterData.characterClass->rank_umbra > EWeaponRank::None)
		{
			if (characterData.current_rank_umbra == EWeaponRank::None)
			{
				characterData.current_rank_umbra = ILocal::Rank(characterData.characterClass->rank_umbra, GetCurrentLevel());
			}
		}

		if (characterData.characterClass->rank_staff > EWeaponRank::None)
		{
			if (characterData.current_rank_staff == EWeaponRank::None)
			{
				characterData.current_rank_staff = ILocal::Rank(characterData.characterClass->rank_staff, GetCurrentLevel());
			}
		}

		if (characterData.characterClass->rank_slash > EWeaponRank::None)
		{
			if (characterData.current_rank_slash == EWeaponRank::None)
			{
				characterData.current_rank_slash = ILocal::Rank(characterData.characterClass->rank_slash, GetCurrentLevel());
			}
		}
	}
}

void AUnit::Damage(int _damage)
{
	if (_damage >= 0)
	{
		characterData.currentHP -= _damage;
		if (characterData.currentHP <= 0)
		{
			characterData.currentHP = 0;
		}
	}
}

void AUnit::Avoid()
{
}

void AUnit::StartEPGain_Implementation(int gainedEP)
{
	
}

void AUnit::StartResourceChangeWindow_Implementation()
{
}

void AUnit::SpawnEffectAtUnit_Implementation(UParticleSystem * _effect)
{
}

bool AUnit::HasEndurance(int amount)
{
	if (characterData.currentEndurance >= amount)
	{
		return true;
	}
	return false;
}

bool AUnit::HasMana(int amount)
{
	if (characterData.currentMana >= amount)
	{
		return true;
	}
	return false;
}

void AUnit::UseEndurance(int amount)
{
	if (amount > 0)
	{
		characterData.currentEndurance -= amount;

		if (characterData.currentEndurance <= 0)
		{
			characterData.currentEndurance = 0;
			TArray<UNavTile*> tls;
			tls.Add(currentTile);
			tileMap->NotifyEnemyRangeTiles(tls);
			tileMap->UpdateEnemyRangeMeshVertexColor();
		}
	}
}

void AUnit::UseMana(int amount)
{
	if (amount > 0)
	{
		characterData.currentMana -= amount;

		if (characterData.currentMana <= 0)
		{
			characterData.currentMana = 0;
			TArray<UNavTile*> tls;
			tls.Add(currentTile);
			tileMap->NotifyEnemyRangeTiles(tls);
			tileMap->UpdateEnemyRangeMeshVertexColor();
		}
	}
}

void AUnit::GainEndurance(int amount)
{
	if (amount > 0)
	{
		characterData.currentEndurance += amount;
		if (characterData.currentEndurance > GetEndurance())
		{
			characterData.currentEndurance = GetEndurance();
		}
	}
}

void AUnit::GainMana(int amount)
{
	if (amount > 0)
	{
		characterData.currentMana += amount;
		if (characterData.currentMana > GetMana())
		{
			characterData.currentMana = GetMana();
		}
	}
}

bool AUnit::IsAlive()
{
	if (!characterData.dead)
	{
		return true;
	}
	return false;
}

int AUnit::GetOffenderDamage(AUnit * _offender, AUnit * _defender)
{
	if (_offender->GetEquippedWeapon() != nullptr)
	{
		int effectivenessMult = 1;
		if (GetOffenderEffectiveness(_offender, _defender))
		{
			effectivenessMult = 2;
		}
		if (_offender->GetEquippedWeapon()->damageType == EDamageType::DT_Physical)
		{
			return FMath::Clamp((_offender->GetPhysicalAttack() * effectivenessMult) - _defender->GetPhysicalDefence(), 0, 999);
		}
		else if (_offender->GetEquippedWeapon()->damageType == EDamageType::DT_Magical)
		{
			return FMath::Clamp((_offender->GetMagicalAttack() * effectivenessMult) - _defender->GetMagicalDefence(), 0, 999);
		}
	}
	return 0;
}

int AUnit::GetOffenderHit(AUnit * _offender, AUnit * _defender)
{

	_offender->tileMap->WeightMapNoUnitBlock(_offender->currentTile, _offender->GetTraverseType());

	int add = 0;

	int heightDiff = FMath::Abs(+_offender->currentTile->height - _defender->currentTile->height);

	if (_offender->currentTile->height > _defender->currentTile->height)
	{
		add = 13*heightDiff;
	}
	else if (_offender->currentTile->height < _defender->currentTile->height)
	{
		add = -13 * heightDiff;
	}

	TArray<UNavTile*> _path;

	int range = FMath::Abs(_offender->currentTile->x - _defender->currentTile->x) 
		+ FMath::Abs(_offender->currentTile->y - _defender->currentTile->y);

	if (_offender->currentTile->currentWeight > _defender->currentTile->currentWeight)
	{
		_path = _defender->tileMap->GetPath(_defender->currentTile, _offender->currentTile);
	}
	else
	{
		_path = _offender->tileMap->GetPath(_offender->currentTile, _defender->currentTile);
	}

	if (_path.Num() - 1 <= range)
	{
		add = 0;
	}

	int wpAdd = 0;
	if (_offender->GetEquippedWeapon() != nullptr && _defender->GetEquippedWeapon() != nullptr)
	{
		EWeaponAdvantages adv = UWeaponData::GetWeaponAdvantage(_offender->GetEquippedWeapon()->type, _defender->GetEquippedWeapon()->type);
		switch (adv)
		{
		case EWeaponAdvantages::WA_Advantage:
			wpAdd = 25;
			break;
		case EWeaponAdvantages::WA_None:
			break;
		case EWeaponAdvantages::WA_Disadvantage:
			wpAdd = -25;
			break;
		default:
			break;
		}
	}
	

	return FMath::Clamp((_offender->GetHitChance() - _defender->GetAvoidChance() + add) + wpAdd, 0, 100);
}

int AUnit::GetOffenderCrit(AUnit * _offender, AUnit * _defender)
{
	int wpAdd = 0;
	if (_offender->GetEquippedWeapon() != nullptr && _defender->GetEquippedWeapon() != nullptr)
	{
		EWeaponAdvantages adv = UWeaponData::GetWeaponAdvantage(_offender->GetEquippedWeapon()->type, _defender->GetEquippedWeapon()->type);
		switch (adv)
		{
		case EWeaponAdvantages::WA_Advantage:
			wpAdd = 5;
			break;
		case EWeaponAdvantages::WA_None:
			break;
		case EWeaponAdvantages::WA_Disadvantage:
			wpAdd = -5;
			break;
		default:
			break;
		}
	}

	return FMath::Clamp(_offender->GetCritChance() - _defender->GetCritDefence() + wpAdd, 0, 100);
}

bool AUnit::GetOffenderDouble(AUnit * _offender, AUnit * _defender)
{
	int double_comp = _offender->GetDoubleCompare() - _defender->GetDoubleCompare();

	if (double_comp >= 4)
	{
		return true;
	}
	return false;
}

bool AUnit::GetOffenderEffectiveness(AUnit * _offender, AUnit * _defender)
{
	if (_offender->GetEquippedWeapon() != nullptr)
	{
		if (_defender->characterData.additionalTags.HasAny(_offender->GetEquippedWeapon()->effectiveness))
		{
			return true;
		}
		else if (_defender->characterData.characterClass->classTags.HasAny(_offender->GetEquippedWeapon()->effectiveness))
		{
			return true;
		}
		
	}
	return false;
}

TArray<UActionOption*> AUnit::GetValidActionOptions()
{
	TArray<UActionOption*> ret;

	UTalkOption* talk = NewObject<UTalkOption>();
	if (talk->MeetsRequirements(this))
	{
		ret.Add(talk);
	}

	UAttackOption* attack = NewObject<UAttackOption>();
	if (attack->MeetsRequirements(this))
	{
		ret.Add(attack);
	}

	UHealOption* heal = NewObject<UHealOption>();
	if (heal->MeetsRequirements(this))
	{
		ret.Add(heal);
	}

	UOpenTreasure* tres = NewObject<UOpenTreasure>();
	if (tres->MeetsRequirements(this))
	{
		ret.Add(tres);
	}

	UPullLeverOption* lev = NewObject<UPullLeverOption>();
	if (lev->MeetsRequirements(this))
	{
		ret.Add(lev);
	}

	UAnimaSoulOption* animaS = NewObject<UAnimaSoulOption>();
	if (animaS->MeetsRequirements(this))
	{
		ret.Add(animaS);
	}


	UItemOption* itemopt = NewObject<UItemOption>();
	if (itemopt->MeetsRequirements(this))
	{
		ret.Add(itemopt);
	}

	UTrade* trade = NewObject<UTrade>();
	if (trade->MeetsRequirements(this))
	{
		ret.Add(trade);
	}

	URestOption* rest = NewObject<URestOption>();
	if (rest->MeetsRequirements(this))
	{
		ret.Add(rest);
	}

	UWaitOption* wait = NewObject<UWaitOption>();
	if (wait->MeetsRequirements(this))
	{
		ret.Add(wait);
	}

	return ret;
}

void AUnit::OnRest_Implementation()
{
}

void AUnit::Die(AUnit* _killer)
{
	characterData.dead = true;
	SetCurrentTile(nullptr);
	SetActorLocation(FVector(0, 0, -10000));

	

	TArray<UNavTile*> tiles;
	tiles.Add(currentTile);
	tileMap->NotifyEnemyRangeTiles(tiles);

	if (currentTile != nullptr)
	{
		if (blocksAttacks != currentTile->tileData->blocksAttacks)
		{
			tileMap->BuildTempWeights(tiles);
		}
	}

	if (tileMap->EnemyRangeContainsUnit(this))
	{
		
		tileMap->RemoveEnemyRangeUnit(this);
	}

	tileMap->UpdateEnemyRangeMeshVertexColor();

	if (_killer != nullptr)
	{
		if (_killer->team == EUnitTeam::E_Player)
		{
			TArray<UItemInstance*> drops;
			for (int i = 0; i < characterData.inventory.Num(); i++)
			{
				if (characterData.inventory[i]->drop)
				{
					drops.Add(characterData.inventory[i]);
					characterData.inventory[i]->drop = false;
					_killer->AddItemWithNotification(characterData.inventory[i]);
				}
			}
			for (int i = 0; i < drops.Num(); i++)
			{
				RemoveItemFrominventory(drops[i]);
			}
		}
	}
}

bool AUnit::isDead()
{
	return characterData.dead;
}

bool AUnit::IsInRangeOfCurrentWeapon(AUnit * _other)
{
	if (_other != nullptr)
	{
		int range = FMath::Abs(_other->currentTile->x - currentTile->x) + FMath::Abs(_other->currentTile->y - currentTile->y);

		if (GetEquippedWeapon() != nullptr)
		{
			if (GetEquippedWeapon()->range.Min <= range && GetEquippedWeapon()->range.Max >= range)
			{
				return true;
			}
		}
	}

	return false;
}

void AUnit::UseItem(UItemInstance * _item)
{
	struct ILocal
	{
		static void UnitUseItem(AUnit* _unit, UItemInstance* _it)
		{
			if (dynamic_cast<UHealingItemData*>(_it->item))
			{
				UHealingItemData* dt = dynamic_cast<UHealingItemData*>(_it->item);
				_unit->Heal(dt->amount);
			}
			if (dynamic_cast<UResourceItemData*>(_it->item))
			{
				UResourceItemData* dt = dynamic_cast<UResourceItemData*>(_it->item);
				if (dt->type == EResourceType::Endurance)
				{
					_unit->GainEndurance(dt->amount);
				}
				else if (dt->type == EResourceType::Mana)
				{
					_unit->GainMana(dt->amount);
				}
			}
			if (dynamic_cast<UStatBonusItemData*>(_it->item))
			{
				UStatBonusItemData* dt = dynamic_cast<UStatBonusItemData*>(_it->item);
				
				_unit->characterData.stat_HP += dt->hp;
				_unit->characterData.stat_endurance += dt->endurence;
				_unit->characterData.stat_mana += dt->mana;
				_unit->characterData.stat_strength += dt->strength;
				_unit->characterData.stat_magic += dt->magic;
				_unit->characterData.stat_defence += dt->defense;
				_unit->characterData.stat_resistence += dt->resistence;
				_unit->characterData.stat_skill += dt->skill;
				_unit->characterData.stat_luck += dt->luck;
				_unit->characterData.stat_speed += dt->speed;
			}
			if (dynamic_cast<UStatusEffectItemData*>(_it->item))
			{
				UStatusEffectItemData* dt = dynamic_cast<UStatusEffectItemData*>(_it->item);

				for (int i = 0; i < dt->statusEffects.Num(); i++)
				{
					_unit->AddStatusEffectPropability(dt->statusEffects[i]);
				}
			}
		}
	};


	if (characterData.inventory.Contains(_item))
	{
		if (_item != nullptr)
		{
			ILocal::UnitUseItem(this, _item);
			_item->durability -= 1;
			if (_item->durability <= 0)
			{
				_item->broken = true;
				int id = 0;
				for (int i = 0; i < characterData.inventory.Num(); i++)
				{
					if (characterData.inventory[i] == _item)
					{
						id = i;
						break;
					}
				}
				characterData.inventory.RemoveAt(id);
			}
		}
	}
}

void AUnit::AddWeaponRankEP(EWeaponType _type, int _ep)
{
	struct ILocal
	{
		static EWeaponRank GetNextWeaponRank(EWeaponRank _rank)
		{
			if (_rank == EWeaponRank::None)
			{
				return EWeaponRank::E;
			}
			else if (_rank == EWeaponRank::E)
			{
				return EWeaponRank::D;
			}
			else if (_rank == EWeaponRank::D)
			{
				return EWeaponRank::C;
			}
			else if (_rank == EWeaponRank::C)
			{
				return EWeaponRank::B;
			}
			else if (_rank == EWeaponRank::B)
			{
				return EWeaponRank::A;
			}
			else if (_rank == EWeaponRank::A)
			{
				return EWeaponRank::S;
			}
			else if (_rank == EWeaponRank::S)
			{
				return EWeaponRank::SS;
			}
			return EWeaponRank::None;
		}
	};

	if (team != EUnitTeam::E_Player)
	{
		return;
	}

	if (_type == EWeaponType::Sword)
	{
		if (characterData.current_rank_sword != characterData.characterClass->rank_sword)
		{
			characterData.current_rankEP_sword += _ep;

			uint8 w_rank = (uint8)ILocal::GetNextWeaponRank(characterData.current_rank_sword);
			if (characterData.current_rankEP_sword >= GetNeededRankEPToRank(w_rank))
			{
				characterData.current_rank_sword = ILocal::GetNextWeaponRank(characterData.current_rank_sword);
				OnWeaponRankUp(EWeaponType::Sword);
				characterData.current_rankEP_sword = 0;
			}
		}
	}
	else if (_type == EWeaponType::Axe)
	{
		if (characterData.current_rank_axe != characterData.characterClass->rank_axe)
		{
			characterData.current_rankEP_axe += _ep;

			uint8 w_rank = (uint8)ILocal::GetNextWeaponRank(characterData.current_rank_axe);
			if (characterData.current_rankEP_axe >= GetNeededRankEPToRank(w_rank))
			{
				characterData.current_rank_axe = ILocal::GetNextWeaponRank(characterData.current_rank_axe);
				OnWeaponRankUp(EWeaponType::Axe);
				characterData.current_rankEP_axe = 0;
			}
		}
	}
	else if (_type == EWeaponType::Lance)
	{
		if (characterData.current_rank_lance != characterData.characterClass->rank_lance)
		{
			characterData.current_rankEP_lance += _ep;

			uint8 w_rank = (uint8)ILocal::GetNextWeaponRank(characterData.current_rank_lance);
			if (characterData.current_rankEP_lance >= GetNeededRankEPToRank(w_rank))
			{
				characterData.current_rank_lance = ILocal::GetNextWeaponRank(characterData.current_rank_lance);
				OnWeaponRankUp(EWeaponType::Lance);
				characterData.current_rankEP_lance = 0;
			}
		}
	}
	else if (_type == EWeaponType::Bow)
	{
		if (characterData.current_rank_bow != characterData.characterClass->rank_bow)
		{
			characterData.current_rankEP_bow += _ep;

			uint8 w_rank = (uint8)ILocal::GetNextWeaponRank(characterData.current_rank_bow);
			if (characterData.current_rankEP_bow >= GetNeededRankEPToRank(w_rank))
			{
				characterData.current_rank_bow = ILocal::GetNextWeaponRank(characterData.current_rank_bow);
				OnWeaponRankUp(EWeaponType::Bow);
				characterData.current_rankEP_bow = 0;
			}
		}
	}
	else if (_type == EWeaponType::Dagger)
	{
		if (characterData.current_rank_dagger != characterData.characterClass->rank_dagger)
		{
			characterData.current_rankEP_dagger += _ep;

			uint8 w_rank = (uint8)ILocal::GetNextWeaponRank(characterData.current_rank_dagger);
			if (characterData.current_rankEP_dagger >= GetNeededRankEPToRank(w_rank))
			{
				characterData.current_rank_dagger = ILocal::GetNextWeaponRank(characterData.current_rank_dagger);
				OnWeaponRankUp(EWeaponType::Dagger);
				characterData.current_rankEP_dagger = 0;
			}
		}
	}
	else if (_type == EWeaponType::Ignis)
	{
		if (characterData.current_rank_ignis != characterData.characterClass->rank_ignis)
		{
			characterData.current_rankEP_ignis += _ep;

			uint8 w_rank = (uint8)ILocal::GetNextWeaponRank(characterData.current_rank_ignis);
			if (characterData.current_rankEP_ignis >= GetNeededRankEPToRank(w_rank))
			{
				characterData.current_rank_ignis = ILocal::GetNextWeaponRank(characterData.current_rank_ignis);
				OnWeaponRankUp(EWeaponType::Ignis);
				characterData.current_rankEP_ignis = 0;
			}
		}
	}
	else if (_type == EWeaponType::Terra)
	{
		if (characterData.current_rank_terra != characterData.characterClass->rank_terra)
		{
			characterData.current_rankEP_terra += _ep;

			uint8 w_rank = (uint8)ILocal::GetNextWeaponRank(characterData.current_rank_terra);
			if (characterData.current_rankEP_terra >= GetNeededRankEPToRank(w_rank))
			{
				characterData.current_rank_terra = ILocal::GetNextWeaponRank(characterData.current_rank_terra);
				OnWeaponRankUp(EWeaponType::Terra);
				characterData.current_rankEP_terra = 0;
			}
		}
	}
	else if (_type == EWeaponType::Aer)
	{
		if (characterData.current_rank_aer != characterData.characterClass->rank_aer)
		{
			characterData.current_rankEP_aer += _ep;

			uint8 w_rank = (uint8)ILocal::GetNextWeaponRank(characterData.current_rank_aer);
			if (characterData.current_rankEP_aer >= GetNeededRankEPToRank(w_rank))
			{
				characterData.current_rank_aer = ILocal::GetNextWeaponRank(characterData.current_rank_aer);
				OnWeaponRankUp(EWeaponType::Aer);
				characterData.current_rankEP_aer = 0;
			}
		}
	}
	else if (_type == EWeaponType::Aqua)
	{
		if (characterData.current_rank_aqua != characterData.characterClass->rank_aqua)
		{
			characterData.current_rankEP_aqua += _ep;

			uint8 w_rank = (uint8)ILocal::GetNextWeaponRank(characterData.current_rank_aqua);
			if (characterData.current_rankEP_aqua >= GetNeededRankEPToRank(w_rank))
			{
				characterData.current_rank_aqua = ILocal::GetNextWeaponRank(characterData.current_rank_aqua);
				OnWeaponRankUp(EWeaponType::Aqua);
				characterData.current_rankEP_aqua = 0;
			}
		}
	}
	else if (_type == EWeaponType::Lux)
	{
		if (characterData.current_rank_lux != characterData.characterClass->rank_lux)
		{
			characterData.current_rankEP_lux += _ep;

			uint8 w_rank = (uint8)ILocal::GetNextWeaponRank(characterData.current_rank_lux);
			if (characterData.current_rankEP_lux >= GetNeededRankEPToRank(w_rank))
			{
				characterData.current_rank_lux = ILocal::GetNextWeaponRank(characterData.current_rank_lux);
				OnWeaponRankUp(EWeaponType::Lux);
				characterData.current_rankEP_lux = 0;
			}
		}
	}
	else if (_type == EWeaponType::Umbra)
	{
		if (characterData.current_rank_umbra != characterData.characterClass->rank_umbra)
		{
			characterData.current_rankEP_umbra += _ep;

			uint8 w_rank = (uint8)ILocal::GetNextWeaponRank(characterData.current_rank_umbra);
			if (characterData.current_rankEP_umbra >= GetNeededRankEPToRank(w_rank))
			{
				characterData.current_rank_umbra = ILocal::GetNextWeaponRank(characterData.current_rank_umbra);
				OnWeaponRankUp(EWeaponType::Umbra);
				characterData.current_rankEP_umbra = 0;
			}
		}
	}
	else if (_type == EWeaponType::Staff)
	{
		if (characterData.current_rank_staff != characterData.characterClass->rank_staff)
		{
			characterData.current_rankEP_staff += _ep;

			uint8 w_rank = (uint8)ILocal::GetNextWeaponRank(characterData.current_rank_staff);
			if (characterData.current_rankEP_staff >= GetNeededRankEPToRank(w_rank))
			{
				characterData.current_rank_staff = ILocal::GetNextWeaponRank(characterData.current_rank_staff);
				OnWeaponRankUp(EWeaponType::Staff);
				characterData.current_rankEP_staff = 0;
			}
		}
	}
	else if (_type == EWeaponType::Slash)
	{
		if (characterData.current_rank_slash != characterData.characterClass->rank_slash)
		{
			characterData.current_rankEP_slash += _ep;

			uint8 w_rank = (uint8)ILocal::GetNextWeaponRank(characterData.current_rank_slash);
			if (characterData.current_rankEP_slash >= GetNeededRankEPToRank(w_rank))
			{
				characterData.current_rank_slash = ILocal::GetNextWeaponRank(characterData.current_rank_slash);
				OnWeaponRankUp(EWeaponType::Slash);
				characterData.current_rankEP_slash = 0;
			}
		}
	}
}

void AUnit::OnWeaponRankUp_Implementation(EWeaponType _type)
{
}

bool AUnit::IsMaxWeaponRank(EWeaponType _type)
{

	switch (_type)
	{
	case EWeaponType::Sword:
		if (characterData.current_rank_sword >= characterData.characterClass->rank_sword)
		{
			return true;
		}
		break;
	case EWeaponType::Lance:
		if (characterData.current_rank_lance >= characterData.characterClass->rank_lance)
		{
			return true;
		}
		break;
	case EWeaponType::Axe:
		if (characterData.current_rank_axe >= characterData.characterClass->rank_axe)
		{
			return true;
		}
		break;
	case EWeaponType::Bow:
		if (characterData.current_rank_bow >= characterData.characterClass->rank_bow)
		{
			return true;
		}
		break;
	case EWeaponType::Dagger:
		if (characterData.current_rank_dagger >= characterData.characterClass->rank_dagger)
		{
			return true;
		}
		break;
	case EWeaponType::Ignis:
		if (characterData.current_rank_ignis >= characterData.characterClass->rank_ignis)
		{
			return true;
		}
		break;
	case EWeaponType::Terra:
		if (characterData.current_rank_terra >= characterData.characterClass->rank_terra)
		{
			return true;
		}
		break;
	case EWeaponType::Aer:
		if (characterData.current_rank_aer >= characterData.characterClass->rank_aer)
		{
			return true;
		}
		break;
	case EWeaponType::Aqua:
		if (characterData.current_rank_aqua >= characterData.characterClass->rank_aqua)
		{
			return true;
		}
		break;
	case EWeaponType::Lux:
		if (characterData.current_rank_lux >= characterData.characterClass->rank_lux)
		{
			return true;
		}
		break;
	case EWeaponType::Umbra:
		if (characterData.current_rank_umbra >= characterData.characterClass->rank_umbra)
		{
			return true;
		}
		break;
	case EWeaponType::Staff:
		if (characterData.current_rank_staff >= characterData.characterClass->rank_staff)
		{
			return true;
		}
		break;
	case EWeaponType::Slash:
		if (characterData.current_rank_slash >= characterData.characterClass->rank_slash)
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

int AUnit::GetNeededRankEPToRank(uint8 _rank)
{
	return 50 * _rank;
}

EWeaponRank AUnit::GetWeaponRank(EWeaponType _rank)
{
	switch (_rank)
	{
	case EWeaponType::Sword:
		return characterData.current_rank_sword;
		break;
	case EWeaponType::Lance:
		return characterData.current_rank_lance;
		break;
	case EWeaponType::Axe:
		return characterData.current_rank_axe;
		break;
	case EWeaponType::Bow:
		return characterData.current_rank_bow;
		break;
	case EWeaponType::Dagger:
		return characterData.current_rank_dagger;
		break;
	case EWeaponType::Ignis:
		return characterData.current_rank_ignis;
		break;
	case EWeaponType::Terra:
		return characterData.current_rank_terra;
		break;
	case EWeaponType::Aer:
		return characterData.current_rank_aer;
		break;
	case EWeaponType::Aqua:
		return characterData.current_rank_aqua;
		break;
	case EWeaponType::Lux:
		return characterData.current_rank_lux;
		break;
	case EWeaponType::Umbra:
		return characterData.current_rank_umbra;
		break;
	case EWeaponType::Staff:
		return characterData.current_rank_staff;
		break;
	case EWeaponType::Slash:
		return characterData.current_rank_slash;
		break;
	default:
		break;
	}
	return EWeaponRank::None;
}

void AUnit::Heal(int _amount)
{
	if (_amount > 0)
	{
		characterData.currentHP += _amount;
		if (characterData.currentHP > GetHP())
		{
			characterData.currentHP = GetHP();
		}
	}
}

UPerson * AUnit::GetPerson()
{

	if (characterData.person != nullptr)
	{
		return characterData.person;
	}
	return nullptr;
}

TArray<TSubclassOf<UAbility>> AUnit::GetAbilities()
{
	TArray<TSubclassOf<UAbility>> ret;

	ret.Append(characterData.characterClass->gainAbalitys);
	ret.Append(characterData.characterAbilitys);

	return ret;
}

void AUnit::RemoveItemFrominventory(UItemInstance * _item)
{
	characterData.inventory.Remove(_item);
}

int AUnit::GetIndexOfItem(UItemInstance * _item)
{
	if (_item != nullptr)
	{
		for (int i = 0; i < characterData.inventory.Num(); i++)
		{
			if (characterData.inventory[i] == _item)
			{
				return i;
			}
		}
	}
	return -1;
}

void AUnit::AddItemAtID(UItemInstance * _item, int id)
{
	if (_item != nullptr)
	{
		if (id >= 0)
		{
			characterData.inventory.EmplaceAt(id, _item);
		}
		else
		{
			if (characterData.inventory.Num() < 8)
			{
				AddItem(_item);
			}
		}
	}
}
