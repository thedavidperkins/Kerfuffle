#include "AttackAction.h"
#include "Attack.h"
#include "Creature.h"
#include "Logger.h"
#include "Ring.h"

AttackAction::AttackAction(Creature* user) : Action(ATTACK_ACTION, user) {}


bool AttackAction::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	if (!atkActionUsable(m_user, friends, enemies)) {
		return false;
	}
	return true;
}


// Even though we checked attack targets available in isUsable, we start fresh here because
//	for units with multiple attacks per action, the best target and loadout may change after
//	the first attack
bool AttackAction::invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) {
	LOG(m_user->getName() + " attacks.");
	std::vector<Attack*> atks;
	m_user->getAttackList(atks);
	bool attacked = false;
	for (auto& curAttack : atks) {
		Creature* target = m_user->chooseAttackTarget(enemies);
		if (target == nullptr) {
			break;
		}
		// The following shouldn't fail if isUsable returned true for a player,
		//	but a Foe may have multiple attacks with different ranges or other
		//	restrictions, so one may fail and another may succeed (hence the continue)
		if (!m_user->prepNextAttack(curAttack, target)) continue;
		if (!attacked)
		{
			m_user->usedAction();
			attacked = true;
		}
		if (target->checkHit(curAttack)) {
			target->takeDamage(curAttack);
		}
		else {
			LOG(m_user->getName() + " misses!");
		}
	}
	m_user->cleanupAttackList(atks);

	if (!attacked)
	{
		LOG(m_user->getName() + " wasn't able to attack.");
	}
	return attacked;
}


void AttackAction::setPriorityWeight(ARCHETYPE arch) {
	switch (arch)
	{
	case BRAWLER:
	case ROGUE:
	case RANGER:
		m_weight = 90;
		break;
	case DAMAGE_CASTER:
	case SUPPORT_CASTER:
		m_weight = 50;
		break;
	default:
		throw std::runtime_error("Error: unrecognized archetype for action priority.");
	}
}