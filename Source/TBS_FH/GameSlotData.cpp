// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSlotData.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameSlotData::AGameSlotData()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameSlotData::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameSlotData::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AGameSlotData * AGameSlotData::GetGameSlotData(AActor * outer)
{

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(outer->GetWorld(), AGameSlotData::StaticClass(), FoundActors);
	if (FoundActors.Num() <= 0)
	{
		return nullptr;
	}
	return dynamic_cast<AGameSlotData*>(FoundActors[0]);

}

void AGameSlotData::AddCharacter(FCharacterData _char, bool initInventory, int _team)
{
	CheckTeam(_team);
	bool contains = false;
	for (int i = 0; i < teams[_team].characters.Num() && !contains; i++)
	{
		if (teams[_team].characters[i].person == _char.person)
		{
			contains = true;
		}
	}
	if (!contains)
	{
		if (initInventory)
		{
			for (int i = 0; i < _char.startItems.Num(); i++)
			{
				UItemInstance* it_inst = NewObject<UItemInstance>();
				it_inst->item = (_char.startItems[i])._item;
				if ((_char.startItems[i]).customDurability > 0)
				{
					it_inst->durability = (_char.startItems[i]).customDurability;
				}
				else
				{
					it_inst->durability = (_char.startItems[i])._item->durability;
				}
				it_inst->drop = (_char.startItems[i]).drop;
				it_inst->bound = (_char.startItems[i]).bound;
				_char.inventory.Add(it_inst);
			}
		}
		teams[_team].characters.Add(_char);
	}
}

FCharacterData AGameSlotData::GetCharacterDataByPerson(UPerson * _person, int _team)
{
	CheckTeam(_team);
	for (int i = 0; i < teams[_team].characters.Num(); i++)
	{
		if (teams[_team].characters[i].person == _person)
		{
			return teams[_team].characters[i];
		}
	}

	return FCharacterData();
}

bool AGameSlotData::ContainsPerson(UPerson * _person, int _team)
{
	CheckTeam(_team);
	for (int i = 0; i < teams[_team].characters.Num(); i++)
	{
		if (teams[_team].characters[i].person == _person)
		{
			return true;
		}
	}
	return false;
}

void AGameSlotData::UpdateCharacterData(TArray<FCharacterData> _chars, int _team)
{
	CheckTeam(_team);
	for (int i = 0; i < _chars.Num(); i++)
	{

		for (int c = 0; c < teams[_team].characters.Num(); c++)
		{
			if (teams[_team].characters[c].person == _chars[i].person)
			{
				teams[_team].characters[c] = _chars[i];
				break;
			}
		}

	}
}

void AGameSlotData::AddItemToConvoy(UItemInstance * _item, int _team)
{
	CheckTeam(_team);
	if (_item != nullptr)
	{
		if (_team < teams.Num())
		{
			teams[_team].convoy.Add(_item);
		}
		else
		{
			teams[0].convoy.Add(_item);
		}
	}
}

bool AGameSlotData::TradeWithConvoy(UPerson * _person, int invID, int conID, int _team)
{
	FCharacterData charDat = GetCharacterDataByPerson(_person, _team);;
	if (conID >= 0)
	{
		if (invID < charDat.inventory.Num())
		{
			UItemInstance* trd = charDat.inventory[invID];
			UItemInstance* cnvTrd = (teams[_team]).convoy[conID];

			charDat.inventory.Remove(trd);
			TArray<UItemInstance*> insertIts;
			insertIts.Add(cnvTrd);

			charDat.inventory.Insert(insertIts, invID);
			(teams[_team]).convoy.Remove(cnvTrd);
			AddItemToConvoy(trd, _team);

			TArray<FCharacterData> chars;
			chars.Add(charDat);
			UpdateCharacterData(chars, _team);
			return true;
		}
		else
		{
			UItemInstance* cnvTrd = (teams[_team]).convoy[conID];
			(teams[_team]).convoy.Remove(cnvTrd);
			charDat.inventory.Add(cnvTrd);

			TArray<FCharacterData> chars;
			chars.Add(charDat);
			UpdateCharacterData(chars, _team);
			return true;
		}
	}
	else
	{
		if (invID < charDat.inventory.Num())
		{
			UItemInstance* trd = charDat.inventory[invID];
			AddItemToConvoy(trd, _team);
			charDat.inventory.Remove(trd);

			TArray<FCharacterData> chars;
			chars.Add(charDat);
			UpdateCharacterData(chars, _team);
			return true;
		}
	}
	return false;
}

void AGameSlotData::AddFreshItemToConvoy(UItemData * _itemData, int _team)
{
	CheckTeam(_team);
	if (_itemData != nullptr)
	{
		UItemInstance* it = NewObject<UItemInstance>();
		it->item = _itemData;
		it->durability = _itemData->durability;
		if (_team < teams.Num())
		{
			teams[_team].convoy.Add(it);
		}
		else
		{
			teams[0].convoy.Add(it);
		}
	}
}

void AGameSlotData::AddGold(int value, int _team)
{
	if (value >= 0)
	{
		teams[_team].gold += value;
		if (teams[_team].gold > 999999999)
		{
			teams[_team].gold = 999999999;
		}
	}
}

int AGameSlotData::GetGold(int _team)
{
	return teams[_team].gold;
}

void AGameSlotData::SpendGold(int value, int _team)
{
	if (value >= 0)
	{
		teams[_team].gold -= value;
		if (teams[_team].gold < 0)
		{
			teams[_team].gold = 0;
		}
	}
}

bool AGameSlotData::HasGold(int value, int _team)
{
	if (teams[_team].gold >= value)
	{
		return true;
	}
	return false;
}

void AGameSlotData::CheckTeam(int _team)
{
	if (_team >= teams.Num())
	{
		for (int i = 0; i < _team - (teams.Num()-1); i++)
		{
			teams.Add(FTeamConvoyInstance());
		}
	}
}

void AGameSlotData::CombineTeam(int _teamA, int _teamB)
{
	if (teams.Num() > _teamA && teams.Num() > _teamB)
	{
		teams[_teamA].gold += teams[_teamB].gold;
		teams[_teamA].bonusEP += teams[_teamB].bonusEP;
		teams[_teamA].genericUnits += teams[_teamB].genericUnits;
		teams[_teamA].convoy.Append(teams[_teamB].convoy);
		teams[_teamA].abilityScrolls.Append(teams[_teamB].abilityScrolls);
		teams[_teamA].characters.Append(teams[_teamB].characters);
		teams.RemoveAt(_teamB);
	}
}

