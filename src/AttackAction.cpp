#include "AttackAction.h"
#include "Attack.h"
#include "Creature.h"
#include "Logger.h"
#include "Ring.h"

AttackAction::AttackAction(Creature* user) : Action(ATTACK_ACTION, user) {}

bool AttackAction::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	if (!atkActionUsable(m_user, friends, enemies)) {
		delete this;
		return false;
	}
	return true;
}

void AttackAction::invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) {
	LOG(m_user->getName() + " attacks.");
	m_user->usedAction();
	std::vector<Attack*> atks;
	m_user->getAttackList(atks);
	for (auto& curAttack : atks) {
		Creature* target = m_user->chooseAttackTarget(enemies);
		if (target == nullptr) {
			break;
		}
		if (!m_user->prepNextAttack(curAttack, target)) continue; // don't break because Foes may be able to use subsequent attacks even if one fails
		if (target->checkHit(curAttack)) {
			target->takeDamage(curAttack);
		}
		else {
			LOG(m_user->getName() + " misses!");
		}
	}
	m_user->cleanupAttackList(atks);
	delete this;
}
