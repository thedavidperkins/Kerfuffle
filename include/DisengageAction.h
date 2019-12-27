#ifndef KERF_DISENGAGE_ACTION_H
#define KERF_DISENGAGE_ACTION_H

#include "Action.h"

class DisengageAction : public Action {
public:
	DisengageAction(Creature* user);

	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual bool invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies);

	virtual void setPriorityWeight(ARCHETYPE arch, const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
};

#endif // !KERF_DISENGAGE_ACTION_H

