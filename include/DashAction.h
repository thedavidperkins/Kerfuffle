#ifndef KERF_DASH_ACTION_H
#define KERF_DASH_ACTION_H

class Creature;

#include "Action.h"

class DashAction : public Action {
public:
	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual void invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies);
	DashAction(Creature* user);
private:
	Creature* m_enemyToApproach;
};

#endif // !KERF_DASH_ACTION_H

