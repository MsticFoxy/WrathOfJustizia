// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Unit.h"
#include "LevelOrderLink.h"
#include "CBluePrints.h"
#include "GameSlotData.generated.h"


USTRUCT(BlueprintType)
struct FTeamConvoyInstance
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
		int gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
		int bonusEP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
		int genericUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
		TArray<UItemInstance*> convoy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
		TArray<TSubclassOf<UAbility>> abilityScrolls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
		TArray<FCharacterData> characters;
};

UCLASS()
class TBS_FH_API AGameSlotData : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameSlotData();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame")
		ULevelOrderLink* chapterLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame")
		int currentLevelIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame")
		int run;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame")
		bool hasBookmark;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame")
		FDifficultySettings difficulty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
		TArray<FTeamConvoyInstance> teams;


	UFUNCTION(BlueprintCallable)
		static AGameSlotData* GetGameSlotData(AActor* outer);

	UFUNCTION(BlueprintCallable)
		void AddCharacter(FCharacterData _char, bool initInventory, int _team);

	UFUNCTION(BlueprintCallable)
		FCharacterData GetCharacterDataByPerson(UPerson* _person, int _team);

	UFUNCTION(BlueprintCallable)
		bool ContainsPerson(UPerson* _person, int _team);

	UFUNCTION(BlueprintCallable)
		void UpdateCharacterData(TArray<FCharacterData> _chars, int _team);

	UFUNCTION(BlueprintCallable)
		void AddItemToConvoy(UItemInstance* _item, int _team);

	UFUNCTION(BlueprintCallable)
		bool TradeWithConvoy(UPerson* _person, int invID, int conID, int _team);

	UFUNCTION(BlueprintCallable)
		void AddFreshItemToConvoy(UItemData* _itemData, int _team);

	UFUNCTION(BlueprintCallable)
		void AddGold(int value, int _team);

	UFUNCTION(BlueprintCallable)
		int GetGold(int _team);

	UFUNCTION(BlueprintCallable)
		void SpendGold(int value, int _team);

	UFUNCTION(BlueprintCallable)
		bool HasGold(int value, int _team);

	UFUNCTION(BlueprintCallable)
		void CheckTeam(int _team);

	UFUNCTION(BlueprintCallable)
		void CombineTeam(int _teamA, int _teamB);
};
