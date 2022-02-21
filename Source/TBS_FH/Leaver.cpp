// Fill out your copyright notice in the Description page of Project Settings.


#include "Leaver.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ALeaver::ALeaver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALeaver::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALeaver::Tick(float DeltaTime)
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

void ALeaver::Pull()
{
	pulled = true;
	OnPulled();
}

void ALeaver::OnPulled_Implementation()
{
}

