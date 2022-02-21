// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NavTile.h"
#include "TileData.h"
#include "stdlib.h"
#include <vector>
#include "Engine/Engine.h"
#include "UnrealMathUtility.h"
#include "UObject/UObjectGlobals.h"
#include "TileMap.generated.h"


USTRUCT()
struct FTileRow
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		TArray<UNavTile*> row;
};
/**
 * 
 */
UCLASS()
class TBS_FH_API UTileMap : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		TArray<FTileRow> tiles;

public:
	UTileMap()
	{
		
	}
	~UTileMap()
	{
		//free(&tiles);
	}

	void init()
	{
		int x = 1;
		int y = 1;

		for (int dx = 0; dx < x; dx++)
		{
			FTileRow row;
			for (int dy = 0; dy < y; dy++)
			{
				UNavTile* nt = NewObject<UNavTile>(this, UNavTile::StaticClass());
				nt->init(dx, dy);
				row.row.Add(nt);
			}
			tiles.Add(row);
		}
	}

	void init(const int x, const int y)
	{
		for (int dx = 0; dx < x; dx++)
		{
			FTileRow row;
			for (int dy = 0; dy < y; dy++)
			{
				UNavTile* nt = NewObject<UNavTile>(this, UNavTile::StaticClass());
				nt->init(dx, dy);

				for (int i = 0; i < x*y; i++)
				{
					nt->tempWeights.Add(0);
				}

				row.row.Add(nt);
			}
			tiles.Add(row);
		}
	}

	UNavTile* Get(int x, int y)
	{
		/*x = FMath::Clamp<int>(x, 0, tiles.size()-1);
		y = FMath::Clamp<int>(y, 0, tiles[x].size()-1);*/

		if (x < tiles.Num() && x >= 0 && y < tiles[x].row.Num() && y >= 0)
		{
			return (tiles[x].row[y]);
		}
		
		return nullptr;
	}

	UNavTile* GetClamped(int x, int y)
	{
		if (tiles.Num() > 0)
		{
			x = FMath::Clamp<int>(x, 0, tiles.Num() - 1);
			if (tiles[x].row.Num() > 0)
			{
				y = FMath::Clamp<int>(y, 0, tiles[x].row.Num() - 1);
				if (x < tiles.Num() && x >= 0 && y < tiles[x].row.Num() && y >= 0)
				{
					return (tiles[x].row[y]);
				}
			}
		}

		return nullptr;
	}

	void SetWeightOfAll(int _weight)
	{
		for (int dx = 0; dx < tiles.Num(); dx++)
		{
			for (int dy = 0; dy < tiles[dx].row.Num(); dy++)
			{
				(tiles[dx].row[dy])->currentWeight = _weight;
			}
		}
	}

	void SetTempWeightOfAll(int _weight, int id)
	{
		for (int dx = 0; dx < tiles.Num(); dx++)
		{
			for (int dy = 0; dy < tiles[dx].row.Num(); dy++)
			{
				(tiles[dx].row[dy])->tempWeights[id] = _weight;
			}
		}
	}

	TArray<UNavTile*> GetAllNodes()
	{
		TArray<UNavTile*> nodes;
		for (int dx = 0; dx < tiles.Num(); dx++)
		{
			for (int dy = 0; dy < tiles[dx].row.Num(); dy++)
			{
				nodes.Add(tiles[dx].row[dy]);
			}
		}
		return nodes;
	}

	TArray<UNavTile*> GetNeighbours(UNavTile* node)
	{
		TArray<UNavTile*> neigh;
		UNavTile* addition = Get(node->x + 1, node->y);
		if (addition != nullptr)
		{
			neigh.Add(addition);
		}
		addition = Get(node->x - 1, node->y);
		if (addition != nullptr)
		{
			neigh.Add(addition);
		}
		addition = Get(node->x, node->y + 1);
		if (addition != nullptr)
		{
			neigh.Add(addition);
		}
		addition = Get(node->x, node->y - 1);
		if (addition != nullptr)
		{
			neigh.Add(addition);
		}
		return neigh;
	}

	TArray<UNavTile*> GetNeighboursSplitInfluence(UNavTile* node)
	{
		TArray<UNavTile*> neigh;
		UNavTile* addition = Get(node->x + 1, node->y);
		if (addition != nullptr)
		{
			if (addition->splitWS_x == 0)
			{
				neigh.Add(addition);
			}
		}
		addition = Get(node->x - 1, node->y);
		if (addition != nullptr)
		{
			if (node->splitWS_x == 0)
			{
				neigh.Add(addition);
			}
		}
		addition = Get(node->x, node->y + 1);
		if (addition != nullptr)
		{
			if (node->splitWS_y == 0)
			{
				neigh.Add(addition);
			}
		}
		addition = Get(node->x, node->y - 1);
		if (addition != nullptr)
		{
			if (addition->splitWS_y == 0)
			{
				neigh.Add(addition);
			}
		}
		return neigh;
	}
};
