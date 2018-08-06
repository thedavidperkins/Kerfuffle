#include <algorithm>

#include "DashAction.h"
#include "Logger.h"
#include "Creature.h"
#include "Ring.h"

DashAction::DashAction(Creature* user) :
	Action(DASH, user),
	m_enemyToApproach(nullptr)
{}

bool DashAction::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	Ring& r = Ring::getInstance();
	for (auto& c : m_user->getAdjCreatures(enemies)) {
		if (c->isStanding()) {
			delete this;
			return false;
		}
	}	
	
	int minEnemyDistance;
	m_enemyToApproach = m_user->findNearest(enemies, minEnemyDistance);
	if (m_enemyToApproach == nullptr || (minEnemyDistance <= m_user->getRemainingRange())) {
		delete this;
		return false;
	}

	int friendDistance;
	m_enemyToApproach->findNearest(friends, friendDistance);
	if (minEnemyDistance <= friendDistance) { // Don't rush in alone, only dash in to help friend in need
		delete this;
		return false;
	}
	
	return true;
}

void DashAction::invoke(std::vector<Creature*>&, std::vector<Creature*>&) {
	LOG(m_user->getName() + " dashes.");
	m_user->usedAction();
	m_user->dash();
	m_user->moveToRangeOf(m_enemyToApproach, m_user->getSpeed() / 2);
	delete this;
}
