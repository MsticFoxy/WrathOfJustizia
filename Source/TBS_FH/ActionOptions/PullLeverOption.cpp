// Fill out your copyright notice in the Description page of Project Settings.


#include "PullLeverOption.h"
#include "Unit.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/Canto.h"

FName UPullLeverOption::GetActionName()
{
	return "Hebel";
}

bool UPullLeverOption::MeetsRequirements(AUnit* _unit)
{
	if (_unit != nullptr)
	{
		if (_unit->state != EUnitState::ECantoEnd)
		{
			TArray<UNavTile*> neigh = _unit->tileMap->GetNeighborsOfTle(_unit->currentTile);

			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(_unit->GetWorld(), ALeaver::StaticClass(), FoundActors);
			ALeaver* lv;

			for (int t = 0; t < FoundActors.Num(); t++)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("in"));
				if (dynamic_cast<ALeaver*>(FoundActors[t]))
				{
					lv = dynamic_cast<ALeaver*>(FoundActors[t]);
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("a"));
					if (lv != nullptr && !lv->pulled)
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("found"));
						for (int i = 0; i < neigh.Num(); i++)
						{
							if (neigh[i] == lv->tile)
							{
								leaver = lv;
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

void UPullLeverOption::Run(AUnit* _unit)
{
	if (_unit != nullptr)
	{
		if (leaver != nullptr)
		{
			leaver->Pull();
		}
		if (_unit->GetAbilities().Contains(UCanto::StaticClass()))
		{
			_unit->state = EUnitState::ECanto;
		}
		else
		{
			_unit->state = EUnitState::EWaiting;
		}
	}
}