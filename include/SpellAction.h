#ifndef KERF_SPELL_ACTION_H
#define KERF_SPELL_ACTION_H

#include "Action.h"
#include "Spell.h"


class SpellAction : public Action {
public:
	SpellAction(SPELLS spell, Creature* user, Spell* spellTracker);

	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual bool invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies);

	// in the long run priority should be set by the spell tracker
	virtual void setPriorityWeight(ARCHETYPE arch, const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);

	// No need to implement resetActionState for spell actions; this is handled by Creature::_resetSpells()
private:
	SPELLS m_spellType;
	Spell* m_spell;
};

#endif//KERF_SPELL_ACTION_H
