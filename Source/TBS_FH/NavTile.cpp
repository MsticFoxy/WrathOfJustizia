// Fill out your copyright notice in the Description page of Project Settings.


#include "NavTile.h"

UNavTile::UNavTile()
{
	
}

UNavTile::~UNavTile()
{
}

void UNavTile::init(int _x, int _y)
{
	height = 0;
	x = _x;
	y = _y;
	unit = nullptr;
}
