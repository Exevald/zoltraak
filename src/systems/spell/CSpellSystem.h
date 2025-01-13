#pragma once

#include "spells/Spells.h"

class CSpellSystem
{
public:
	static void Init();
private:
	static void UseFightSpell(int ownerId, int spellIndex);
	static void UseSpellEffect(const Spell& spell);
};
