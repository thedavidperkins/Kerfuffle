#include "Spell.h"
#include "Creature.h"

#define SPELL_DEF(className, spellName) \
{ #spellName, S_##spellName },

static std::map<std::string, SPELLS> lSpellNames{
#include "SpellDefs.inl"
	{ "Invalid", N_SPELLS }
};

bool splFrmStr(const std::string& str, SPELLS spl) {
	auto i = lSpellNames.find(str);
	if (i == lSpellNames.end()) return false;
	spl = i->second;
	return true;
}

#define SPELL_DEF(className, spellName)		\
	case S_##spellName:									\
		return new className##Spell(user);

Spell* Spell::makeSpell(SPELLS spl, Creature* user) {
	switch (spl)
	{
#include "SpellDefs.inl"
	default:
		return nullptr;
	}
}


// Dummy implementations for all virtual spell functions for now, just to quiet the compiler
#define SPELL_DEF(className, spellName)																\
bool className##Spell::isUsable(const std::vector<Creature*>&, const std::vector<Creature*>& ) {	\
	return false;																					\
}																									\
void className##Spell::cast() {}																									

#include "SpellDefs.inl"