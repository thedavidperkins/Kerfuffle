#ifndef KERF_DODGE_ACTION_H
#define KERF_DODGE_ACTION_H

#include "Action.h"

class DodgeAction : public Action {
public:
	DodgeAction(Creature* user);

	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual bool invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies);

	virtual void setPriorityWeight(ARCHETYPE arch);
};

#endif//KERF_DODGE_ACTION_H

