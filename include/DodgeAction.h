#ifndef KERF_DODGE_ACTION_H
#define KERF_DODGE_ACTION_H

#include "Action.h"

class DodgeAction : public Action {
public:
	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual void invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies);
	DodgeAction(Creature* user);
};

#endif//KERF_DODGE_ACTION_H

