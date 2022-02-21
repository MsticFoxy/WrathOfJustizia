// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassData.h"



int UClassData::GetTotalStatValue()
{

	return base_HP +
		base_strength + base_endurance + base_defence +
		base_magic + base_mana + base_resistence +
		base_luck + base_skill + base_speed;

}


