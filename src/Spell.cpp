#include <algorithm>
#include <vector>

#include "Spell.h"
#include "Creature.h"

#define SPELL_DEF(_className_, spellName, actionTiming, levelBits) \
{ #spellName, S_##spellName, actionTiming, levelBits },


struct SpellDetails {
	std::string name;
	SPELLS spell;
	ACTION_TIMING timing;
	SPELL_LEVELS lvls;
};


static std::vector<SpellDetails> lSpellDetails{
#include "SpellDefs.inl"
	{ "Invalid", S_INVALID_SPELL, ACTION_TIMING_INVALID, CANTRIP }
};


static const SpellDetails& getSpellDetails(std::function<bool(const SpellDetails&)> pred) {
	auto entry = std::find_if(lSpellDetails.begin(), lSpellDetails.end(), pred);

	if (entry != lSpellDetails.end()) {
		return *entry;
	}

	return lSpellDetails.back();
}


static const SpellDetails& getSpellDetails(SPELLS spl) {
	return getSpellDetails([spl](const SpellDetails& in) {
		return in.spell == spl;
	});
}


static const SpellDetails& getSpellDetails(std::string name) {
	return getSpellDetails([name](const SpellDetails& in) {
		return in.name == name;
	});
}



bool splFrmStr(const std::string& str, SPELLS& spl) {
	const SpellDetails& deets = getSpellDetails(str);
	if (deets.name != str) {
		spl = S_INVALID_SPELL;
		return false;
	}

	spl = deets.spell;
	return true;
}


std::string strFromSpl(SPELLS spl) {
	return getSpellDetails(spl).name;
}


SPELL_LEVELS levelsFromSpl(SPELLS spl) {
	return getSpellDetails(spl).lvls;
}


ACTION_TIMING actionTimingFromSpl(SPELLS spl) {
	return getSpellDetails(spl).timing;
}


#define SPELL_DEF(className, spellName, _actionTiming_, _levelBits_)		\
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
#define SPELL_DEF(className, _spellName_, _actionTiming_, _levelBits_)																\
bool className##Spell::isUsable(const std::vector<Creature*>&, const std::vector<Creature*>& ) {	\
	return false;																					\
}																									\
bool className##Spell::cast() { return false; }																									

#include "SpellDefs.inl"
