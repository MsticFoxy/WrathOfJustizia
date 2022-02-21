// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NavMap.h"
#include "Unit.h"
#include "PlayerCursor.generated.h"

UCLASS()
class TBS_FH_API APlayerCursor : public APawn
{
	GENERATED_BODY()

private:
	bool init;

public:
	// Sets default values for this pawn's properties
	APlayerCursor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnInit();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UProceduralMeshComponent* cursorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UMaterialInterface* cursorMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ANavMap* tileMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UNavTile* cursorTile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EUnitTeam playerTeam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		AUnit* selectedUnit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<UNavTile*> movementPath;

	UFUNCTION(BlueprintCallable)
		int MoveCursor(int x, int y);

	UFUNCTION(BlueprintCallable)
		FVector GetTracedVertex(float x, float y, FVector2D offset);

	UFUNCTION(BlueprintCallable)
		TArray<FVector> CursorMeshVerts();

	UFUNCTION(BlueprintCallable)
		void CreateCursorMesh();

	UFUNCTION(BlueprintCallable)
		void UpdateCursorMesh();

	UFUNCTION(BlueprintCallable)
		bool TryToSelectUnit();

	UFUNCTION(BlueprintCallable)
		bool MoveSelectedUnitToCursor();

	UFUNCTION(BlueprintCallable)
		void ClearSelectedUnit();

	UFUNCTION(BlueprintCallable)
		void SetCurrentTile(UNavTile* _tile);

};
