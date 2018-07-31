#include "BonusAction.h"
#include "Creature.h"

BonusAction::BonusAction(Creature* user, TIME_TO_USE time, bool persistent) :
	m_user(user), m_timeToUse(time), m_persistent(persistent)
{
	if (m_timeToUse == 0) m_timeToUse = START_OF_TURN | END_OF_TURN;
}