// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "NavMap.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ClassData.h"
#include "ItemData.h"
#include "ActionOption.h"
#include "HealingItemData.h"
#include "ResourceItemData.h"
#include "Dialog/Person.h"
#include "StatusEffect/StatusEffectData.h"
#include "StatusEffect/StatusEffectInstance.h"
#include "StatusEffectItemData.h"
#include "StatBonusItemData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Dialog/Dialog.h"
#include "WeaponData.h"
#include "CharacterDataSet.h"
#include "Unit.generated.h"

USTRUCT(BlueprintType)
struct FDialogPossability
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UPerson*> initiationTalkPartner;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDialog* dialog;
};

UENUM(BlueprintType)
enum class EUnitState : uint8
{
	EReady,
	EMoving,
	EActing,
	EAttacking,
	EItems,
	ETradeSelect,
	ETrade,
	ESelectInteractable,
	EExecuteSelection,
	ECanto,
	ECantoMove,
	ECantoEnd,
	EWaiting
};

UENUM(BlueprintType)
enum class ESelectionPurpose : uint8
{
	EStaff,
	ELeaver
};

UENUM(BlueprintType)
enum class EStatEnumerator: uint8
{
	HP,
	Strength,
	Magic,
	Endurance,
	Mana,
	Defence,
	Resistence,
	Skill,
	Luck,
	Speed
};

UCLASS()
class TBS_FH_API AUnit : public APawn
{
	GENERATED_BODY()

private:
	bool init;

public:	
	// Sets default values for this actor's properties
	AUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		UCharacterDataSet* characterSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		EUnitTeam team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool autoGenerateStats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool addManualStats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool blocksShadow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool blocksAttacks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		FCharacterData characterData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		TArray<UStatusEffectInstance*> statusEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		AActor* spawnedBy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FDialogPossability> fieldDialogs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool canMove;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBehaviorTree* currentBehavior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* unitMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UArrowComponent* forwardArrowComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UArrowComponent* worldSteadyArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ANavMap* tileMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UNavTile* currentTile;
	UNavTile* lastTile;

	FRotator lookRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EUnitState state;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESelectionPurpose selectionPurpose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int alreadyMoved;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UStatusEffectData*> afterBattleStatusEffects;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void NotifyItemAdditionToInventory(UItemInstance* _item, bool inventoryFull);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void StartFieldDialogBP(UDialog* _dialog);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void SpawnAnima(bool getEP);

	UFUNCTION(BlueprintCallable)
		FCharacterData GetAnimaCharacterData();

	UFUNCTION(BlueprintCallable)
		void StartFieldDialog(UDialog* _dialog, int removeID);

	UFUNCTION(BlueprintCallable)
		void initiateCharacterSet();

	UFUNCTION(BlueprintCallable)
		bool IsStartInventoryInitiated();

	UFUNCTION(BlueprintCallable)
		void SetStartInventoryInitiated(bool val);

	UFUNCTION(BlueprintCallable)
		void WeightMapFromHere();

	UFUNCTION(BlueprintCallable)
		ANavMap* GetMap();

	UFUNCTION(BlueprintCallable)
		void WeightMapFromHereWithRange(int _range);
	UFUNCTION(BlueprintCallable)
		void WeightMapFromHereWithRangeAndSpecificWeapon(int _range, UWeaponData * _weapon);

	UFUNCTION(BlueprintCallable)
		void SetCurrentTile(UNavTile* _tile);

	UFUNCTION(BlueprintCallable)
		void ForceCurrentTile(UNavTile* _tile);

	UFUNCTION(BlueprintCallable)
		bool MoveToTile(UNavTile* _tile);

	UFUNCTION(BlueprintCallable)
	void ResetToLastTile();

	void VisualMoving(float deltaTime);

	float moveStep;
	TArray<UNavTile*> moveTilePath;
	TArray<FVector> moveVectorPath;
	void ApplyUnitStates(float deltaTime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void StatusEffectsOnStartTurnTick();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void StatusEffectsOnEndTurnTick();

	UFUNCTION(BlueprintCallable)
		void AddStatusEffect(UStatusEffectData* _data);

	UFUNCTION(BlueprintCallable)
		void AddStatusEffectAfterBattle(UStatusEffectData* _data);

	UFUNCTION(BlueprintCallable)
		void AddStatusEffectPropability(FStatusEffectPropapility _prop);

	UFUNCTION(BlueprintCallable)
		void AddStatusEffectPropabilityAfterBattle(FStatusEffectPropapility _prop);

	UFUNCTION(BlueprintCallable)
		void RemoveStatusEffect(UStatusEffectData* _data);

	UFUNCTION(BlueprintCallable)
		void LookTo(FVector _direction);
	UFUNCTION(BlueprintCallable)
		int GetCurrentEP();
	UFUNCTION(BlueprintCallable)
		int GetCurrentLevel();
	UFUNCTION(BlueprintCallable)
		int GetTotalStatValue();
	UFUNCTION(BlueprintCallable)
		int GetHP();
	UFUNCTION(BlueprintCallable)
		int GetStrength();
	UFUNCTION(BlueprintCallable)
		int GetMagic();
	UFUNCTION(BlueprintCallable)
		int GetEndurance();
	UFUNCTION(BlueprintCallable)
		int GetMana();
	UFUNCTION(BlueprintCallable)
		int GetDefence();
	UFUNCTION(BlueprintCallable)
		int GetResistence();
	UFUNCTION(BlueprintCallable)
		int GetSkill();
	UFUNCTION(BlueprintCallable)
		int GetLuck();
	UFUNCTION(BlueprintCallable)
		int GetSpeed();
	UFUNCTION(BlueprintCallable)
		int GetMovement();
	UFUNCTION(BlueprintCallable, Meta = (Bitmask, BitmaskEnum = "ETraverseType"))
		int32 GetTraverseType();
	UFUNCTION(BlueprintCallable)
		int GetCurrentHP();
	UFUNCTION(BlueprintCallable)
		int GetCurrentEndurance();
	UFUNCTION(BlueprintCallable)
		int GetCurrentMana();

	UFUNCTION(BlueprintCallable)
		int GetTotalGrowthValue();
	UFUNCTION(BlueprintCallable)
		int GetGrowthHP();
	UFUNCTION(BlueprintCallable)
		int GetGrowthStrength();
	UFUNCTION(BlueprintCallable)
		int GetGrowthMagic();
	UFUNCTION(BlueprintCallable)
		int GetGrowthEndurance();
	UFUNCTION(BlueprintCallable)
		int GetGrowthMana();
	UFUNCTION(BlueprintCallable)
		int GetGrowthDefence();
	UFUNCTION(BlueprintCallable)
		int GetGrowthResistence();
	UFUNCTION(BlueprintCallable)
		int GetGrowthSkill();
	UFUNCTION(BlueprintCallable)
		int GetGrowthLuck();
	UFUNCTION(BlueprintCallable)
		int GetGrowthSpeed();

	UFUNCTION(BlueprintCallable)
		int GetRootStrength();
	UFUNCTION(BlueprintCallable)
		int GetRootMagic();
	UFUNCTION(BlueprintCallable)
		int GetRootDefence();
	UFUNCTION(BlueprintCallable)
		int GetRootResistence();
	UFUNCTION(BlueprintCallable)
		int GetRootSkill();
	UFUNCTION(BlueprintCallable)
		int GetRootLuck();
	UFUNCTION(BlueprintCallable)
		int GetRootSpeed();



	UFUNCTION(BlueprintCallable)
		UWeaponData* GetEquippedWeapon();
	UFUNCTION(BlueprintCallable)
		void EquippWeapon(UItemInstance* _item);

	UFUNCTION(BlueprintCallable)
		UItemInstance* GetEquippedItemInstance();

	UFUNCTION(BlueprintCallable)
		TArray<UItemInstance*> GetStaffs();
	UFUNCTION(BlueprintCallable)
		TArray<UItemInstance*> GetStaffsWithUnitsInRage();

	UFUNCTION(BlueprintCallable)
		int GetPhysicalAttack();
	UFUNCTION(BlueprintCallable)
		int GetMagicalAttack();
	UFUNCTION(BlueprintCallable)
		int GetPhysicalDefence();
	UFUNCTION(BlueprintCallable)
		int GetMagicalDefence();
	UFUNCTION(BlueprintCallable)
		int GetHitChance();
	UFUNCTION(BlueprintCallable)
		int GetAvoidChance();
	UFUNCTION(BlueprintCallable)
		int GetCritChance();
	UFUNCTION(BlueprintCallable)
		int GetCritDefence();
	UFUNCTION(BlueprintCallable)
		int GetDoubleCompare();

	UFUNCTION(BlueprintCallable)
		bool CanUseHealing(UHealingItemData* _healing);
	UFUNCTION(BlueprintCallable)
		bool CanUseWeapon(UWeaponData* _weapon);
	UFUNCTION(BlueprintCallable)
		bool CanUseResourceItem(UResourceItemData* _res);

	UFUNCTION(BlueprintCallable)
		void UseStaff(UItemInstance* _staff, AUnit* _reciever);

	UFUNCTION(BlueprintCallable)
		TArray<FIntervalRange> GetAttackRange();

	UFUNCTION(BlueprintCallable)
		TArray<FIntervalRange> GetStaffRange();

	UFUNCTION(BlueprintCallable)
		bool BlocksUnitOfTeam(EUnitTeam _team, AUnit* _other);

	UFUNCTION(BlueprintCallable)
		bool CanAttackUnitOfTeam(EUnitTeam _team);

	UFUNCTION(BlueprintCallable)
		bool CanReachWithEquippedWeapon(int _range);

	UFUNCTION(BlueprintCallable)
		bool AddItem(UItemInstance* _item);

	UFUNCTION(BlueprintCallable)
		void AddAbility(TSubclassOf<UAbility> _abil);

	UFUNCTION(BlueprintCallable)
		void RemoveAbility(TSubclassOf<UAbility> _abil);

	UFUNCTION(BlueprintCallable)
		bool AddItemWithNotification(UItemInstance* _item);

	UFUNCTION(BlueprintCallable)
		int GetEPToLevel(int _level);
	UFUNCTION(BlueprintCallable)
		void AddEP(int _ep);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void LevelUp();
	UFUNCTION(BlueprintCallable)
		void AddValueToStat(EStatEnumerator _stat, int amount);
	UFUNCTION(BlueprintCallable)
		int GetStatUpWithPropability(EStatEnumerator _stat);
	UFUNCTION(BlueprintCallable)
		void AutoGenerateStatsByLevel();
	UFUNCTION(BlueprintCallable)
		void Damage(int _damage);
	UFUNCTION(BlueprintCallable)
		void Avoid();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void StartEPGain(int gainedEP);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void StartResourceChangeWindow();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void SpawnEffectAtUnit(UParticleSystem* _effect);

	UFUNCTION(BlueprintCallable)
		bool HasEndurance(int amount);
	UFUNCTION(BlueprintCallable)
		bool HasMana(int amount);
	UFUNCTION(BlueprintCallable)
		void UseEndurance(int amount);
	UFUNCTION(BlueprintCallable)
		void UseMana(int amount);
	UFUNCTION(BlueprintCallable)
		void GainEndurance(int amount);
	UFUNCTION(BlueprintCallable)
		void GainMana(int amount);

	UFUNCTION(BlueprintCallable)
		bool IsAlive();

	UFUNCTION(BlueprintCallable)
		static int GetOffenderDamage(AUnit* _offender, AUnit* _defender);

	UFUNCTION(BlueprintCallable)
		static int GetOffenderHit(AUnit* _offender, AUnit* _defender);

	UFUNCTION(BlueprintCallable)
		static int GetOffenderCrit(AUnit* _offender, AUnit* _defender);

	UFUNCTION(BlueprintCallable)
		static bool GetOffenderDouble(AUnit* _offender, AUnit* _defender);

	UFUNCTION(BlueprintCallable)
		static bool GetOffenderEffectiveness(AUnit* _offender, AUnit* _defender);

	UFUNCTION(BlueprintCallable)
		TArray<UActionOption*> GetValidActionOptions();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnRest();

	UFUNCTION(BlueprintCallable)
		void Die(AUnit* _killer);

	UFUNCTION(BlueprintCallable)
		bool isDead();

	UFUNCTION(BlueprintCallable)
		bool IsInRangeOfCurrentWeapon(AUnit* _other);

	UFUNCTION(BlueprintCallable)
		void UseItem(UItemInstance* _item);

	UFUNCTION(BlueprintCallable)
		void AddWeaponRankEP(EWeaponType _type, int _ep);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnWeaponRankUp(EWeaponType _type);

	UFUNCTION(BlueprintCallable)
		bool IsMaxWeaponRank(EWeaponType _type);

	UFUNCTION(BlueprintCallable)
		int GetNeededRankEPToRank(uint8 _rank);

	UFUNCTION(BlueprintCallable)
		EWeaponRank GetWeaponRank(EWeaponType _rank);

	UFUNCTION(BlueprintCallable)
		void Heal(int _amount);

	UFUNCTION(BlueprintCallable)
		UPerson* GetPerson();

	UFUNCTION(BlueprintCallable)
		TArray<TSubclassOf<UAbility>> GetAbilities();

	UFUNCTION(BlueprintCallable)
		void RemoveItemFrominventory(UItemInstance* _item);

	UFUNCTION(BlueprintCallable)
		int GetIndexOfItem(UItemInstance* _item);

	UFUNCTION(BlueprintCallable)
		void AddItemAtID(UItemInstance* _item, int id);

	UFUNCTION(BlueprintCallable)
		bool ItemIsUsable(UItemInstance* _item)
	{
		if (_item != nullptr && _item->item != nullptr)
		{
			if (dynamic_cast<UWeaponData*>(_item->item))
			{
				UWeaponData* wp = dynamic_cast<UWeaponData*>(_item->item);

				if (wp->damageType == EDamageType::DT_Physical)
				{
					if (!HasEndurance(wp->cost))
					{
						return false;
					}
				}
				else if (wp->damageType == EDamageType::DT_Magical)
				{
					if (!HasMana(wp->cost))
					{
						return false;
					}
				}

				return CanUseWeapon(wp);
			}
			if (dynamic_cast<UHealingItemData*>(_item->item))
			{
				UHealingItemData* hl = dynamic_cast<UHealingItemData*>(_item->item);
				return CanUseHealing(hl);
			}
			if (dynamic_cast<UResourceItemData*>(_item->item))
			{
				UResourceItemData* ri = dynamic_cast<UResourceItemData*>(_item->item);
				return CanUseResourceItem(ri);
			}
			if (dynamic_cast<UStatusEffectItemData*>(_item->item))
			{
				UStatusEffectItemData* sei = dynamic_cast<UStatusEffectItemData*>(_item->item);
				return true;
			}
			if (dynamic_cast<UStatBonusItemData*>(_item->item))
			{
				UStatBonusItemData* sbi = dynamic_cast<UStatBonusItemData*>(_item->item);
				return true;
			}
		}

		return false;
	}
};
