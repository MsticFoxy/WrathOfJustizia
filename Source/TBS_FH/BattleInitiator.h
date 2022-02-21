// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Unit.h"
#include "BattleProtocol/BattleProtocol.h"
#include "Camera/CameraActor.h"
#include "BattleInitiator.generated.h"

UCLASS()
class TBS_FH_API ABattleInitiator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleInitiator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UBattleProtocol* protocol;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float epMult;*/

	UFUNCTION(BlueprintCallable)
		void InitiateBattle(AUnit* _offender, AUnit* _defender);

	UFUNCTION(BlueprintCallable)
		void ApplyAftermath();

	UFUNCTION(BlueprintCallable)
		int GetUnitKillEP(AUnit* _unit);

	UFUNCTION(BlueprintCallable)
		int GetUnitBattleEP(AUnit* _unit);

	

};
