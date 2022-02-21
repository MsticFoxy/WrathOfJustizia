// Fill out your copyright notice in the Description page of Project Settings.


#include "Treasure.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATreasure::ATreasure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATreasure::BeginPlay()
{
	Super::BeginPlay();
	
	tileMap = nullptr;
}

// Called every frame
void ATreasure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!initiated)
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

		if (tileMap != nullptr)
		{
			tile = tileMap->GetTileAtLocation(GetActorLocation());
			initiated = true;
		}
	}

}

UItemInstance * ATreasure::Open()
{
	if (item != nullptr)
	{
		UItemInstance* ret = NewObject<UItemInstance>();

		ret->item = item;
		ret->durability = item->durability;
		opened = true;

		return ret;
	}
	return nullptr;
}

