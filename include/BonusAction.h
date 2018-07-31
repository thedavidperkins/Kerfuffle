#ifndef KERF_BONUS_ACTION_H
#define KERF_BONUS_ACTION_H

#include <vector>

class Creature;

enum TIME_TO_USE_BITS {
	START_OF_TURN = 0x1,
	END_OF_TURN = 0x2
};
typedef unsigned int TIME_TO_USE;

class BonusAction {
public:
	BonusAction(Creature* user, TIME_TO_USE time, bool persistent);

	TIME_TO_USE timeToUse() const { return m_timeToUse; }
	bool isPersistent() const { return m_persistent; }

	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) = 0;
	virtual void invoke() = 0;
protected:
	Creature* m_user;
	TIME_TO_USE m_timeToUse;
	bool m_persistent;
};

#endif//KERF_BONUS_ACTION_H

