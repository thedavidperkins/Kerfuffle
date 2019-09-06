#include "SpellAction.h"
#include "Creature.h"
#include "Spell.h"
#include "Logger.h"

SpellAction::SpellAction(Creature* user) : Action(SPELL_ACTION, user) {}

bool SpellAction::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	delete this;
	return false;
}

void SpellAction::invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) {
	LOG(m_user->getName() + " casts spell " + m_queuedSpell->getName() + ".");
	m_user->usedSpell();
	m_queuedSpell->cast();
	delete this;
}