#include "OffhandAttack.h"
#include "Creature.h"
#include "Attack.h"
#include "Player.h"
#include "Logger.h"

OffhandAttack::OffhandAttack(Creature* user) : BonusAction(user, END_OF_TURN, false) {}

bool OffhandAttack::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	m_target = nullptr;
	if (atkActionUsable(m_user, friends, enemies, true)) {
		m_target = m_user->chooseAttackTarget(enemies);
	}
	return m_target != nullptr;
}

void OffhandAttack::invoke() {
	LOG(m_user->getName() + " takes a bonus action to attack with an offhand weapon");
	Player* plyr = dynamic_cast<Player*>(m_user);
	if (plyr == nullptr) {
		throw std::runtime_error("Error: Creature not castable to player type");
	}
	Attack atk(plyr);
	if (!plyr->loadOffhandAttack(atk, m_target)) return;
	if (m_target->checkHit(&atk)) {
		m_target->takeDamage(&atk);
	}
	else {
		LOG(m_user->getName() + " misses!");
	}
	plyr->usedBonusAction();
}