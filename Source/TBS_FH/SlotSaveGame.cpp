// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotSaveGame.h"

void USlotSaveGame::WriteConvoy(TArray<UItemInstance*> _items, int _team)
{
	teams[_team].convoy.Empty();
	for (int i = 0; i < _items.Num(); i++)
	{
		UItemInstance* it = _items[i];
		FItemInstanceSave save;
		save.item = it->item;
		save.durability = it->durability;
		save.broken = it->broken;
		save.drop = it->drop;
		teams[_team].convoy.Add(save);
	}
}

TArray<UItemInstance*> USlotSaveGame::ReadConvoy(int _team)
{
	TArray<UItemInstance*> newCon;
	for (int i = 0; i < teams[_team].convoy.Num(); i++)
	{
		UItemInstance* it = NewObject<UItemInstance>();
		it->item = teams[_team].convoy[i].item;
		it->durability = teams[_team].convoy[i].durability;
		it->broken = teams[_team].convoy[i].broken;
		it->drop = teams[_team].convoy[i].drop;
		it->bound = teams[_team].convoy[i].bound;
		newCon.Add(it);
	}
	return newCon;
}

void USlotSaveGame::WriteCharacters(TArray<FCharacterData> _chars, int _team)
{
	teams[_team].characters = _chars;
	for (int c = 0; c < teams[_team].characters.Num(); c++)
	{
		teams[_team].characters[c].inventorySave.Empty();
		for (int i = 0; i < teams[_team].characters[c].inventory.Num(); i++)
		{
			UItemInstance* it = teams[_team].characters[c].inventory[i];
			FItemInstanceSave save;
			save.item = it->item;
			save.durability = it->durability;
			save.broken = it->broken;
			save.bound = it->bound;
			teams[_team].characters[c].inventorySave.Add(save);
		}
	}
}

TArray<FCharacterData> USlotSaveGame::ReadCharacters(int _team)
{
	for (int c = 0; c < teams[_team].characters.Num(); c++)
	{
		teams[_team].characters[c].inventory.Empty();
		TArray<UItemInstance*> newInv;
		for (int i = 0; i < teams[_team].characters[c].inventorySave.Num(); i++)
		{
			UItemInstance* it = NewObject<UItemInstance>();
			it->item = teams[_team].characters[c].inventorySave[i].item;
			it->durability = teams[_team].characters[c].inventorySave[i].durability;
			it->broken = teams[_team].characters[c].inventorySave[i].broken;
			it->bound = teams[_team].characters[c].inventorySave[i].bound;
			newInv.Add(it);
		}
		teams[_team].characters[c].inventory = newInv;
	}
	return teams[_team].characters;
}

void USlotSaveGame::WriteSaveGameData(ULevelOrderLink* level, int levelID, int _run, FDifficultySettings _diff)
{
	chapterLevel = level;
	currentLevelIndex = levelID;
	run = _run;
	difficulty = _diff;
}

void USlotSaveGame::WriteAll(AGameSlotData * _gameData)
{
	WriteSaveGameData(_gameData->chapterLevel, _gameData->currentLevelIndex, _gameData->run, _gameData->difficulty);
	teams.Empty();
	for (int i = 0; i < _gameData->teams.Num(); i++)
	{
		teams.Add(FTeamConvoy());
		WriteConvoy(_gameData->teams[i].convoy, i);
		WriteCharacters(_gameData->teams[i].characters, i);
		teams[i].gold = _gameData->teams[i].gold;
		teams[i].bonusEP = _gameData->teams[i].bonusEP;
		teams[i].abilityScrolls = _gameData->teams[i].abilityScrolls;
		teams[i].genericUnits = _gameData->teams[i].genericUnits;
	}
}

void USlotSaveGame::ReadAll(AGameSlotData * _gameData)
{
	_gameData->chapterLevel = chapterLevel;
	_gameData->currentLevelIndex = currentLevelIndex;
	_gameData->run = run;
	_gameData->difficulty = difficulty;

	_gameData->teams.Empty();
	for (int i = 0; i < teams.Num(); i++)
	{
		_gameData->teams.Add(FTeamConvoyInstance());
		_gameData->teams[i].convoy = ReadConvoy(i);
		_gameData->teams[i].characters = ReadCharacters(i);
		_gameData->teams[i].gold = teams[i].gold;
		_gameData->teams[i].bonusEP = teams[i].bonusEP;
		_gameData->teams[i].genericUnits = teams[i].genericUnits;
		_gameData->teams[i].abilityScrolls = teams[i].abilityScrolls;
	}
}
