#ifndef KERF_DISENGAGE_ACTION_H
#define KERF_DISENGAGE_ACTION_H

#include "Action.h"

class DisengageAction : public Action {
public:
	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual void invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies);
	DisengageAction(Creature* user);
};

#endif // !KERF_DISENGAGE_ACTION_H

