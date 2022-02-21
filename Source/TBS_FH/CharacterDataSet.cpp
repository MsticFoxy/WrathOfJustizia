// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDataSet.h"

int FCharacterData::GetTotalStatValue()
{
	return GetHP() + GetStrength() + GetEndurance() + GetDefence() +
		GetMagic() + GetMana() + GetResistence() +
		GetLuck() + GetSkill() + GetSpeed();
}

int FCharacterData::GetHP()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return characterClass->base_HP + stat_HP;
}

int FCharacterData::GetStrength()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return characterClass->base_strength + stat_strength;
}

int FCharacterData::GetMagic()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return characterClass->base_magic + stat_magic;
}

int FCharacterData::GetEndurance()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return characterClass->base_endurance + stat_endurance;
}

int FCharacterData::GetMana()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return characterClass->base_mana + stat_mana;
}

int FCharacterData::GetDefence()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return characterClass->base_defence + stat_defence;
}

int FCharacterData::GetResistence()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return characterClass->base_resistence + stat_resistence;
}

int FCharacterData::GetSkill()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return characterClass->base_skill + stat_skill;
}

int FCharacterData::GetLuck()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return characterClass->base_luck + stat_luck;
}

int FCharacterData::GetSpeed()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return characterClass->base_speed + stat_speed;
}

int FCharacterData::GetTotalGrowthValue()
{
	return GetGrowthHP() + GetGrowthStrength() + GetGrowthEndurance() + GetGrowthDefence() +
		GetGrowthMagic() + GetGrowthMana() + GetGrowthResistence() +
		GetGrowthLuck() + GetGrowthSkill() + GetGrowthSpeed();
}

int FCharacterData::GetGrowthHP()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return growth_HP + characterClass->growth_HP;
}

int FCharacterData::GetGrowthStrength()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return growth_strength + characterClass->growth_strength;
}

int FCharacterData::GetGrowthMagic()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return growth_magic + characterClass->growth_magic;
}

int FCharacterData::GetGrowthEndurance()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return growth_endurance + characterClass->growth_endurance;
}

int FCharacterData::GetGrowthMana()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return growth_mana + characterClass->growth_mana;
}

int FCharacterData::GetGrowthDefence()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return growth_defence + characterClass->growth_defence;
}

int FCharacterData::GetGrowthResistence()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return growth_resistence + characterClass->growth_resistence;
}

int FCharacterData::GetGrowthSkill()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return growth_skill + characterClass->growth_skill;
}

int FCharacterData::GetGrowthLuck()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return growth_luck + characterClass->growth_luck;
}

int FCharacterData::GetGrowthSpeed()
{
	if (characterClass == nullptr)
	{
		return 0;
	}
	return growth_speed + characterClass->growth_speed;
}
