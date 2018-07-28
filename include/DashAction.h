#ifndef KERF_DASH_ACTION_H
#define KERF_DASH_ACTION_H

#include "Action.h"

class DashAction : public Action {
public:
	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual void invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies);
	DashAction(Creature* user);
};

#endif // !KERF_DASH_ACTION_H

