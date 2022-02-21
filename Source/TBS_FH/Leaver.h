// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavTile.h"
#include "NavMap.h"
#include "Leaver.generated.h"

UCLASS(BlueprintType)
class TBS_FH_API ALeaver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeaver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool pulled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UNavTile* tile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ANavMap* tileMap;

	bool initiated;

	UFUNCTION(BlueprintCallable)
		void Pull();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnPulled();
};
