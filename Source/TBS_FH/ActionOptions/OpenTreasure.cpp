// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenTreasure.h"
#include "Unit.h"
#include "NavMap.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/Canto.h"

FName UOpenTreasure::GetActionName()
{
	return "Truhe";
}

bool UOpenTreasure::MeetsRequirements(AUnit* _unit)
{
	if (_unit != nullptr)
	{
		if (_unit->state != EUnitState::ECantoEnd)
		{
			TArray<UNavTile*> neigh = _unit->tileMap->GetNeighborsOfTle(_unit->currentTile);

			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(_unit->GetWorld(), ATreasure::StaticClass(), FoundActors);
			ATreasure* tres = nullptr;

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Check"));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%i"), FoundActors.Num()));

			for (int t = 0; t < FoundActors.Num(); t++)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("in"));
				if (dynamic_cast<ATreasure*>(FoundActors[t]))
				{
					tres = dynamic_cast<ATreasure*>(FoundActors[t]);
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("a"));
					if (tres != nullptr && !tres->opened)
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("found"));
						for (int i = 0; i < neigh.Num(); i++)
						{
							if (neigh[i] == tres->tile)
							{
								treasure = tres;
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

void UOpenTreasure::Run(AUnit* _unit)
{
	if (_unit != nullptr)
	{
		
		if (treasure != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OPEN"));
			UItemInstance* it = treasure->Open();
			_unit->AddItemWithNotification(it);
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