#ifndef KERF_SPELL_ACTION_H
#define KERF_SPELL_ACTION_H

#include "Action.h"

class Spell;

class SpellAction : public Action {
public:
	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual void invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies);
	SpellAction(Creature* user);
private:
	Spell* m_queuedSpell;
};

#endif//KERF_SPELL_ACTION_H
