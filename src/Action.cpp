#include "Action.h"
#include "AttackAction.h"
#include "SpellAction.h"
#include "DodgeAction.h"
#include "DisengageAction.h"
#include "DashAction.h"

Action::Action(ACTION_TYPE t, Creature* user) : m_type(t), m_user(user) {}

Action* Action::tryAction(ACTION_TYPE t, Creature* user) {
	switch (t)
	{
	case ATTACK:
		return new AttackAction(user);
	case SPELL:
		return new SpellAction(user);
	case DASH:
		return new DashAction(user);
	case DISENGAGE:
		return new DisengageAction(user);
	case DODGE:
		return new DodgeAction(user);
	case SPECIAL:

	default:
		return nullptr;
	}
}