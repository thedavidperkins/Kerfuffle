#include <algorithm>

#include "DashAction.h"
#include "Logger.h"
#include "Creature.h"
#include "Ring.h"


DashAction::DashAction(Creature* user) :
	Action(DASH_ACTION, user),
	m_enemyToApproach(nullptr)
{}


bool DashAction::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	Ring& r = Ring::getInstance();

	// Don't dash if enemies are adjacent
	for (auto& c : m_user->getAdjCreatures(enemies)) {
		if (c->isStanding()) {
			return false;
		}
	}	
	
	// Don't dash if you can close the distance to an enemy without dashing
	float minEnemyDistance;
	m_enemyToApproach = m_user->findNearest(enemies, minEnemyDistance);
	if (m_enemyToApproach == nullptr || (minEnemyDistance <= m_user->getRemainingRange())) {
		return false;
	}

	const float standardRange = 25.f;

	// Don't rush in alone, only dash in to help friend in need
	float friendDistance;
	m_enemyToApproach->findNearest(friends, friendDistance);
	if (minEnemyDistance <= friendDistance || friendDistance > standardRange) {
		return false;
	}
	
	return true;
}


bool DashAction::invoke(std::vector<Creature*>&, std::vector<Creature*>&) {
	LOG(m_user->getName() + " dashes.");
	m_user->dash();
	m_user->moveToRangeOf(m_enemyToApproach, m_user->getSpeed() / 2);
	if (m_user->getRemainingRange() >= m_user->getSpeed())
	{
		LOG(m_user->getName() + " didn't use the dash after all.");
		m_user->cancelDash();
		return false;
	}
	m_user->usedAction();
	return true;
}


void DashAction::setPriorityWeight(ARCHETYPE, const std::vector<Creature*>&, const std::vector<Creature*>&) {
	m_weight = 10;
}