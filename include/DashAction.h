#ifndef KERF_DASH_ACTION_H
#define KERF_DASH_ACTION_H

class Creature;

#include "Action.h"

class DashAction : public Action {
public:
	DashAction(Creature* user);

	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual bool invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies);
	virtual void resetActionState() { m_enemyToApproach = nullptr; }

	virtual void setPriorityWeight(ARCHETYPE arch);
private:
	Creature* m_enemyToApproach;
};

#endif // !KERF_DASH_ACTION_H

