#include "SpellAction.h"
#include "Creature.h"
#include "Spell.h"

SpellAction::SpellAction(Creature* user) : Action(SPELL, user) {}

bool SpellAction::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	m_queuedSpell = m_user->chooseSpell(friends, enemies);
	if (m_queuedSpell != nullptr) return true;

	delete this;
	return false;
}

void SpellAction::invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) {
	m_user->usedSpell();
	m_queuedSpell->cast();
	delete this;
}