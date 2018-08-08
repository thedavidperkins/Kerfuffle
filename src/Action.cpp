#include "Action.h"
#include "AttackAction.h"
#include "SpellAction.h"
#include "DodgeAction.h"
#include "DisengageAction.h"
#include "DashAction.h"
#include "FeatureAction.h"

Action::Action(ACTION_TYPE t, Creature* user) : m_type(t), m_user(user) {}

Action* Action::tryAction(ACTION_TYPE t, Creature* user) {
	switch (t)
	{
	case FEATURE_ACTION:
		return new FeatureAction(user);
	case ATTACK_ACTION:
		return new AttackAction(user);
	case SPELL_ACTION:
		return new SpellAction(user);
	case DASH_ACTION:
		return new DashAction(user);
	case DISENGAGE_ACTION:
		return new DisengageAction(user);
	case DODGE_ACTION:
		return new DodgeAction(user);
	default:
		throw std::runtime_error("Error: Action type not supported.");
	}
}