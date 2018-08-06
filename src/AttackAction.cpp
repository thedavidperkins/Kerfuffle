#include "AttackAction.h"
#include "Attack.h"
#include "Creature.h"
#include "Logger.h"
#include "Ring.h"

AttackAction::AttackAction(Creature* user) : Action(ATTACK, user) {}

bool AttackAction::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	// check all adjacent enemies
	for (const auto& en : m_user->getAdjCreatures(enemies)) {
		if (en->isStanding()) {
			return true;
		}
	}

	// If we've made it this far, no standing enemies are adjacent 
	// (i.e. in the eight cells immediately surrounding the user's cell)

	// Next check for reach attack
	if (m_user->hasAttackProp(REACH)) {
		for (const auto& en : m_user->getCreaturesInRange(enemies, 10)) {
			if (en->isStanding()) {
				m_user->incentivizeProp(REACH);
				return true;
			}
		}
	}

	int range = m_user->getRemainingRange(); // movement range
	int atkRange = m_user->getMaxAtkRange(); // attack range

	// Next check for ranged attack
	if (m_user->hasAttackProp(RANGED)) {
		for (const auto& en : m_user->getCreaturesInRange(enemies, range + atkRange)) {
			if (en->isStanding()) {
				m_user->incentivizeProp(RANGED);
				return true;
			}
		}
	}

	// Next check if you can close in on a target (need to get within 5 feet)
	for (const auto& en : m_user->getCreaturesInRange(enemies, range + 5)) {
		if (en->isStanding()) {
			return true;
		}
	}

	// Finally, check if you can do a thrown attack
	if (m_user->hasAttackProp(THROWN)) {
		for (const auto& en : m_user->getCreaturesInRange(enemies, range + atkRange)) {
			if (en->isStanding()) {
				m_user->incentivizeProp(THROWN);
				return true;
			}
		}
	}

	delete this;
	return false;
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
