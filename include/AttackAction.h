#ifndef KERF_ATTACK_ACTION_H
#define KERF_ATTACK_ACTION_H

#include "Action.h"

class AttackAction : public Action {
public:
	AttackAction(Creature* user);

	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual bool invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies);

	virtual void setPriorityWeight(ARCHETYPE arch, const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
};

#endif//KERF_ATTACK_ACTION_H
