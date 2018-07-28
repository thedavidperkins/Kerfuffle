#ifndef KERF_ATTACK_ACTION_H
#define KERF_ATTACK_ACTION_H

#include "Action.h"

class AttackAction : public Action {
public:
	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual void invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies);
	AttackAction(Creature* user);
};

#endif//KERF_ATTACK_ACTION_H

