// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponData.h"

EWeaponAdvantages UWeaponData::GetWeaponAdvantage(EWeaponType offender, EWeaponType defender)
{
	switch (offender)
	{
	case EWeaponType::Sword:
		switch (defender)
		{

		case EWeaponType::Lance:
			return EWeaponAdvantages::WA_Disadvantage;
			break;
		case EWeaponType::Axe:
			return EWeaponAdvantages::WA_Advantage;
			break;
		default:
			break;
		}
		break;
	case EWeaponType::Lance:
		switch (defender)
		{

		case EWeaponType::Sword:
			return EWeaponAdvantages::WA_Advantage;
			break;
		case EWeaponType::Axe:
			return EWeaponAdvantages::WA_Disadvantage;
			break;
		default:
			break;
		}
		break;
	case EWeaponType::Axe:
		switch (defender)
		{

		case EWeaponType::Lance:
			return EWeaponAdvantages::WA_Advantage;
			break;
		case EWeaponType::Sword:
			return EWeaponAdvantages::WA_Disadvantage;
			break;
		default:
			break;
		}
		break;
	case EWeaponType::Ignis:
		switch (defender)
		{

		case EWeaponType::Aer:
			return EWeaponAdvantages::WA_Advantage;
			break;
		case EWeaponType::Aqua:
			return EWeaponAdvantages::WA_Disadvantage;
			break;
		default:
			break;
		}
		break;
	case EWeaponType::Terra:
		switch (defender)
		{

		case EWeaponType::Aqua:
			return EWeaponAdvantages::WA_Advantage;
			break;
		case EWeaponType::Aer:
			return EWeaponAdvantages::WA_Disadvantage;
			break;
		default:
			break;
		}
		break;
	case EWeaponType::Aer:
		switch (defender)
		{

		case EWeaponType::Terra:
			return EWeaponAdvantages::WA_Advantage;
			break;
		case EWeaponType::Ignis:
			return EWeaponAdvantages::WA_Disadvantage;
			break;
		default:
			break;
		}
		break;
	case EWeaponType::Aqua:
		switch (defender)
		{

		case EWeaponType::Ignis:
			return EWeaponAdvantages::WA_Advantage;
			break;
		case EWeaponType::Terra:
			return EWeaponAdvantages::WA_Disadvantage;
			break;
		default:
			break;
		}
		break;
	case EWeaponType::Lux:
		break;
	case EWeaponType::Umbra:
		break;
	case EWeaponType::Staff:
		break;
	case EWeaponType::Slash:
		break;
	default:
		break;
	}

	return EWeaponAdvantages::WA_None;
}
