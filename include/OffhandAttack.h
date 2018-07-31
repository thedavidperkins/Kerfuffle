#ifndef KERF_OFFHAND_ATTACK_H
#define KERF_OFFHAND_ATTACK_H

#include "BonusAction.h"

class OffhandAttack : public BonusAction {
public:
	OffhandAttack(Creature* user);

	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual void invoke();
private:
	Creature* m_target;
};

#endif // !KERF_OFFHAND_ATTACK_H
