// Fill out your copyright notice in the Description page of Project Settings.


#include "BookmarkSave.h"
#include "Kismet/GameplayStatics.h"

UBookmarkSave::UBookmarkSave()
{
	
}

UBookmarkSave::~UBookmarkSave()
{
}

void UBookmarkSave::RegisterUnits(TArray<AUnit*> _units)
{
	unitRefs.Empty();
	characters.Empty();
	unitData.Empty();
	for (int i = 0; i < _units.Num(); i++)
	{

		AUnit* u = _units[i];
		unitRefs.Add(u);
		WriteUnitInventoryData(u);
		characters.Add(u->characterData);
		unitData.Add(GetUnitData(u));
		
	}
	for (int i = 0; i < _units.Num(); i++)
	{
		if (unitData[i].spawnedBy != nullptr)
		{
			if (dynamic_cast<AUnit*>(unitData[i].spawnedBy))
			{
				for (int u = 0; u < _units.Num(); u++)
				{
					if (_units[u] == unitData[i].spawnedBy)
					{
						unitData[i].spawnedByUnitID = u;
						break;
					}
				}
			}
			else
			{
				unitData[i].spawnedByUnitID = -1;
			}
		}
		else
		{
			unitData[i].spawnedByUnitID = -1;
		}
	}
}

void UBookmarkSave::ResolveUnits()
{
	for (int i = 0; i < unitRefs.Num(); i++)
	{
		if (unitData[i].spawnedBy != nullptr)
		{
			SpawnUnit(i);
		}

		if ((unitRefs[i]) != nullptr)
		{
			(unitRefs[i])->characterData = characters[i];
			ResolveUnitInventoryData((unitRefs[i]));
			ResolveUnitData(unitRefs[i], unitData[i]);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Unit couldnt be Spawned"));
		}
	}
}

void UBookmarkSave::RegisterTreasures(TArray<ATreasure*> _treasures)
{
	treasures.Empty();
	treasureSaves.Empty();
	for (int i = 0; i < _treasures.Num(); i++)
	{
		ATreasure* tr = _treasures[i];
		WriteTreasureData(tr);
		treasures.Add(tr);
	}
}

void UBookmarkSave::ResolveTreasures()
{
	for (int i = 0; i < treasures.Num(); i++)
	{
		ATreasure* tr = treasures[i];
		ResolveTreasureData(tr, treasureSaves[i]);
	}
}

void UBookmarkSave::RegisterLeavers(TArray<ALeaver*> _leavers)
{
	leavers.Empty();
	leaverSaves.Empty();
	for (int i = 0; i < _leavers.Num(); i++)
	{
		ALeaver* tr = _leavers[i];
		leaverSaves.Add(_leavers[i]->pulled);
		leavers.Add(tr);
	}
}

void UBookmarkSave::ResolveLeavers()
{
	for (int i = 0; i < leavers.Num(); i++)
	{
		ALeaver* tr = leavers[i];
		tr->pulled = leaverSaves[i];
	}
}

FUnitDataSave UBookmarkSave::GetUnitData(AUnit * _unit)
{
	FUnitDataSave ret;

	ret.team = _unit->team;
	ret.autoGenerateStats = _unit->autoGenerateStats;
	ret.addManualStats = _unit->addManualStats;
	ret.blocksShadow = _unit->blocksShadow;
	ret.blocksAttacks = _unit->blocksAttacks;

	for (int i = 0; i < _unit->statusEffects.Num(); i++)
	{
		FStatusEffectSave s;
		s.statusEffect = _unit->statusEffects[i]->statusEffect;
		s.remaining = _unit->statusEffects[i]->remaining;
		ret.statusEffects.Add(s);
	}

	ret.canMove = _unit->canMove;
	ret.tileMap = _unit->tileMap;
	ret.currentTile = _unit->currentTile;
	ret.lastTile = _unit->lastTile;
	ret.lookRotation = _unit->lookRotation;
	ret.state = _unit->state;
	ret.alreadyMoved = _unit->alreadyMoved;
	ret.location = _unit->GetActorLocation();
	ret.spawnedBy = _unit->spawnedBy;
	ret.behavior = _unit->currentBehavior;

	return ret;
}

void UBookmarkSave::ResolveUnitData(AUnit * _unit, FUnitDataSave unitDataSave)
{
	_unit->team = unitDataSave.team;
	_unit->autoGenerateStats = unitDataSave.autoGenerateStats;
	_unit->addManualStats = unitDataSave.addManualStats;
	_unit->blocksShadow = unitDataSave.blocksShadow;
	_unit->blocksAttacks = unitDataSave.blocksAttacks;

	_unit->statusEffects.Empty();
	for (int i = 0; i < unitDataSave.statusEffects.Num(); i++)
	{
		UStatusEffectInstance* s = NewObject<UStatusEffectInstance>();

		s->statusEffect = unitDataSave.statusEffects[i].statusEffect;
		s->remaining = unitDataSave.statusEffects[i].remaining;
		_unit->statusEffects.Add(s);
	}

	_unit->canMove = unitDataSave.canMove;
	_unit->tileMap = unitDataSave.tileMap;
	if (!_unit->characterData.dead)
	{
		_unit->ForceCurrentTile(unitDataSave.currentTile);
		_unit->currentTile->unit = _unit;
		_unit->lastTile = unitDataSave.lastTile;
	}
	else
	{
		_unit->SetCurrentTile(nullptr);
	}
	_unit->lookRotation = unitDataSave.lookRotation;
	_unit->state = unitDataSave.state;
	_unit->alreadyMoved = unitDataSave.alreadyMoved;
	_unit->spawnedBy = unitDataSave.spawnedBy;
	_unit->currentBehavior = unitDataSave.behavior;

	_unit->SetActorRotation(unitDataSave.lookRotation);
	_unit->SetActorLocation(unitDataSave.location);
}

void UBookmarkSave::WriteUnitInventoryData(AUnit * _unit)
{
	_unit->characterData.inventorySave.Empty();
	for (int i = 0; i < _unit->characterData.inventory.Num(); i++)
	{
		UItemInstance* it = _unit->characterData.inventory[i];
		FItemInstanceSave save;
		save.item = it->item;
		save.durability = it->durability;
		save.broken = it->broken;
		save.drop = it->drop;
		save.bound = it->bound;
		_unit->characterData.inventorySave.Add(save);
	}
}

void UBookmarkSave::ResolveUnitInventoryData(AUnit * _unit)
{
	_unit->characterData.inventory.Empty();
	TArray<UItemInstance*> newInv;
	for (int i = 0; i < _unit->characterData.inventorySave.Num(); i++)
	{
		UItemInstance* it = NewObject<UItemInstance>();
		it->item = _unit->characterData.inventorySave[i].item;
		it->durability = _unit->characterData.inventorySave[i].durability;
		it->broken = _unit->characterData.inventorySave[i].broken;
		it->drop = _unit->characterData.inventorySave[i].drop;
		it->bound = _unit->characterData.inventorySave[i].bound;
		newInv.Add(it);
	}
	_unit->characterData.inventory = newInv;
}

void UBookmarkSave::WriteTreasureData(ATreasure * _treasure)
{
	FTreasureSave ts;
	ts.item = _treasure->item;
	ts.opened = _treasure->opened;
	treasureSaves.Add(ts);
}

void UBookmarkSave::ResolveTreasureData(ATreasure * _treasure, FTreasureSave _tresureSave)
{
	_treasure->item = _tresureSave.item;
	_treasure->opened = _tresureSave.opened;
}

void UBookmarkSave::SpawnUnit(int _unitID)
{
	if (_navMap != nullptr)
	{
		_navMap->SpawnUnit(unitData[_unitID].spawnedBy);
		if (_navMap->loadSpawnUnit == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Spawned Unit Is Null"));
		}
		unitRefs[_unitID] = _navMap->loadSpawnUnit;
	}
}
