#include "AttackAction.h"
#include "Attack.h"
#include "Creature.h"

AttackAction::AttackAction(Creature* user) : Action(ATTACK, user) {}

bool AttackAction::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	for (const auto& en : enemies) {
		if (en->alive) {
			return true;
		}
	}
	delete this;
	return false;
}

void AttackAction::invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) {
	m_user->usedAction();
	std::vector<Attack*> atks = m_user->getAttackList();
	for (auto& curAttack : atks) {
		Creature* target = m_user->chooseAttackTarget(enemies);
		if (!curAttack->isLoaded()) {
			if (!m_user->loadNextAttack(curAttack)) break;
		}
		if (target->checkHit(curAttack)) {
			target->takeDamage(curAttack);
		}
	}
	delete this;
}
