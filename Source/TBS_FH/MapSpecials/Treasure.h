// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavTile.h"
#include "NavMap.h"
#include "ItemData.h"
#include "Treasure.generated.h"

USTRUCT(BlueprintType)
struct FTreasureSave
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UItemData* item;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool opened;
};

UCLASS()
class TBS_FH_API ATreasure : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATreasure();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ANavMap* tileMap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UNavTile* tile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Content")
		UItemData* item;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool opened;

	UFUNCTION(BlueprintCallable)
		UItemInstance* Open();

	bool initiated;

};
