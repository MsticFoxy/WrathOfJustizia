// Fill out your copyright notice in the Description page of Project Settings.


#include "NavMap.h"
#include "Engine/Engine.h"
#include "LogMacros.h"
#include "Engine/StaticMesh.h"
#include "CBluePrints.h"
#include <string>
#include "PlatformFilemanager.h"
#include "FileHelper.h"
#include "Paths.h"
#include "DefaultValueHelper.h"
#include "Unit.h"
#include "CharacterDataSet.h"
#include "StaffData.h"
#include "Staffs/HealingStaffData.h"
#include "TileData.h"
#include "GameplayTagContainer.h"

// Sets default values
ANavMap::ANavMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	gridMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GridMesh"));
	movementMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MovementMesh"));
	enemyRangeMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("EnemyRangeMesh"));
	pathMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("pathMesh"));
	selectedTile = nullptr;
}

// Called when the game starts or when spawned
void ANavMap::BeginPlay()
{
	Super::BeginPlay();
	path = FPaths::ProjectDir() + "Content/Maps/NavMapData/" + mapSaveName + "_Save.NavMap";

	
	if (tileMap == nullptr)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, path);
		tileMap = NewObject<UTileMap>(this, UTileMap::StaticClass());
		tileMap->init(sizeX, sizeY);

		for (int x = 0; x < sizeX; x++)
		{
			for (int y = 0; y < sizeY; y++)
			{
				tileMap->Get(x, y)->tileData = initTileData;
			}
		}
	
		LoadMap();

		TArray<UNavTile*> noTiles;
		BuildTempWeights(noTiles);
	}

	CreateGridMesh();
	if (editMode)
	{
		gridMesh->SetMaterial(0, editGridMaterial);
	}
	else
	{
		gridMesh->SetMaterial(0, gridMaterial);
	}
	gridMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	movementMesh->SetMaterial(0, movementMaterial);
	movementMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	enemyRangeMesh->SetMaterial(0, enemyRangeMaterial);
	enemyRangeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	pathMesh->SetMaterial(0, movementPathMaterial);
	pathMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	UpdateGridMeshVertexColor();
	UpdateEnemyRangeMeshVertexColor();
	ClearMovementMesh();
}

// Called every frame
void ANavMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//tile = tileMap->Get(1, 1);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(tile->height));
	movementMesh->SetRelativeLocation(FVector(0, 0, -1));
	enemyRangeMesh->SetRelativeLocation(FVector(0, 0, 2));
	pathMesh->SetRelativeLocation(FVector(0, 0, 1));

	if (finishedBuildOfTempWeights)
	{
		EndBuildingTempWeights();
		finishedBuildOfTempWeights = false;
	}
}

/*std::vector<UNavTile*> ANavMap::GetPath(UNavTile * start, UNavTile * end, ETraverseType traverseType)
{
	return std::vector<UNavTile*>();
}*/

UTileMap * ANavMap::GetWeightedMap(UNavTile * start, UNavTile * end, ETraverseType traverseType)
{
	return nullptr;
}

FVector ANavMap::GetTracedVertex(float x, float y, FVector2D offset)
{
	float ox = x + offset.X;
	float oy = y + offset.Y;

	x = (x * 0.99f) + offset.X;
	y = (y * 0.99f) + offset.Y;

	FHitResult Hit(ForceInit);

	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(Hit, FVector(x, y, 10000), FVector(x, y, -10000), ECC_WorldStatic, CollisionParams))
	{
		return FVector(ox,oy, Hit.Location.Z + 10);
	}

	return FVector(ox, oy, 10);
}

TArray<FVector> ANavMap::GridTileVerts(FVector2D offset)
{
	offset = offset * 100;
	TArray<FVector> vertices;

	vertices.Add(GetTracedVertex(-50, -50, offset));
	vertices.Add(GetTracedVertex(-50, 50, offset));
	vertices.Add(GetTracedVertex(50, -50, offset));
	vertices.Add(GetTracedVertex(50, 50, offset));
	return vertices;
}

void ANavMap::CreateGridMesh() 
{
	struct ILocal
	{
		static TArray<int> GridTileTris(int offset)
		{
			TArray<int> tris;
			tris.Add(0 + offset);
			tris.Add(1 + offset);
			tris.Add(2 + offset);
			tris.Add(1 + offset);
			tris.Add(3 + offset);
			tris.Add(2 + offset);
			return tris;
		}
		static TArray<FVector2D> GridTileUV()
		{
			TArray<FVector2D> uv;
			uv.Add(FVector2D(1, 1));
			uv.Add(FVector2D(1, 0));
			uv.Add(FVector2D(0, 1));
			uv.Add(FVector2D(0, 0));
			return uv;
		}
		static TArray<FVector> GridColorData(UNavTile* _tile)
		{
			TArray<FVector> color;

			

			if (_tile != nullptr)
			{
				float r = ((float)_tile->height) / 10.0f;
				float g = 0;
				float b = 0;

				if (_tile->tileData != nullptr)
				{
					if (!TEST_BIT(_tile->tileData->traverseType, ETraverseType::TT_Walking) && !TEST_BIT(_tile->tileData->traverseType, ETraverseType::TT_Walking))
					{
						g = 1;
					}
				}

				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
			}
			else
			{
				float r = 0;
				float g = 0;
				float b = 0;

				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
			}

			return color;
		}
	};

	TArray<FVector> verts;
	TArray<int> tris;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	int quadNum = 0;
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			verts.Append(GridTileVerts(FVector2D(x + startX, y + startY)));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(tris.Num()));

			tris.Append(ILocal::GridTileTris(4 * quadNum));
			quadNum++;
			uvs.Append(ILocal::GridTileUV());
			vertexColors.Append(ILocal::GridColorData(tileMap->Get(x,y)));
		}
	}

	gridMesh->CreateMeshSection_LinearColor(0, verts, tris, normals, uvs, vertexColors, tangents, true);
	movementMesh->CreateMeshSection_LinearColor(0, verts, tris, normals, uvs, vertexColors, tangents, true);
	enemyRangeMesh->CreateMeshSection_LinearColor(0, verts, tris, normals, uvs, vertexColors, tangents, true);
	
}

void ANavMap::UpdateGridMeshVertexColor()
{
	struct ILocal
	{
		static TArray<int> GridTileTris(int offset)
		{
			TArray<int> tris;
			tris.Add(0 + offset);
			tris.Add(1 + offset);
			tris.Add(2 + offset);
			tris.Add(1 + offset);
			tris.Add(3 + offset);
			tris.Add(2 + offset);
			return tris;
		}
		static TArray<FVector2D> GridTileUV(bool _edit, UNavTile* tl)
		{
			TArray<FVector2D> uv;
			if (_edit)
			{
				float xa = (float)tl->splitWS_x * 1;
				float ya = (float)tl->splitWS_y * 1;
				uv.Add(FVector2D(1+xa, 1+ya));
				uv.Add(FVector2D(1+xa, 0+ya));
				uv.Add(FVector2D(0+xa, 1+ya));
				uv.Add(FVector2D(0+xa, 0+ya));
			}
			else
			{
				uv.Add(FVector2D(1, 1));
				uv.Add(FVector2D(1, 0));
				uv.Add(FVector2D(0, 1));
				uv.Add(FVector2D(0, 0));
			}
			return uv;
		}
		static TArray<FVector> GridColorData(UNavTile* _tile, UNavTile* _selected, TArray<UNavTile*> special, bool EMode, UTileData* _selectedData)
		{
			TArray<FVector> color;



			if (_tile != nullptr)
			{
				float r = 0;
				if (EMode)
				{
					r = ((float)_tile->height) / 10.0f;
				}
				float g = 0;
				float b = 0;

				/*if (_tile->tileData != nullptr)
				{
					if (!TEST_BIT(_tile->tileData->traverseType, ETraverseType::TT_Walking) && !TEST_BIT(_tile->tileData->traverseType, ETraverseType::TT_Swimming))
					{
						
						g = 1;
						
					}
				}*/
				if (!EMode && special.Contains(_tile))
				{
					r = 1;
				}
				/*else
				{
					r = 1;
				}*/

				if (!EMode)
				{
					if (_selected != nullptr && _tile == _selected)
					{
						//b = 1;
					}
				}
				else
				{
					if (_selectedData != nullptr && _tile->tileData != nullptr)
					{
						if (_tile->tileData == _selectedData)
						{
							b = 1;
						}
					}
				}
				
				if (_tile->tileData->show)
				{
					g = 0;
				}
				else
				{
					g = 1;
				}

				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
			}
			else
			{
				float r = 0;
				float g = 0;
				float b = 0;

				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
			}

			return color;
		}
	};

	TArray<FVector> verts;
	TArray<int> tris;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	int quadNum = 0;
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			verts.Append(GridTileVerts(FVector2D(x + startX, y + startY)));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(tris.Num()));

			tris.Append(ILocal::GridTileTris(4 * quadNum));
			quadNum++;
			uvs.Append(ILocal::GridTileUV(editMode, tileMap->Get(x, y)));
			vertexColors.Append(ILocal::GridColorData(tileMap->Get(x, y), selectedTile, specialEventTiles, editMode, _selectedTileData));
		}
	}
	gridMesh->UpdateMeshSection_LinearColor(0, verts, normals, uvs, vertexColors, tangents);
}

void ANavMap::UpdateMovementMeshVertexColor(int movement, TArray<FIntervalRange> _range, TArray<FIntervalRange> _hl_range)
{
	struct ILocal
	{
		static TArray<int> GridTileTris(int offset)
		{
			TArray<int> tris;
			tris.Add(0 + offset);
			tris.Add(1 + offset);
			tris.Add(2 + offset);
			tris.Add(1 + offset);
			tris.Add(3 + offset);
			tris.Add(2 + offset);
			return tris;
		}
		static TArray<FVector2D> GridTileUV()
		{
			TArray<FVector2D> uv;
			uv.Add(FVector2D(1, 1));
			uv.Add(FVector2D(1, 0));
			uv.Add(FVector2D(0, 1));
			uv.Add(FVector2D(0, 0));
			return uv;
		}
		static TArray<FVector> GridColorData(UNavTile* _tile, UTileMap* _tMap, int _move, TArray<FIntervalRange> _rng, TArray<FIntervalRange> _hl_rng, ANavMap* _nMap, UEngine* engine)
		{
			TArray<FVector> color;

			if (_tile != nullptr)
			{
				float r = 0.0f;
				float g = 0.0f;
				float b = 0.0f;

				
				if (_tile->currentWeight <= _move)
				{
					r = 1;
					if (!_tile->tileData->restable)
					{
						b = 0.6f;
					}
				}

				if (_tile->tileData->restable)
				{
					int stf_id = _rng.Num();
					TArray<FIntervalRange> rngs = _rng;
					rngs.Append(_hl_rng);
					int maxRange = 0;
					for (int i = 0; i < rngs.Num(); i++)
					{
						if (rngs[i].Max > maxRange)
						{
							maxRange = rngs[i].Max;
						}
					}
					
					bool staff_inRange = false;
					bool inRange = false;
					for (int x = 0; FMath::Abs(x) <= maxRange && !inRange; x+=0)
					{
						for (int y = 0; FMath::Abs(y) <= maxRange && !inRange; y+=0)
						{
							for (int i = 0; i < rngs.Num() && !inRange; i++)
							{
								int dist = FMath::Abs(x) + FMath::Abs(y);
								if (rngs[i].Min <= dist && dist <= rngs[i].Max )
								{
									UNavTile* nt = _tMap->Get(_tile->x + x, _tile->y + y);
									if (nt != nullptr && nt->tileData != nullptr && nt->tileData->restable)
									{
										if (nt->currentWeight <= _move)
										{
											if (nt->currentWeight == 0 || nt->unit == nullptr)
											{
												/*if (dist <= 1)
												{
													if (i < stf_id)
													{
														inRange = true;
													}
													else
													{
														staff_inRange = true;
													}
												}
												else
												{*/
													if (_nMap->GetTempWeightOfTile(nt, _tile) <= rngs[i].Max)
													{
														if (i < stf_id)
														{
															inRange = true;
														}
														else
														{
															staff_inRange = true;
														}
													}
												//}
												//inRange = true;
											}
										}
									}
								}
							}

							if (y == 0)
							{
								y = 1;
							}
							else if (y > 0)
							{
								y = -y;
							}
							else if (y < 0)
							{
								y = -y;
								y++;
							}
						}
						if (x == 0)
						{
							x = 1;
						}
						else if (x > 0)
						{
							x = -x;
						}
						else if (x < 0)
						{
							x = -x;
							x++;
						}
					}
					if (staff_inRange)
					{
						g = 0.6f;
					}
					if (inRange)
					{
						g = 1.0f;
					}
					
				}


				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
			}
			else
			{
				float r = 0;
				float g = 0;
				float b = 0;

				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
			}

			return color;
		}
	};

	TArray<FVector> verts;
	TArray<int> tris;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	int quadNum = 0;
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			verts.Append(GridTileVerts(FVector2D(x + startX, y + startY)));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(tris.Num()));

			tris.Append(ILocal::GridTileTris(4 * quadNum));
			quadNum++;
			uvs.Append(ILocal::GridTileUV());
			vertexColors.Append(ILocal::GridColorData(tileMap->Get(x, y), tileMap, movement, _range, _hl_range, this, GEngine));
		}
	}
	movementMesh->UpdateMeshSection_LinearColor(0, verts, normals, uvs, vertexColors, tangents);
}

void ANavMap::HighlightUnitMeshVertexColor(TArray<AUnit*> _units)
{
	struct ILocal
	{
		static TArray<int> GridTileTris(int offset)
		{
			TArray<int> tris;
			tris.Add(0 + offset);
			tris.Add(1 + offset);
			tris.Add(2 + offset);
			tris.Add(1 + offset);
			tris.Add(3 + offset);
			tris.Add(2 + offset);
			return tris;
		}
		static TArray<FVector2D> GridTileUV()
		{
			TArray<FVector2D> uv;
			uv.Add(FVector2D(1, 1));
			uv.Add(FVector2D(1, 0));
			uv.Add(FVector2D(0, 1));
			uv.Add(FVector2D(0, 0));
			return uv;
		}
		static TArray<FVector> GridColorData(TArray<AUnit*> _units, UNavTile* _tile)
		{
			TArray<FVector> color;

			float r = 0;
			float g = 0;
			float b = 0;

			if (_units.Contains(_tile->unit))
			{
				g = 1;
			}

			color.Add(FVector(r, g, b));
			color.Add(FVector(r, g, b));
			color.Add(FVector(r, g, b));
			color.Add(FVector(r, g, b));

			return color;
		}
	};

	TArray<FVector> verts;
	TArray<int> tris;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	int quadNum = 0;
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			verts.Append(GridTileVerts(FVector2D(x + startX, y + startY)));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(tris.Num()));

			tris.Append(ILocal::GridTileTris(4 * quadNum));
			quadNum++;
			uvs.Append(ILocal::GridTileUV());
			vertexColors.Append(ILocal::GridColorData(_units, tileMap->Get(x, y)));
		}
	}
	movementMesh->UpdateMeshSection_LinearColor(0, verts, normals, uvs, vertexColors, tangents);
}

void ANavMap::CreatePathMesh(TArray<UNavTile*> _path)
{
	struct ILocal
	{
		enum EDir
		{
			up,down,left,right
		};
		static TArray<int> GridTileTris(int offset)
		{
			TArray<int> tris;
			tris.Add(0 + offset);
			tris.Add(1 + offset);
			tris.Add(2 + offset);
			tris.Add(1 + offset);
			tris.Add(3 + offset);
			tris.Add(2 + offset);
			return tris;
		}
		static TArray<FVector2D> GridTileUV(EDir direction)
		{
			TArray<FVector2D> uv;
			switch (direction)
			{
			case ILocal::up:
				uv.Add(FVector2D(1, 1));
				uv.Add(FVector2D(1, 0));
				uv.Add(FVector2D(0, 1));
				uv.Add(FVector2D(0, 0));
				break;
			case ILocal::down:
				uv.Add(FVector2D(0, 0));
				uv.Add(FVector2D(0, 1));
				uv.Add(FVector2D(1, 0));
				uv.Add(FVector2D(1, 1));
				break;
			case ILocal::left:
				uv.Add(FVector2D(1, 0));
				uv.Add(FVector2D(0, 0));
				uv.Add(FVector2D(1, 1));
				uv.Add(FVector2D(0, 1));
				break;
			case ILocal::right:
				uv.Add(FVector2D(0, 1));
				uv.Add(FVector2D(1, 1));
				uv.Add(FVector2D(0, 0));
				uv.Add(FVector2D(1, 0));
				break;
			default:
				break;
			}
			
			return uv;
		}
		static TArray<FVector> GridColorData(EDir from, EDir to, bool last)
		{
			TArray<FVector> color;

			float r = 0;
			float g = 0;
			float b = 0;

			if (to == EDir::up)
			{
				if (from == right)
				{
					r = 1;
				}
				else if (from == left)
				{
					g = 1;
				}
			}

			if (to == EDir::right)
			{
				if (from == down)
				{
					r = 1;
				}
				else if (from == up)
				{
					g = 1;
				}
			}

			if (to == EDir::down)
			{
				if (from == left)
				{
					r = 1;
				}
				else if (from == right)
				{
					g = 1;
				}
			}

			if (to == EDir::left)
			{
				if (from == up)
				{
					r = 1;
				}
				else if (from == down)
				{
					g = 1;
				}
			}

			if (last)
			{
				b = 1;
			}

			color.Add(FVector(r, g, b));
			color.Add(FVector(r, g, b));
			color.Add(FVector(r, g, b));
			color.Add(FVector(r, g, b));

			return color;
		}

		static EDir GetDirection(UNavTile* start, UNavTile* next)
		{
			int x = next->x - start->x;
			int y = next->y - start->y;

			if (x < 0)
			{
				return EDir::left;
			}
			if (x > 0)
			{
				return EDir::right;
			}
			if (y < 0)
			{
				return EDir::down;
			}
			if (y > 0)
			{
				return EDir::up;
			}
			return EDir::up;
		}
	};

	TArray<FVector> verts;
	TArray<int> tris;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	int quadNum = 0;
	for (int x = 1; x < _path.Num(); x++)
	{
		verts.Append(GridTileVerts(FVector2D(_path[x]->x + startX, _path[x]->y + startY)));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(tris.Num()));

		tris.Append(ILocal::GridTileTris(4 * quadNum));
		quadNum++;

		
		ILocal::EDir fromDir;
		ILocal::EDir toDir;
		if (x == 0)
		{
			fromDir = ILocal::GetDirection(_path[0], _path[1]);
			toDir = ILocal::GetDirection(_path[x], _path[x+1]);
		}
		else if (x == _path.Num()-1)
		{
			fromDir = ILocal::GetDirection(_path[x - 1], _path[x]);
			toDir = ILocal::GetDirection(_path[x-1], _path[x]);
		}
		else
		{
			fromDir = ILocal::GetDirection(_path[x - 1], _path[x]);
			toDir = ILocal::GetDirection(_path[x], _path[x + 1]);
		}

		bool last = false;

		if (x == _path.Num() - 1)
		{
			last = true;
		}

		uvs.Append(ILocal::GridTileUV(toDir));
		vertexColors.Append(ILocal::GridColorData(fromDir, toDir, last));
	}

	pathMesh->CreateMeshSection_LinearColor(0, verts, tris, normals, uvs, vertexColors, tangents, true);
}

void ANavMap::AddEnemyRangeUnit(AUnit * _unit)
{
	if (!EnemyRangeContainsUnit(_unit))
	{
		enemyRanges.Add(FEnemyRangeSet());
		enemyRanges[enemyRanges.Num() - 1]._unit = _unit;
		_unit->WeightMapFromHere();
		enemyRanges[enemyRanges.Num() - 1].ranges = GetReachableAttackNodes(_unit);
		enemyRanges[enemyRanges.Num() - 1].movement = GetReachableNodes(_unit);
		(enemyRanges[enemyRanges.Num() - 1].movement).Add(_unit->currentTile);
		ClearMovementMesh();
		
	}
}

void ANavMap::NotifyEnemyRangeTiles(TArray<UNavTile*> _tiles)
{
	TArray<int> rangeIDs;
	for (int i = 0; i < enemyRanges.Num(); i++)
	{
		bool stop = false;
		for (int k = 0; k < _tiles.Num() && !stop; k++)
		{
			if (enemyRanges[i].movement.Contains(_tiles[k]) || enemyRanges[i].ranges.Contains(_tiles[k]))
			{
				stop = true;
				if (!rangeIDs.Contains(i))
				{
					rangeIDs.Add(i);
				}
				
			}
		}
	}

	for (int i = 0; i < rangeIDs.Num(); i++)
	{
		enemyRanges[rangeIDs[i]]._unit->WeightMapFromHere();
		enemyRanges[rangeIDs[i]].movement = GetReachableNodes(enemyRanges[rangeIDs[i]]._unit);
		enemyRanges[rangeIDs[i]].ranges = GetReachableAttackNodes(enemyRanges[rangeIDs[i]]._unit);
	}

	ClearMovementMesh();
}

void ANavMap::NotifyEnemyRangeUnit(AUnit * _unit)
{
	int id = -1;
	for (int i = 0; i < enemyRanges.Num(); i++)
	{
		if (enemyRanges[i]._unit == _unit)
		{
			id = i;
			break;
		}
	}
	if (id >= 0)
	{
		enemyRanges[id]._unit->WeightMapFromHere();
		enemyRanges[id].movement = GetReachableNodes(_unit);
		enemyRanges[id].ranges = GetReachableAttackNodes(_unit);
	}
}

bool ANavMap::EnemyRangeContainsUnit(AUnit * _unit)
{
	int id = -1;
	for (int i = 0; i < enemyRanges.Num(); i++)
	{
		if (enemyRanges[i]._unit == _unit)
		{
			return true;
		}
	}
	return false;
}

void ANavMap::RemoveEnemyRangeUnit(AUnit * _unit)
{
	int id = -1;
	for (int i = 0; i < enemyRanges.Num(); i++)
	{
		if (enemyRanges[i]._unit == _unit)
		{
			id = i;
			break;
		}
	}
	if (id >= 0)
	{
		enemyRanges.RemoveAt(id);
	}
}

void ANavMap::UpdateEnemyRangeMeshVertexColor()
{
	struct ILocal
	{
		static TArray<int> GridTileTris(int offset)
		{
			TArray<int> tris;
			tris.Add(0 + offset);
			tris.Add(1 + offset);
			tris.Add(2 + offset);
			tris.Add(1 + offset);
			tris.Add(3 + offset);
			tris.Add(2 + offset);
			return tris;
		}
		static TArray<FVector2D> GridTileUV(bool _neigh[4])
		{
			TArray<FVector2D> uv;
			float n = 0;
			if (_neigh[0]) { n = 0.025f; }

			float e = 0;
			if (_neigh[1]) { e = 0.025f; }

			float s = 0;
			if (_neigh[2]) { s = -0.025f; }

			float w = 0;
			if (_neigh[3]) { w = -0.025f; }

			float ym = 1;
			if (!_neigh[0] && !_neigh[2]) { ym = 2; }

			float xm = 1;
			if (!_neigh[1] && !_neigh[3]) { xm = 2; }

			n *= ym;
			s *= ym;
			e *= xm;
			w *= xm;

			uv.Add(FVector2D(0.75f+e, 0.75f+n));
			uv.Add(FVector2D(0.75f+e, 0.25f+s));
			uv.Add(FVector2D(0.25f+w, 0.75f+n));
			uv.Add(FVector2D(0.25f+w, 0.25f+s));
			return uv;
		}
		static TArray<FVector2D> GridTileUV2(bool _dneigh[4])
		{
			TArray<FVector2D> uv;

			float n = 0;
			if (_dneigh[0]) { n = 0.025f; }

			float e = 0;
			if (_dneigh[1]) { e = 0.025f; }

			float s = 0;
			if (_dneigh[2]) { s = 0.025f; }

			float w = 0;
			if (_dneigh[3]) { w = 0.025f; }

			float ym = 1;
			if (!_dneigh[0] && !_dneigh[2]) { ym = 2; }

			float xm = 1;
			if (!_dneigh[1] && !_dneigh[3]) { xm = 2; }

			n *= ym;
			s *= ym;
			e *= xm;
			w *= xm;

			uv.Add(FVector2D(0.75f+n, 0.75f+n));
			uv.Add(FVector2D(0.75f+e, 0.25f-e));
			uv.Add(FVector2D(0.25f-w, 0.75f+w));
			uv.Add(FVector2D(0.25f-s, 0.25f-s));
			return uv;
		}
		static TArray<FVector> GridColorData(UNavTile* _tile, TArray<UNavTile*> _ranges)
		{
			TArray<FVector> color;

			if (_tile != nullptr)
			{
				float r = 0;
				float g = 0;
				float b = 0;

				if (_ranges.Contains(_tile))
				{
					r = 1;
				}

				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
			}
			else
			{
				float r = 0;
				float g = 0;
				float b = 0;

				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
				color.Add(FVector(r, g, b));
			}

			return color;
		}
	};

	TArray<FVector> verts;
	TArray<int> tris;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FVector2D> uvs2;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	TArray<UNavTile*> ranges;

	for (int i = 0; i < enemyRanges.Num(); i++)
	{
		if (enemyRanges[i]._unit->IsAlive())
		{
			TArray<UNavTile*> _rng = enemyRanges[i].ranges;
			for (int k = 0; k < _rng.Num(); k++)
			{
				ranges.AddUnique(_rng[k]);
			}
		}
	}

	int quadNum = 0;
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			verts.Append(GridTileVerts(FVector2D(x + startX, y + startY)));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(tris.Num()));

			tris.Append(ILocal::GridTileTris(4 * quadNum));
			quadNum++;

			bool neigh[4] = {false,false,false,false};
			if (ranges.Contains(tileMap->Get(x, y)))
			{
				if (tileMap->Get(x, y - 1) == nullptr || !ranges.Contains(tileMap->Get(x, y - 1)))
				{
					neigh[0] = true;
				}
				if (tileMap->Get(x - 1, y) == nullptr || !ranges.Contains(tileMap->Get(x - 1, y)))
				{
					neigh[1] = true;
				}
				if (tileMap->Get(x, y + 1) == nullptr || !ranges.Contains(tileMap->Get(x, y + 1)))
				{
					neigh[2] = true;
				}
				if (tileMap->Get(x + 1, y) == nullptr || !ranges.Contains(tileMap->Get(x + 1, y)))
				{
					neigh[3] = true;
				}
			}

			bool dneigh[4] = { false,false,false,false };
			if (ranges.Contains(tileMap->Get(x, y)))
			{
				if (tileMap->Get(x-1, y - 1) == nullptr || !ranges.Contains(tileMap->Get(x-1, y - 1)))
				{
					dneigh[0] = true;
				}
				if (tileMap->Get(x - 1, y+1) == nullptr || !ranges.Contains(tileMap->Get(x - 1, y+1)))
				{
					dneigh[1] = true;
				}
				if (tileMap->Get(x+1, y + 1) == nullptr || !ranges.Contains(tileMap->Get(x+1, y + 1)))
				{
					dneigh[2] = true;
				}
				if (tileMap->Get(x + 1, y-1) == nullptr || !ranges.Contains(tileMap->Get(x + 1, y-1)))
				{
					dneigh[3] = true;
				}
			}

			uvs.Append(ILocal::GridTileUV(neigh));
			uvs2.Append(ILocal::GridTileUV2(dneigh));
			vertexColors.Append(ILocal::GridColorData(tileMap->Get(x, y), ranges));
		}
	}
	enemyRangeMesh->UpdateMeshSection_LinearColor(0, verts, normals, uvs, uvs2, uvs2, uvs2, vertexColors, tangents);
}

UNavTile * ANavMap::GetNearestRestableTile(UNavTile * _tile)
{
	UNavTile* ret = nullptr;
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			UNavTile* cur = tileMap->Get(x,y);
			if (cur != _tile && cur->unit == nullptr && cur->tileData->restable)
			{
				if (ret == nullptr || ret->currentWeight > cur->currentWeight)
				{
					ret = cur;
				}
			}
		}
	}
	return ret;
}

UNavTile * ANavMap::GetTileAtLocation(FVector loc)
{
	int x = FMath::RoundToInt(loc.X / 100.0f) - startX;
	int y = FMath::RoundToInt(loc.Y / 100.0f) - startY;

	return tileMap->Get(x,y);
}

FVector ANavMap::GetLocationOfTile(UNavTile * _tile)
{
	FVector loc;

	bool stop = false;


	for (int x = 0; x < sizeX && !stop; x++)
	{
		for (int y = 0; y < sizeY && !stop; y++)
		{
			if (tileMap->Get(x, y) == _tile)
			{
				loc = FVector(startX + x, startY + y, 0) * 100;
				stop = true;
			}
		}
	}


	FHitResult Hit(ForceInit);

	FCollisionQueryParams CollisionParams;

	if (GetWorld()->SweepSingleByChannel(Hit, loc + FVector(0, 0, 10000), loc + FVector(0, 0, -10000), FQuat(), ECC_WorldStatic, FCollisionShape::MakeSphere(5),  CollisionParams))
	{
		return FVector(loc.X, loc.Y, Hit.Location.Z);
	}

	return FVector();
}

TArray<UNavTile*> ANavMap::GetReachableNodes(AUnit * _unit)
{
	TArray<UNavTile*> ret;

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			if (tileMap->Get(x, y)->currentWeight <= _unit->GetMovement() && tileMap->Get(x, y) != _unit->currentTile)
			{
				if (tileMap->Get(x, y)->tileData->restable && tileMap->Get(x, y)->unit == nullptr)
				{
					ret.Add(tileMap->Get(x, y));
				}
			}
		}
	}
	return ret;
}

TArray<UNavTile*> ANavMap::GetReachableNodesWithIgnore(AUnit * _unit, AUnit* _ignore)
{
	TArray<UNavTile*> ret;

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			if (tileMap->Get(x, y)->currentWeight <= _unit->GetMovement() )
			{
				if (tileMap->Get(x, y)->tileData->restable && (tileMap->Get(x, y)->unit == nullptr || tileMap->Get(x, y)->unit == _ignore))
				{
					ret.Add(tileMap->Get(x, y));
				}
			}
		}
	}
	return ret;
}

TArray<UNavTile*> ANavMap::GetReachableAttackNodes(AUnit * _unit)
{
	TArray<UNavTile*> ret;
	if (_unit != nullptr)
	{
		int maxRange = 0;
		for (int i = 0; i < _unit->GetAttackRange().Num(); i++)
		{
			if (_unit->GetAttackRange()[i].Max > maxRange)
			{
				maxRange = _unit->GetAttackRange()[i].Max;
			}
		}
		
		for (int x = 0; x < sizeX; x++)
		{
			for (int y = 0; y < sizeY; y++)
			{
				UNavTile* t = tileMap->Get(x, y);
				if (t != nullptr && t->currentWeight <= _unit->GetMovement() && t->tileData->restable)
				{
					if (t->unit == nullptr || t->unit == _unit)
					{
						for (int dx = 0; FMath::Abs(dx) <= maxRange; dx+=0)
						{
							for (int dy = 0; FMath::Abs(dy) <= maxRange; dy+=0)
							{
								for (int i = 0; i < _unit->GetAttackRange().Num(); i++)
								{
									int dist = FMath::Abs(dx) + FMath::Abs(dy);
									if (_unit->GetAttackRange()[i].Min <= dist && dist <= _unit->GetAttackRange()[i].Max)
									{
										UNavTile* nt = tileMap->Get(dx + x, dy + y);
										if (nt != nullptr && nt->tileData != nullptr && nt->tileData->restable)
										{
											if (!ret.Contains(nt))
											{
												if (dist <= 1)
												{
													ret.Add(nt);
												}
												else
												{
													if (GetTempWeightOfTile(nt, t) <= _unit->GetAttackRange()[i].Max)
													{
														ret.Add(nt);
													}
												}

												//ret.Add(nt);
											}
										}
									}
								}
								if (dy == 0)
								{
									dy = 1;
								}
								else if (dy > 0)
								{
									dy = -dy;
								}
								else if (dy < 0)
								{
									dy = -dy;
									dy++;
								}
							}
							if (dx == 0)
							{
								dx = 1;
							}
							else if (dx > 0)
							{
								dx = -dx;
							}
							else if (dx < 0)
							{
								dx = -dx;
								dx++;
							}
						}
					}
				}


				/*if (tileMap->Get(x, y)->tileData->restable)
				{
					bool range = false;
					for (int dx = -maxRange; dx <= maxRange && !range; dx++)
					{
						for (int dy = -maxRange; dy <= maxRange && !range; dy++)
						{
							for (int i = 0; i < _unit->GetAttackRange().Num(); i++)
							{
								int dist = FMath::Abs(dx) + FMath::Abs(dy);
								if (_unit->GetAttackRange()[i].Min <= dist && dist <= _unit->GetAttackRange()[i].Max)
								{
									UNavTile* nt = tileMap->Get(dx + x, dy + y);
									if (nt != nullptr && nt->tileData != nullptr && nt->tileData->restable && nt->currentWeight <= _unit->GetMovement())
									{
										ret.Add(tileMap->Get(x, y));
										range = true;
									}
								}
							}
						}
					}
				}*/
				
			}
		}
	}

	return ret;
}

TArray<AUnit*> ANavMap::GetReachableUnits(AUnit * _unit)
{
	TArray<AUnit*> ret;

	int maxRange = 0;
	for (int i = 0; i < _unit->GetAttackRange().Num(); i++)
	{
		if (_unit->GetAttackRange()[i].Max > maxRange)
		{
			maxRange = _unit->GetAttackRange()[i].Max;
		}
	}

	for (int dx = 0; dx < sizeX; dx++)
	{
		for (int dy = 0; dy < sizeY; dy++)
		{
			UNavTile* _tile = tileMap->Get(dx, dy);

			if (_tile->unit != nullptr)
			{

				bool inRange = false;
				for (int x = 0; FMath::Abs(x) <= maxRange && !inRange; x+=0)
				{
					for (int y = 0; FMath::Abs(y) <= maxRange && !inRange; y+=0)
					{
						int dist = FMath::Abs(x) + FMath::Abs(y);

						for (int i = 0; i < _unit->GetAttackRange().Num() && !inRange; i++)
						{

							if (_unit->GetAttackRange()[i].Min <= dist && dist <= _unit->GetAttackRange()[i].Max)
							{
								UNavTile* nt = tileMap->Get(_tile->x + x, _tile->y + y);

								if (nt != nullptr && nt->tileData != nullptr && nt->tileData->restable && (nt->unit == nullptr || nt->unit == _unit))
								{
									if (nt->currentWeight <= _unit->GetMovement())
									{
										
										if (!ret.Contains(_tile->unit))
										{

											if (dist <= 1)
											{
												ret.Add(_tile->unit);
												inRange = true;
											}
											else
											{
												if (GetTempWeightOfTile(nt, _tile) <= maxRange)
												{
													ret.Add(_tile->unit);
													inRange = true;
												}
											}
										}
										
									}
								}
							}
						}
						if (y == 0)
						{
							y = 1;
						}
						else if (y > 0)
						{
							y = -y;
						}
						else if (y < 0)
						{
							y = -y;
							y++;
						}
					}
					if (x == 0)
					{
						x = 1;
					}
					else if (x > 0)
					{
						x = -x;
					}
					else if (x < 0)
					{
						x = -x;
						x++;
					}
				}
			}
		}
	}

	return ret;
}

TArray<AUnit*> ANavMap::GetReachableEnemyUnits(AUnit * _unit)
{
	TArray<AUnit*> units = GetReachableUnits(_unit);
	TArray<AUnit*> ret;

	for (int i = 0; i < units.Num(); i++)
	{
		if (_unit->CanAttackUnitOfTeam(units[i]->team))
		{
			ret.Add(units[i]);
		}
	}

	return ret;
}

TArray<AUnit*> ANavMap::GetReachableAllyUnits(AUnit * _unit)
{
	TArray<AUnit*> units = GetReachableUnits(_unit);
	TArray<AUnit*> ret;

	for (int i = 0; i < units.Num(); i++)
	{
		if (!_unit->CanAttackUnitOfTeam(units[i]->team))
		{
			ret.Add(units[i]);
		}
	}

	return ret;
}

TArray<UNavTile*> ANavMap::GetLocationsToAttackUnit(AUnit* _currentUnit, AUnit * _unit)
{
	TArray<UNavTile*> ret;
	TArray<UNavTile*> reach = GetReachableNodesWithIgnore(_currentUnit, _currentUnit);
	TArray<FIntervalRange> ranges = _currentUnit->GetAttackRange();

	for (int i = 0; i < reach.Num(); i++)
	{
		int range = FMath::Abs(reach[i]->x - _unit->currentTile->x) + FMath::Abs(reach[i]->y - _unit->currentTile->y);

		
		for (int r = 0; r < ranges.Num(); r++)
		{
			if (ranges[r].Min <= range && ranges[r].Max >= range)
			{
				if (range <= 1)
				{
					ret.Add(reach[i]);
				}
				else
				{
					if (GetTempWeightOfTile(reach[i], _unit->currentTile) <= ranges[r].Max)
					{
						ret.Add(reach[i]);
					}
				}
			}
		}
	}

	return ret;
}

TArray<UNavTile*> ANavMap::GetLocationsToAttackUnitGlobal(AUnit * _currentUnit, AUnit * _unit)
{
	TArray<UNavTile*> ret;
	TArray<UNavTile*> reach;
	TArray<FIntervalRange> ranges = _currentUnit->GetAttackRange();

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			UNavTile* _tile = GetTile(x, y);

			if (!ret.Contains(_tile) && _tile->tileData != nullptr && _tile->tileData->restable && (_tile->unit == nullptr || _tile->unit == _currentUnit))
			{
				bool inRange = false;
				for (int r = 0; r < ranges.Num() && !inRange; r++)
				{
					int range = GetTempWeightOfTile(_tile, _unit->currentTile);

					if (ranges[r].Min <= range && range <= ranges[r].Max)
					{
						ret.Add(_tile);
						inRange = true;
					}
				}
			}
		}
	}

	return ret;
}

AUnit * ANavMap::GetUnitAtCursor()
{
	if (selectedTile != nullptr)
	{
		AUnit* u = selectedTile->unit;
		if (u != nullptr)
		{
			return u;
		}
	}
	return nullptr;
}

void ANavMap::TryWeightMapAtCursor()
{
	AUnit* u = GetUnitAtCursor();
	if (u != nullptr)
	{
		u->WeightMapFromHere();
	}
}


TArray<UNavTile*> ANavMap::GetNeighborsOfTle(UNavTile * _tile)
{
	return tileMap->GetNeighbours(_tile);
}

UTileData * ANavMap::GetTileDataAtLocation(FVector loc)
{
	UNavTile* nav = GetTileAtLocation(loc);
	if (nav != nullptr)
	{
		return nav->tileData;
	}
	return nullptr;
}

int ANavMap::GetTileHeightAtLocation(FVector loc)
{
	UNavTile* nav = GetTileAtLocation(loc);
	if (nav != nullptr)
	{
		return nav->height;
	}
	return 0;
}

FString ANavMap::GetTileTraverseTypeAtLocation(FVector loc)
{
	FString str = "";
	UTileData* t = GetTileDataAtLocation(loc);
	if (t != nullptr)
	{
		if (TEST_BIT(t->traverseType, (int32)ETraverseType::TT_Flying))
		{
			str += " (Fly) ";
		}
		if (TEST_BIT(t->traverseType, (int32)ETraverseType::TT_Walking))
		{
			str += " (Walk) ";
		}
		if (TEST_BIT(t->traverseType, (int32)ETraverseType::TT_Swimming))
		{
			str += " (Swim) ";
		}
	}
	else
	{
		str += "-";
	}

	return str;
}

void ANavMap::SetTileDataAtLocation(FVector loc, UTileData * data)
{
	UNavTile* nav = GetTileAtLocation(loc);
	nav->tileData = data;
}

void ANavMap::SetTileHeightAtLocation(FVector loc, int height)
{
	UNavTile* nav = GetTileAtLocation(loc);
	nav->height = height;
}

void ANavMap::SplitTile(FVector loc)
{
	struct ILocal
	{
	public:
		static void ToggleX(UNavTile* tl)
		{
			if (tl != nullptr)
			{
				if (tl->splitWS_x == 0)
				{
					tl->splitWS_x = 1;
				}
				else
				{
					tl->splitWS_x = 0;
				}
			}
		}

		static void ToggleY(UNavTile* tl)
		{
			if (tl != nullptr)
			{
				if (tl->splitWS_y == 0)
				{
					tl->splitWS_y = 1;
				}
				else
				{
					tl->splitWS_y = 0;
				}
			}
		}
	};

	UNavTile* tl = GetTileAtLocation(loc);

	FVector l = loc - GetLocationOfTile(tl);

	if (FMath::Abs(l.X) > FMath::Abs(l.Y))
	{
		if (l.X > 0)
		{
			tl = GetTileAtLocation(loc + FVector(100, 0, 0));
		}
		ILocal::ToggleX(tl);
	}
	else
	{
		if (l.Y < 0)
		{
			tl = GetTileAtLocation(loc - FVector(0, 100, 0));
		}
		ILocal::ToggleY(tl);
	}
}

void ANavMap::SaveMap()
{
	TArray<UObject*> dataObjects = UCBluePrints::LoadObjectLibrary("/Tiles", TSubclassOf<UTileData>());
	TArray<UTileData*> datas;
	for (int i = 0; i < dataObjects.Num(); i++)
	{
		if (dynamic_cast<UTileData*>(dataObjects[i]))
		{
			datas.Add(dynamic_cast<UTileData*>(dataObjects[i]));
		}
	}

	/*std::string sn = std::string(TCHAR_TO_UTF8(*mapSaveName));
	sn += "_Save";
	FILE* stream = fopen( sn.c_str() , "w");*/


	FString stream = "";

	stream += "" + FString::FromInt(sizeX) + "\n";
	stream += "" + FString::FromInt(sizeY) + "\n";

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			stream += "" + FString::FromInt(tileMap->Get(x, y)->tileData->id) + "\n";
			stream += "" + FString::FromInt(tileMap->Get(x, y)->height) + "\n";
			stream += "" + FString::FromInt(tileMap->Get(x, y)->splitWS_x) + "\n";
			stream += "" + FString::FromInt(tileMap->Get(x, y)->splitWS_y) + "\n";
		}
	}

	


	FFileHelper::SaveStringToFile(stream, *(path), FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_None);
}

void ANavMap::LoadMap()
{
	TArray<UObject*> dataObjects = UCBluePrints::LoadObjectLibrary("/Tiles", TSubclassOf<UTileData>());
	TArray<UTileData*> datas;
	for (int i = 0; i < dataObjects.Num(); i++)
	{
		if (dynamic_cast<UTileData*>(dataObjects[i]))
		{
			datas.Add(dynamic_cast<UTileData*>(dataObjects[i]));
		}
	}

	struct ILocal
	{
		static UTileData* GetTileDataByID(int id, TArray<UTileData*> tileDatas)
		{
			for (int i = 0; i < tileDatas.Num(); i++)
			{
				if (tileDatas[i]->id == id)
				{
					return tileDatas[i];
				}
			}
			return tileDatas[0];
		}
	};

	/*std::string sn = std::string(TCHAR_TO_UTF8(*mapSaveName));
	sn += "_Save";
	FILE* stream = fopen(sn.c_str(), "w");*/

	FString stream;
	FFileHelper::LoadFileToString(stream, *(path), FFileHelper::EHashOptions::None);

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	TArray<FString> lines = TArray<FString>();
	stream.ParseIntoArrayLines(lines, true);

	if (!PlatformFile.FileExists(*path))
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("File Does Not Exist: " + path));
		return;
	}

	int r_sizeX;
	FDefaultValueHelper::ParseInt(lines[0], r_sizeX);
	int r_sizeY;
	FDefaultValueHelper::ParseInt(lines[1], r_sizeY);

	int run = 0;
	if (r_sizeX == sizeX && r_sizeY == sizeY)
	{
		for (int x = 0; x < sizeX; x++)
		{
			for (int y = 0; y < sizeY; y++)
			{
				int i;
				FDefaultValueHelper::ParseInt(lines[2+run], i);

				UTileData* dt = nullptr;
				
				tileMap->Get(x, y)->tileData = ILocal::GetTileDataByID(i, datas);

				run++;

				int h;
				FDefaultValueHelper::ParseInt(lines[2 + run], h);
				tileMap->Get(x, y)->height = h;
				run++;

				int spx;
				FDefaultValueHelper::ParseInt(lines[2 + run], spx);
				tileMap->Get(x, y)->splitWS_x = spx;
				run++;


				int spy;
				FDefaultValueHelper::ParseInt(lines[2 + run], spy);
				tileMap->Get(x, y)->splitWS_y = spy;
				run++;
			}
		}
	}
}

void ANavMap::ClearMovementMesh()
{
	tileMap->SetWeightOfAll(1000);
	UpdateMovementMeshVertexColor(0,TArray<FIntervalRange>(),TArray<FIntervalRange>());
}

TArray<UNavTile*> ANavMap::GetAttackableNodes(AUnit * _unit)
{
	TArray<UNavTile*> ret;
	if (_unit != nullptr)
	{
		int maxRange = 0;
		for (int i = 0; i < _unit->GetAttackRange().Num(); i++)
		{
			if (_unit->GetAttackRange()[i].Max > maxRange)
			{
				maxRange = _unit->GetAttackRange()[i].Max;
			}
		}

		for (int x = -maxRange; x <= maxRange; x++)
		{
			for (int y = -maxRange; y <= maxRange; y++)
			{
				for (int i = 0; i < _unit->GetAttackRange().Num(); i++)
				{
					int dist = FMath::Abs(x) + FMath::Abs(y);
					if (_unit->GetAttackRange()[i].Min <= dist && dist <= _unit->GetAttackRange()[i].Max)
					{
						UNavTile* nt = tileMap->Get(_unit->currentTile->x + x, _unit->currentTile->y + y);
						if (nt != nullptr && nt->tileData != nullptr && nt->tileData->restable)
						{
							if(dist <= 1)
							{ 
								ret.Add(nt);
							}
							else
							{
								if (GetTempWeightOfTile(nt, _unit->currentTile) <= maxRange)
								{
									ret.Add(nt);
								}
							}
						}
					}
				}
			}
		}
	}

	return ret;
}

TArray<AUnit*> ANavMap::GetAttackableUnits(AUnit * _unit)
{
	TArray<AUnit*> ret;

	if (_unit != nullptr)
	{
		TArray<UNavTile*> tls = GetAttackableNodes(_unit);

		for (int i = 0; i < tls.Num(); i++)
		{
			if (tls[i]->unit != nullptr && _unit->CanAttackUnitOfTeam(tls[i]->unit->team))
			{
				ret.Add(tls[i]->unit);
			}
		}
	}

	return ret;
}

TArray<AUnit*> ANavMap::GetUnitsInStaffRange(AUnit * _unit)
{
	TArray<AUnit*> ret;

	if (_unit != nullptr)
	{
		for (int i = 0; i < (_unit->characterData).inventory.Num(); i++)
		{
			UItemData* itd = (_unit->characterData).inventory[i]->item;
			if (dynamic_cast<UStaffData*>(itd))
			{
				UStaffData* stf = dynamic_cast<UStaffData*>(itd);

				ret.Append(GetUnitsInSpecificStaffRange(_unit, stf));
			}
		}
	}

	return ret;
}

TArray<AUnit*> ANavMap::GetUnitsInEquippedStaffRange(AUnit * _unit)
{
	TArray<AUnit*> ret;

	if (_unit != nullptr)
	{
		if (dynamic_cast<UStaffData*>(_unit->GetEquippedWeapon()))
		{
			UStaffData* stf = dynamic_cast<UStaffData*>(_unit->GetEquippedWeapon());

			ret = GetUnitsInSpecificStaffRange(_unit, stf);
		}
	}

	return ret;
}

TArray<AUnit*> ANavMap::GetUnitsInSpecificStaffRange(AUnit * _unit, UStaffData* _item)
{
	TArray<AUnit*> ret;

	if (_unit != nullptr)
	{
		for (int x = 0; x < sizeX; x++)
		{
			for (int y = 0; y < sizeY; y++)
			{
				int distX = x - _unit->currentTile->x;
				int distY = y - _unit->currentTile->y;
				int dist = GetTempWeightOfTile(_unit->currentTile, tileMap->Get(x,y));

				if (_item != nullptr)
				{
					UStaffData* stf = _item;
					if (dist <= stf->staffRange.Max && dist >= stf->staffRange.Min)
					{
						UNavTile* tl = tileMap->Get(x, y);
						if (tl != nullptr)
						{
							if (tl->unit != nullptr && !_unit->CanAttackUnitOfTeam(tl->unit->team))
							{
								if (tl->unit != _unit)
								{
									bool canAdd = false;
									if (dynamic_cast<UHealingStaffData*>(stf))
									{
										if (tl->unit->GetCurrentHP() < tl->unit->GetHP())
										{
											canAdd = true;
										}
									}
									if (canAdd)
									{
										ret.Add(tl->unit);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return ret;
}

TArray<AUnit*> ANavMap::GetAttackableUnitsWithEquippedWeapon(AUnit * _unit)
{
	TArray<AUnit*> ret;

	if (_unit != nullptr)
	{
		TArray<UNavTile*> tls = GetAttackableNodes(_unit);

		for (int i = 0; i < tls.Num(); i++)
		{
			if (tls[i]->unit != nullptr && _unit->CanAttackUnitOfTeam(tls[i]->unit->team))
			{
				if (_unit->IsInRangeOfCurrentWeapon(tls[i]->unit))
				{
					if (!ret.Contains(tls[i]->unit))
					{
						ret.Add(tls[i]->unit);
					}
				}
			}
		}
	}

	return ret;
}

UNavTile* ANavMap::GetTile(int x, int y)
{
	return tileMap->Get(x, y);
}

void ANavMap::WeightMap(UNavTile * start, int32 traverseType, EUnitTeam _team)
{
	if (tileMap != nullptr && start != nullptr)
	{
		TArray<UNavTile*> unvisited = tileMap->GetAllNodes();

		tileMap->SetWeightOfAll(10000);
		start->currentWeight = 0;

		UNavTile* current = start;
		TArray<UNavTile*> neighbours;
		while (unvisited.Num() > 0)
		{
			neighbours.Empty();
			if (TEST_BIT(traverseType, ETraverseType::TT_Flying))
			{
				neighbours = tileMap->GetNeighbours(current);
			}
			else
			{
				neighbours = tileMap->GetNeighboursSplitInfluence(current);
			}
			for (int i = 0; i < neighbours.Num(); i++)
			{
				if (neighbours[i]->tileData != nullptr && (neighbours[i]->tileData->traverseType & traverseType) > 0 && current != nullptr)
				{
					bool block = false;
					if (neighbours[i]->unit != nullptr)
					{
						if (neighbours[i]->unit->BlocksUnitOfTeam(_team, nullptr))
						{
							block = true;
						}
					}

					if (!block)
					{
						int cost = 1;

						if (TEST_BIT(traverseType, ETraverseType::TT_Flying))
						{
							cost = neighbours[i]->tileData->flyingCosts + current->currentWeight;
						}
						else
						{
							cost = neighbours[i]->tileData->movementCosts + current->currentWeight;
						}

						if (neighbours[i]->currentWeight > cost)
						{
							neighbours[i]->currentWeight = cost;
						}
					}
				}
			}
			unvisited.Remove(current);
			UNavTile* nextCurrent = nullptr;
			for (int i = 0; i < unvisited.Num(); i++)
			{
				if (nextCurrent == nullptr || nextCurrent->currentWeight > unvisited[i]->currentWeight)
				{
					nextCurrent = unvisited[i];
				}
			}
			current = nextCurrent;
		}
	}
}

void ANavMap::WeightMap(UNavTile * start, int32 traverseType, EUnitTeam _team, AUnit* _unit)
{
	if (tileMap != nullptr && start != nullptr)
	{
		TArray<UNavTile*> unvisited = tileMap->GetAllNodes();

		tileMap->SetWeightOfAll(10000);
		start->currentWeight = 0;

		UNavTile* current = start;
		TArray<UNavTile*> neighbours;
		while (unvisited.Num() > 0)
		{
			neighbours.Empty();
			if (TEST_BIT(traverseType, ETraverseType::TT_Flying))
			{
				neighbours = tileMap->GetNeighbours(current);
			}
			else
			{
				neighbours = tileMap->GetNeighboursSplitInfluence(current);
			}
			for (int i = 0; i < neighbours.Num(); i++)
			{
				if (neighbours[i]->tileData != nullptr && (neighbours[i]->tileData->traverseType & traverseType) > 0 && current != nullptr)
				{
					bool block = false;
					if (neighbours[i]->unit != nullptr)
					{
						if (neighbours[i]->unit->BlocksUnitOfTeam(_team, _unit))
						{
							block = true;
						}
					}

					if (!block)
					{
						int cost = 1;

						if (TEST_BIT(traverseType, ETraverseType::TT_Flying))
						{
							cost = neighbours[i]->tileData->flyingCosts + current->currentWeight;
						}
						else
						{
							int tileCost = neighbours[i]->tileData->movementCosts;
							for(int k = 0; k < neighbours[i]->tileData->customCosts.Num(); k++)
							{
								FCustomCostByGameplayTag cont = neighbours[i]->tileData->customCosts[k];
								if (_unit->characterData.characterClass->classTags.HasTag(cont.tag))
								{
									if (tileCost < cont.cost)
									{
										tileCost = cont.cost;
									}
								}
							}
							cost = tileCost + current->currentWeight;
						}

						if (neighbours[i]->currentWeight > cost)
						{
							neighbours[i]->currentWeight = cost;
						}
					}
				}
			}
			unvisited.Remove(current);
			UNavTile* nextCurrent = nullptr;
			for (int i = 0; i < unvisited.Num(); i++)
			{
				if (nextCurrent == nullptr || nextCurrent->currentWeight > unvisited[i]->currentWeight)
				{
					nextCurrent = unvisited[i];
				}
			}
			current = nextCurrent;
		}
	}
}

void ANavMap::WeightMapTemp(UNavTile * start, int id)
{
	if (tileMap != nullptr && start != nullptr)
	{
		TArray<UNavTile*> unvisited = tileMap->GetAllNodes();

		tileMap->SetTempWeightOfAll(10000, id);
		start->tempWeights[id] = 0;

		UNavTile* current = start;
		TArray<UNavTile*> neighbours;
		while (unvisited.Num() > 0)
		{
			neighbours.Empty();
			neighbours = tileMap->GetNeighbours(current);
			for (int i = 0; i < neighbours.Num(); i++)
			{
				if (neighbours[i]->tileData != nullptr && current != nullptr)
				{

						int cost = 1 + current->tempWeights[id];

						if (!neighbours[i]->tileData->blocksAttacks)
						{
							if (neighbours[i]->tempWeights[id] > cost)
							{
								neighbours[i]->tempWeights[id] = cost;
							}
						}

						if (neighbours[i]->unit != nullptr && neighbours[i]->unit->blocksAttacks)
						{
							neighbours[i]->tempWeights[id] = 10000;
						}
				}
			}
			unvisited.Remove(current);
			UNavTile* nextCurrent = nullptr;
			for (int i = 0; i < unvisited.Num(); i++)
			{
				if (nextCurrent == nullptr || nextCurrent->tempWeights[id] > unvisited[i]->tempWeights[id])
				{
					nextCurrent = unvisited[i];
				}
			}
			current = nextCurrent;
		}
	}
}

void ANavMap::WeightMapNoUnitBlock(UNavTile * start, int32 traverseType)
{
	if (tileMap != nullptr && start != nullptr)
	{
		TArray<UNavTile*> unvisited = tileMap->GetAllNodes();

		tileMap->SetWeightOfAll(10000);
		start->currentWeight = 0;

		UNavTile* current = start;
		TArray<UNavTile*> neighbours;
		while (unvisited.Num() > 0)
		{
			neighbours.Empty();
			if (TEST_BIT(traverseType, ETraverseType::TT_Flying))
			{
				neighbours = tileMap->GetNeighbours(current);
			}
			else
			{
				neighbours = tileMap->GetNeighboursSplitInfluence(current);
			}
			for (int i = 0; i < neighbours.Num(); i++)
			{
				if (neighbours[i]->tileData != nullptr && (neighbours[i]->tileData->traverseType & traverseType) > 0 && current != nullptr)
				{

					int cost = 1;

					if (TEST_BIT(traverseType, ETraverseType::TT_Flying))
					{
						cost = neighbours[i]->tileData->flyingCosts + current->currentWeight;
					}
					else
					{
						cost = neighbours[i]->tileData->movementCosts + current->currentWeight;
					}

					if (neighbours[i]->currentWeight > cost)
					{
						neighbours[i]->currentWeight = cost;
					}
				}
			}
			unvisited.Remove(current);
			UNavTile* nextCurrent = nullptr;
			for (int i = 0; i < unvisited.Num(); i++)
			{
				if (nextCurrent == nullptr || nextCurrent->currentWeight > unvisited[i]->currentWeight)
				{
					nextCurrent = unvisited[i];
				}
			}
			current = nextCurrent;
		}
	}
}

void ANavMap::WeightMapFromMultipleUnits(TArray<AUnit*> _units)
{
	tileMap->SetWeightOfAll(10000);
	for (int k = 0; k < _units.Num(); k++)
	{
		UNavTile* start = _units[k]->currentTile;
		int32 traverseType = _units[k]->GetTraverseType();
		EUnitTeam _team = _units[k]->team;

		if (tileMap != nullptr && start != nullptr)
		{
			TArray<UNavTile*> unvisited = tileMap->GetAllNodes();

			start->currentWeight = 0;

			UNavTile* current = start;
			TArray<UNavTile*> neighbours;
			while (unvisited.Num() > 0)
			{
				neighbours.Empty();
				neighbours = tileMap->GetNeighbours(current);
				for (int i = 0; i < neighbours.Num(); i++)
				{
					if (neighbours[i]->tileData != nullptr && (neighbours[i]->tileData->traverseType & traverseType) > 0 && current != nullptr)
					{
						bool block = false;
						if (neighbours[i]->unit != nullptr)
						{
							if (neighbours[i]->unit->BlocksUnitOfTeam(_team, nullptr))
							{
								block = true;
							}
						}

						if (!block)
						{
							int cost = 1;

							if (TEST_BIT(traverseType, ETraverseType::TT_Flying))
							{
								cost = neighbours[i]->tileData->flyingCosts + current->currentWeight;
							}
							else
							{
								cost = neighbours[i]->tileData->movementCosts + current->currentWeight;
							}

							if (neighbours[i]->currentWeight > cost)
							{
								neighbours[i]->currentWeight = cost;
							}
						}
					}
				}
				unvisited.Remove(current);
				UNavTile* nextCurrent = nullptr;
				for (int i = 0; i < unvisited.Num(); i++)
				{
					if (nextCurrent == nullptr || nextCurrent->currentWeight > unvisited[i]->currentWeight)
					{
						nextCurrent = unvisited[i];
					}
				}
				current = nextCurrent;
			}
		}
	}
}

void ANavMap::SetSelectedTile(UNavTile* _tile)
{
	selectedTile = _tile;
}

TArray<UNavTile*> ANavMap::GetPath(UNavTile * _start, UNavTile * _target)
{
	TArray<UNavTile*> ret;
	TArray<UNavTile*> _path;
	if (_start != nullptr && _target != nullptr)
	{
		UNavTile* current = _target;

		ret.Add(current);

		int num = 1000;
		while (current != nullptr && current != _start && num > 0)
		{
			int min = current->currentWeight + 1;
			TArray<UNavTile*> neigh = tileMap->GetNeighbours(current);;
			
			for (int i = 0; i < neigh.Num(); i++)
			{
				if (neigh[i]->currentWeight < current->currentWeight)
				{
					current = neigh[i];
				}
			}
			ret.Add(current);
			num--;
		}

		
		for (int i = ret.Num() - 1; i >= 0; i--)
		{
			_path.Add(ret[i]);
		}
	}

	return _path;
}

TArray<UNavTile*> ANavMap::GetPathTraverse(UNavTile * _start, UNavTile * _target, int32 traverseType)
{
	TArray<UNavTile*> ret;
	TArray<UNavTile*> _path;
	if (_start != nullptr && _target != nullptr)
	{
		UNavTile* current = _target;

		ret.Add(current);

		int num = 1000;
		while (current != nullptr && current != _start && num > 0)
		{
			int min = current->currentWeight + 1;
			TArray<UNavTile*> neigh;
			if (TEST_BIT(traverseType, ETraverseType::TT_Flying))
			{
				neigh = tileMap->GetNeighbours(current);
			}
			else
			{
				neigh = tileMap->GetNeighboursSplitInfluence(current);
			}
			for (int i = 0; i < neigh.Num(); i++)
			{
				if (neigh[i]->currentWeight < current->currentWeight)
				{
					current = neigh[i];
				}
			}
			ret.Add(current);
			num--;
		}


		for (int i = ret.Num() - 1; i >= 0; i--)
		{
			_path.Add(ret[i]);
		}
	}

	return _path;
}

TArray<FVector> ANavMap::TilePathToVectorPathSmoothed(TArray<UNavTile*> _path)
{
	TArray<FVector> vectorPath;

	if (_path.Num() > 0)
	{
		vectorPath.Add(GetLocationOfTile(_path[0]));

		for (int i = 1; i < _path.Num(); i++)
		{
			FVector new_loc = FMath::Lerp(vectorPath[vectorPath.Num() - 1], GetLocationOfTile(_path[i]), 0.5f);
			vectorPath.Add(new_loc);
			vectorPath.Add(GetLocationOfTile(_path[i]));
		}
		for (int i = 2; i < vectorPath.Num() - 1; i++)
		{
			vectorPath[i] = FMath::Lerp(vectorPath[i], FMath::Lerp(vectorPath[i - 1], vectorPath[i + 1], 0.5f), 0.5f);
			i++;
		}
	}

	return vectorPath;
}

int ANavMap::GetCostOfPath(TArray<UNavTile*> _path, AUnit * _unit)
{

	int cost = 0;
	for (int i = 1; i < _path.Num(); i++)
	{
		UNavTile* t = _path[i];
		if ((t->tileData->traverseType & _unit->characterData.characterClass->traverseType) > 0)
		{
			if (TEST_BIT(_unit->characterData.characterClass->traverseType, ETraverseType::TT_Flying))
			{
				cost += t->tileData->flyingCosts;
			}
			else
			{
				int tileCost = t->tileData->movementCosts;
				for (int k = 0; k < t->tileData->customCosts.Num(); k++)
				{
					FCustomCostByGameplayTag cont = t->tileData->customCosts[k];
					if (_unit->characterData.characterClass->classTags.HasTag(cont.tag))
					{
						if (tileCost < cont.cost)
						{
							tileCost = cont.cost;
						}
					}
				}
				cost += tileCost;
			}
		}
		else
		{
			cost = 1000;
		}
	}

	return cost;
}

TArray<AUnit*> ANavMap::GetTeamNeightbors(AUnit * _unit)
{

	TArray<UNavTile*> neigh = GetNeighborsOfTle(_unit->currentTile);
	TArray<AUnit*> units;

	for (int i = 0; i < neigh.Num(); i++)
	{
		if (neigh[i]->unit != nullptr)
		{
			AUnit* u = neigh[i]->unit;
			if (u->team == _unit->team)
			{
				if (neigh[i]->unit->IsAlive())
				{
					units.Add(neigh[i]->unit);
				}
			}
		}
	}

	return units;
}

TArray<AUnit*> ANavMap::GetAllNeightbors(AUnit * _unit)
{
	TArray<UNavTile*> neigh = GetNeighborsOfTle(_unit->currentTile);
	TArray<AUnit*> units;

	for (int i = 0; i < neigh.Num(); i++)
	{
		if (neigh[i]->unit != nullptr)
		{
			AUnit* u = neigh[i]->unit;
			if (neigh[i]->unit->IsAlive())
			{
				units.Add(neigh[i]->unit);
			}
		}
	}

	return units;
}

int ANavMap::GetTempWeightOfTile(UNavTile * from, UNavTile * to)
{
	int id = from->x*sizeY + from->y;
	return to->tempWeights[id];
}

void ANavMap::BuildTempWeights(TArray<UNavTile*> _notifyTiles)
{
	/*for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			WeightMapTemp(GetTile(x, y), x*sizeY + y);
		}
	}*/
	notifyTilesAfterBuild = _notifyTiles;
	StartBuildingTempWeights();
	(new FAutoDeleteAsyncTask<BuildTempWeightsRuntime>(this))->StartBackgroundTask();
}


void ANavMap::SpawnUnit_Implementation(AActor* spawnedBy)
{
	
}

void ANavMap::StartBuildingTempWeights_Implementation()
{
}

void ANavMap::EndBuildingTempWeights_Implementation()
{
	NotifyEnemyRangeTiles(notifyTilesAfterBuild);

	UpdateEnemyRangeMeshVertexColor();
}



//============================

BuildTempWeightsRuntime::BuildTempWeightsRuntime(ANavMap * _map)
{
	_nMap = _map;
}

BuildTempWeightsRuntime::~BuildTempWeightsRuntime()
{
}

void BuildTempWeightsRuntime::DoWork()
{
	//_nMap->StartBuildingTempWeights();
	weightCount = 0;
	for (int x = 0; x < _nMap->sizeX; x++)
	{
		for (int y = 0; y < _nMap->sizeY; y++)
		{
			_nMap->WeightMapTemp(_nMap->GetTile(x, y), x*_nMap->sizeY + y);
			weightCount++;
		}
	}
	_nMap->finishedBuildOfTempWeights = true;
	//_nMap->EndBuildingTempWeights();
}
