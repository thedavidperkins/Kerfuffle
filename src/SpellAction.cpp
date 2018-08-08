#include "SpellAction.h"
#include "Creature.h"
#include "Spell.h"
#include "Logger.h"

SpellAction::SpellAction(Creature* user) : Action(SPELL_ACTION, user) {}

bool SpellAction::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	m_queuedSpell = m_user->chooseSpell(friends, enemies);
	if (m_queuedSpell != nullptr) return true;

	delete this;
	return false;
}

void SpellAction::invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) {
	LOG(m_user->getName() + " casts spell " + m_queuedSpell->getName() + ".");
	m_user->usedSpell();
	m_queuedSpell->cast();
	delete this;
}