// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryBase_PlayerPawn.h"

// Sets default values
AStoryBase_PlayerPawn::AStoryBase_PlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStoryBase_PlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStoryBase_PlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AStoryBase_PlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

