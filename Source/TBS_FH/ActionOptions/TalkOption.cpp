// Fill out your copyright notice in the Description page of Project Settings.


#include "TalkOption.h"


FName UTalkOption::GetActionName()
{
	return "Reden";
}

bool UTalkOption::MeetsRequirements(AUnit * _unit)
{
	if (_unit != nullptr)
	{
		if (_unit->state != EUnitState::ECantoEnd)
		{
			TArray<AUnit*> units = _unit->tileMap->GetAllNeightbors(_unit);

			for (int i = 0; i < units.Num(); i++)
			{
				for (int d = 0; d < units[i]->fieldDialogs.Num(); d++)
				{
					FDialogPossability dp = units[i]->fieldDialogs[d];
					if (dp.initiationTalkPartner.Contains(_unit->GetPerson()))
					{
						talkPartner = units[i];
						dialog = dp;
						dialogRemoveID = d;
						return true;
					}
				}
			}
		}
	}

	return false;
}

void UTalkOption::Run(AUnit * _unit)
{
	if (_unit != nullptr)
	{
		talkPartner->StartFieldDialog(dialog.dialog, dialogRemoveID);
	}
}