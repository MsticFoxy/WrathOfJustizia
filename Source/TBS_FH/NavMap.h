// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavTile.h"
#include "TileData.h"
#include "ProceduralMeshComponent.h"
#include "TileMap.h"

#include "NavMap.generated.h"

UENUM(BlueprintType)
enum class EUnitTeam : uint8
{
	E_Player,
	E_NPC,
	E_Partner,
	E_Enemy,
	E_None
};

USTRUCT(BlueprintType)
struct FIntervalRange
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		int Min;
	UPROPERTY(EditAnywhere)
		int Max;
};

USTRUCT(BlueprintType)
struct FEnemyRangeSet
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		AUnit* _unit;
	UPROPERTY()
		TArray<UNavTile*> ranges;
	UPROPERTY()
		TArray<UNavTile*> movement;
};

UCLASS()
class TBS_FH_API ANavMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANavMap();

	UPROPERTY(EditAnywhere)
		bool editMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTileData* _selectedTileData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EUnitTeam> player_Blocks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EUnitTeam> player_CanAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EUnitTeam> partner_Blocks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EUnitTeam> partner_CanAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EUnitTeam> npc_Blocks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EUnitTeam> npc_CanAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EUnitTeam> enemy_Blocks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EUnitTeam> enemy_CanAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EUnitTeam> none_Blocks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EUnitTeam> none_CanAttack;

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size")
		int startX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size")
		int startY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size", meta = (ClampMin = "1.0", ClampMax = "100.0", UIMin = "1.0", UIMax = "100.0"))
		int sizeX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size", meta = (ClampMin = "1.0", ClampMax = "100.0", UIMin = "1.0", UIMax = "100.0"))
		int sizeY;
	UPROPERTY(EditAnywhere, Category = "Initialization")
		UTileData* initTileData;
	UPROPERTY(EditAnywhere)
		FString mapSaveName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UNavTile*> specialEventTiles;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		UTileMap* tileMap;
	UPROPERTY(EditAnywhere)
		UNavTile* tile;
	UPROPERTY(EditAnywhere)
		UNavTile* selectedTile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UProceduralMeshComponent* gridMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UProceduralMeshComponent* movementMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UProceduralMeshComponent* enemyRangeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UProceduralMeshComponent* pathMesh;
	UPROPERTY(EditAnywhere)
		UMaterialInterface* editGridMaterial;
	UPROPERTY(EditAnywhere)
		UMaterialInterface* gridMaterial;
	UPROPERTY(EditAnywhere)
		UMaterialInterface* movementMaterial;
	UPROPERTY(EditAnywhere)
		UMaterialInterface* enemyRangeMaterial;
	UPROPERTY(EditAnywhere)
		UMaterialInterface* movementPathMaterial;

	FString path;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//std::vector<UNavTile*> GetPath(UNavTile* start, UNavTile* end, ETraverseType traverseType);

	UTileMap* GetWeightedMap(UNavTile* start, UNavTile* end, ETraverseType traverseType);

	FVector GetTracedVertex(float x, float y, FVector2D offset);

	TArray<FVector> GridTileVerts(FVector2D offset);

	void CreateGridMesh();

	UFUNCTION(BlueprintCallable)
	void UpdateGridMeshVertexColor();

	void UpdateMovementMeshVertexColor(int movement, TArray<FIntervalRange> _range, TArray<FIntervalRange> _hl_range);

	UFUNCTION(BlueprintCallable)
		void HighlightUnitMeshVertexColor(TArray<AUnit*> _units);

	UFUNCTION(BlueprintCallable)
		void CreatePathMesh(TArray<UNavTile*> _path);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FEnemyRangeSet> enemyRanges;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AUnit* loadSpawnUnit;

	UFUNCTION(BlueprintCallable)
		void AddEnemyRangeUnit(AUnit* _unit);
	UFUNCTION(BlueprintCallable)
		void NotifyEnemyRangeTiles(TArray<UNavTile*> _tiles);
	UFUNCTION(BlueprintCallable)
		void NotifyEnemyRangeUnit(AUnit* _unit);
	UFUNCTION(BlueprintCallable)
		bool EnemyRangeContainsUnit(AUnit* _unit);
	UFUNCTION(BlueprintCallable)
		void RemoveEnemyRangeUnit(AUnit* _unit);
	UFUNCTION(BlueprintCallable)
	void UpdateEnemyRangeMeshVertexColor();

	UFUNCTION(BlueprintCallable)
		UNavTile* GetNearestRestableTile(UNavTile* _tile);

	UFUNCTION(BlueprintCallable)
		UNavTile* GetTileAtLocation(FVector loc);

	UFUNCTION(BlueprintCallable)
		FVector GetLocationOfTile(UNavTile* _tile);

	UFUNCTION(BlueprintCallable)
		TArray<UNavTile*> GetReachableNodes(AUnit* _unit);
	UFUNCTION(BlueprintCallable)
		TArray<UNavTile*> GetReachableNodesWithIgnore(AUnit* _unit, AUnit* _ignore);
	UFUNCTION(BlueprintCallable)
		TArray<UNavTile*> GetReachableAttackNodes(AUnit* _unit);

	UFUNCTION(BlueprintCallable)
		TArray<AUnit*> GetReachableUnits(AUnit* _unit);
	UFUNCTION(BlueprintCallable)
		TArray<AUnit*> GetReachableEnemyUnits(AUnit* _unit);
	UFUNCTION(BlueprintCallable)
		TArray<AUnit*> GetReachableAllyUnits(AUnit* _unit);
	UFUNCTION(BlueprintCallable)
		TArray<UNavTile*> GetLocationsToAttackUnit(AUnit* _currentUnit, AUnit* _unit);
	UFUNCTION(BlueprintCallable)
		TArray<UNavTile*> GetLocationsToAttackUnitGlobal(AUnit* _currentUnit, AUnit* _unit);

	UFUNCTION(BlueprintCallable)
		AUnit* GetUnitAtCursor();
	UFUNCTION(BlueprintCallable)
	void TryWeightMapAtCursor();


	UFUNCTION(BlueprintCallable)
		TArray<UNavTile*> GetNeighborsOfTle(UNavTile* _tile);

	UFUNCTION(BlueprintCallable)
	UTileData* GetTileDataAtLocation(FVector loc);

	UFUNCTION(BlueprintCallable)
	int GetTileHeightAtLocation(FVector loc);

	UFUNCTION(BlueprintCallable)
	FString GetTileTraverseTypeAtLocation(FVector loc);

	UFUNCTION(BlueprintCallable)
		void SetTileDataAtLocation(FVector loc, UTileData* data);

	UFUNCTION(BlueprintCallable)
		void SetTileHeightAtLocation(FVector loc, int height);

	UFUNCTION(BlueprintCallable)
		void SplitTile(FVector loc);

	UFUNCTION(BlueprintCallable)
		void SaveMap();

	UFUNCTION(BlueprintCallable)
		void LoadMap();

	UFUNCTION(BlueprintCallable)
		void ClearMovementMesh();

	UFUNCTION(BlueprintCallable)
		TArray<UNavTile*> GetAttackableNodes(AUnit* _unit);

	UFUNCTION(BlueprintCallable)
		TArray<AUnit*> GetAttackableUnits(AUnit* _unit);

	UFUNCTION(BlueprintCallable)
		TArray<AUnit*> GetUnitsInStaffRange(AUnit* _unit);

	UFUNCTION(BlueprintCallable)
		TArray<AUnit*> GetUnitsInEquippedStaffRange(AUnit* _unit);

	UFUNCTION(BlueprintCallable)
		TArray<AUnit*> GetUnitsInSpecificStaffRange(AUnit* _unit, UStaffData* _item);

	UFUNCTION(BlueprintCallable)
		TArray<AUnit*> GetAttackableUnitsWithEquippedWeapon(AUnit* _unit);

	UNavTile* GetTile(int x, int y);

	void WeightMap(UNavTile* start, int32 traverseType, EUnitTeam _team);
	void WeightMap(UNavTile* start, int32 traverseType, EUnitTeam _team, AUnit* _unit);
	void WeightMapTemp(UNavTile* start, int id);

	void WeightMapNoUnitBlock(UNavTile* start, int32 traverseType);

	void WeightMapFromMultipleUnits(TArray<AUnit*> _units);

	UFUNCTION(BlueprintCallable)
		void SetSelectedTile(UNavTile* _tile);

	UFUNCTION(BlueprintCallable)
	TArray<UNavTile*> GetPath(UNavTile* _start, UNavTile* _target);

	UFUNCTION(BlueprintCallable)
	TArray<UNavTile*> GetPathTraverse(UNavTile* _start, UNavTile* _target, int32 traverseType);

	TArray<FVector> TilePathToVectorPathSmoothed(TArray<UNavTile*> path);

	UFUNCTION(BlueprintCallable)
		int GetCostOfPath(TArray<UNavTile*> _path, AUnit* _unit);

	UFUNCTION(BlueprintCallable)
		TArray<AUnit*> GetTeamNeightbors(AUnit* _unit);

	UFUNCTION(BlueprintCallable)
		TArray<AUnit*> GetAllNeightbors(AUnit* _unit);

	UFUNCTION(BlueprintCallable)
		int GetTempWeightOfTile(UNavTile* from, UNavTile* to);

	UFUNCTION(BlueprintCallable)
		void BuildTempWeights(TArray<UNavTile*> _notifyTiles);

	bool finishedBuildOfTempWeights;
	TArray<UNavTile*> notifyTilesAfterBuild;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void StartBuildingTempWeights();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void EndBuildingTempWeights();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void SpawnUnit(AActor* spawnedBy);

};



class BuildTempWeightsRuntime : public FNonAbandonableTask
{
public:
	BuildTempWeightsRuntime(ANavMap* _map);

	~BuildTempWeightsRuntime();

	ANavMap* _nMap;
	int weightCount;

	//required
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(BuildTempWeightsRuntime, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork();
};
