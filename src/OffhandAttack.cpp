#include "OffhandAttack.h"
#include "Creature.h"
#include "Attack.h"
#include "Player.h"

OffhandAttack::OffhandAttack(Creature* user) : BonusAction(user, END_OF_TURN, true) {}

bool OffhandAttack::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	m_target = m_user->chooseAttackTarget(enemies);
	return m_target != nullptr;
}

void OffhandAttack::invoke() {
	Player* plyr = dynamic_cast<Player*>(m_user);
	if (plyr == nullptr) {
		throw std::runtime_error("Error: Creature not castable to player type");
	}
	Attack atk(plyr);
	plyr->loadOffhandAttack(atk);
	if (m_target->checkHit(&atk)) {
		m_target->takeDamage(&atk);
	}
	plyr->usedBonusAction();
}