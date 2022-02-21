// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCursor.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APlayerCursor::APlayerCursor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	cursorMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CursorMesh"));
	cursorMesh->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
	selectedUnit = nullptr;
}

// Called when the game starts or when spawned
void APlayerCursor::BeginPlay()
{
	Super::BeginPlay();
	
	if (tileMap == nullptr)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANavMap::StaticClass(), FoundActors);

		if (FoundActors.Num() > 0)
		{
			if (dynamic_cast<ANavMap*>(FoundActors[0]))
			{
				tileMap = dynamic_cast<ANavMap*>(FoundActors[0]);
			}
		}
	}
	else
	{
		if (cursorTile != nullptr)
		{
			SetActorLocation(tileMap->GetLocationOfTile(cursorTile));
		}
	}

	CreateCursorMesh();
	if (cursorMaterial != nullptr)
	{
		cursorMesh->SetMaterial(0, cursorMaterial);
	}
}

// Called every frame
void APlayerCursor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (tileMap != nullptr)
	{
		if (!init)
		{
			//if (GEngine)
			//	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Yellow, TEXT("Init"));

			TArray<AActor*> FoundUnits;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUnit::StaticClass(), FoundUnits);

			cursorTile = tileMap->GetTile(0, 0);

			/*for(int i = 0; i < FoundUnits.Num(); i++)
			{
				if (dynamic_cast<AUnit*>(FoundUnits[i]))
				{
					AUnit* un = dynamic_cast<AUnit*>(FoundUnits[i]);
					
					if (un->team == playerTeam && un->IsAlive())
					{
						if (un->currentTile != nullptr)
						{
							cursorTile = un->currentTile;
							tileMap->SetSelectedTile(cursorTile);
							UpdateCursorMesh();
							if (GEngine)
								GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Green, UKismetSystemLibrary::GetDisplayName(un));
							//tileMap->UpdateGridMeshVertexColor();
							break;
						}
					}
				}
			}
			
			SetActorLocation(tileMap->GetLocationOfTile(cursorTile));
			UpdateCursorMesh();*/
			init = true;
			OnInit();
		}
	}
	else
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANavMap::StaticClass(), FoundActors);

		if (FoundActors.Num() > 0)
		{
			if (dynamic_cast<ANavMap*>(FoundActors[0]))
			{
				tileMap = dynamic_cast<ANavMap*>(FoundActors[0]);
			}
		}
	}
}

// Called to bind functionality to input
void APlayerCursor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCursor::OnInit_Implementation()
{
}

int APlayerCursor::MoveCursor(int x, int y)
{
	if (tileMap != nullptr && tileMap->GetLevel()->bIsVisible)
	{
		if (cursorTile != nullptr)
		{
			UNavTile* _tile = tileMap->GetTile(cursorTile->x + x, cursorTile->y + y);

			if (_tile != nullptr)
			{
				bool outOfMovement = false;
				if (selectedUnit != nullptr)
				{
					if (cursorTile->currentWeight > selectedUnit->GetMovement())
					{
						outOfMovement = true;
					}
				}

				cursorTile = _tile;

				SetActorLocation(tileMap->GetLocationOfTile(cursorTile));
				tileMap->SetSelectedTile(cursorTile);
				/*tileMap->UpdateGridMeshVertexColor();*/
				UpdateCursorMesh();

				if (selectedUnit != nullptr)
				{
					if (cursorTile->currentWeight <= selectedUnit->GetMovement())
					{
						if (movementPath.Contains(cursorTile))
						{
							TArray<UNavTile*> remap;
							bool stop = false;
							for (int i = 0; i < movementPath.Num() && !stop; i++)
							{
								if (movementPath[i] != cursorTile)
								{
									remap.AddUnique(movementPath[i]);
								}
								else
								{
									remap.AddUnique(movementPath[i]);
									stop = true;
								}
							}
							movementPath.Empty();
							movementPath.Append(remap);
						}
						else
						{
							if (movementPath.Num() > 0)
							{
								UNavTile* tl = movementPath[movementPath.Num() - 1];
								if (FMath::Abs(tl->x - cursorTile->x) + FMath::Abs(tl->y - cursorTile->y) > 1)
								{
									if (movementPath.Contains(tileMap->GetTile(cursorTile->x, tl->y)))
									{
										outOfMovement = true;
									}
									movementPath.Add(tileMap->GetTile(cursorTile->x, tl->y));
								}
								movementPath.AddUnique(cursorTile);
								int cost = 0;
								for (int i = 1; i < movementPath.Num(); i++)
								{
									UNavTile* t = movementPath[i];
									if ((t->tileData->traverseType & selectedUnit->characterData.characterClass->traverseType) > 0)
									{
										if (TEST_BIT(selectedUnit->characterData.characterClass->traverseType, ETraverseType::TT_Flying))
										{
											cost += t->tileData->flyingCosts;
										}
										else
										{
											if (!TEST_BIT(selectedUnit->characterData.characterClass->traverseType, ETraverseType::TT_Swimming))
											{
												if (TEST_BIT(t->tileData->traverseType, ETraverseType::TT_Swimming))
												{
													cost += 1000;
												}
											}
											if (t->unit != nullptr)
											{
												if (t->unit->BlocksUnitOfTeam(selectedUnit->team, selectedUnit))
												{
													outOfMovement = true;
												}
											}

											cost += t->tileData->movementCosts;

										}
									}
									else
									{
										cost += 1000;
									}
								}

								int p_num = movementPath.Num();
								if (p_num >= 2 && !TEST_BIT(selectedUnit->characterData.characterClass->traverseType, ETraverseType::TT_Flying))
								{
									UNavTile* t1 = movementPath[p_num - 1];
									UNavTile* t2 = movementPath[p_num - 2];

									if (FMath::Abs(t1->x - t2->x) > 0)
									{
										if (t1->x - t2->x > 0)
										{
											if (t2->splitWS_x)
											{
												outOfMovement = true;
											}
										}
										else
										{
											if (t1->splitWS_x)
											{
												outOfMovement = true;
											}
										}
									}
									else
									{
										if (t1->y - t2->y > 0)
										{
											if (t2->splitWS_y)
											{
												outOfMovement = true;
											}
										}
										else
										{
											if (t1->splitWS_y)
											{
												outOfMovement = true;
											}
										}
									}
								}

								if (cost > selectedUnit->GetMovement() || outOfMovement)
								{
									movementPath = tileMap->GetPathTraverse(selectedUnit->currentTile, cursorTile, selectedUnit->GetTraverseType());
								}
							}
						}
					}
					tileMap->CreatePathMesh(movementPath);
				}

				if (_tile != nullptr)
				{
					return _tile->currentWeight;
				}
			}
		}
	}
	return -1;
}

FVector APlayerCursor::GetTracedVertex(float x, float y, FVector2D offset)
{
	float ox = x + offset.X;
	float oy = y + offset.Y;

	x = (x * 0.99f) + offset.X;
	y = (y * 0.99f) + offset.Y;

	FHitResult Hit(ForceInit);

	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(Hit, FVector(x, y, 10000) + GetActorLocation(), FVector(x, y, -10000) + GetActorLocation(), ECC_WorldStatic, CollisionParams))
	{
		return FVector(ox, oy, Hit.Location.Z + 20) - FVector(0,0,GetActorLocation().Z);
	}

	return FVector(ox, oy, 20);
}

TArray<FVector> APlayerCursor::CursorMeshVerts()
{
	FVector2D offset = FVector2D(0,0);
	TArray<FVector> vertices;

	vertices.Add(GetTracedVertex(-50, -50, offset));
	vertices.Add(GetTracedVertex(-50, 50, offset));
	vertices.Add(GetTracedVertex(50, -50, offset));
	vertices.Add(GetTracedVertex(50, 50, offset));
	return vertices;
}

void APlayerCursor::CreateCursorMesh()
{
	struct ILocal
	{
		static TArray<int> GridTileTris()
		{
			TArray<int> tris;
			tris.Add(0);
			tris.Add(1);
			tris.Add(2);
			tris.Add(1);
			tris.Add(3);
			tris.Add(2);
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
	};

	TArray<FVector> verts;
	TArray<int> tris;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	verts.Append(CursorMeshVerts());

	tris.Append(ILocal::GridTileTris());
	uvs.Append(ILocal::GridTileUV());

	cursorMesh->CreateMeshSection_LinearColor(0, verts, tris, normals, uvs, vertexColors, tangents, true);
}

void APlayerCursor::UpdateCursorMesh()
{
	struct ILocal
	{
		static TArray<int> GridTileTris()
		{
			TArray<int> tris;
			tris.Add(0);
			tris.Add(1);
			tris.Add(2);
			tris.Add(1);
			tris.Add(3);
			tris.Add(2);
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
	};

	TArray<FVector> verts;
	TArray<int> tris;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	verts.Append(CursorMeshVerts());

	//tris.Append(ILocal::GridTileTris());
	uvs.Append(ILocal::GridTileUV());

	cursorMesh->UpdateMeshSection_LinearColor(0, verts, normals, uvs, vertexColors, tangents);
}

bool APlayerCursor::TryToSelectUnit()
{
	if (cursorTile != nullptr && cursorTile->unit != nullptr)
	{
		if (cursorTile->unit->team == playerTeam)
		{
			if (cursorTile->unit->state != EUnitState::EWaiting)
			{
				selectedUnit = cursorTile->unit;
				selectedUnit->WeightMapFromHere();
				movementPath.Empty();
				movementPath.Add(cursorTile);
			}
			return true;
		}
		else if(cursorTile->unit->CanAttackUnitOfTeam(playerTeam))
		{
			if (tileMap->EnemyRangeContainsUnit(cursorTile->unit))
			{
				tileMap->RemoveEnemyRangeUnit(cursorTile->unit);
			}
			else
			{
				tileMap->AddEnemyRangeUnit(cursorTile->unit);
			}
			tileMap->UpdateEnemyRangeMeshVertexColor();
			return false;
		}
	}
	return false;
}

bool APlayerCursor::MoveSelectedUnitToCursor()
{
	if (selectedUnit != nullptr && cursorTile != nullptr)
	{
		selectedUnit->moveTilePath = movementPath;
		if (selectedUnit->MoveToTile(cursorTile))
		{
			//selectedUnit = nullptr;
			return true;
		}
	}
	return false;
}

void APlayerCursor::ClearSelectedUnit()
{
	selectedUnit = nullptr;
}

void APlayerCursor::SetCurrentTile(UNavTile * _tile)
{
	if (_tile != nullptr)
	{
		cursorTile = _tile;
	}
}

