#include "Action.h"
#include "AttackAction.h"
#include "SpellAction.h"
#include "DodgeAction.h"
#include "DisengageAction.h"
#include "DashAction.h"
#include "FeatureAction.h"


Action::Action(ACTION_TYPE t, Creature* user) : m_type(t), m_user(user) {}


Action* Action::newAction(ACTION_TYPE t, Creature* user) {
	switch (t)
	{
	case ATTACK_ACTION:
		return new AttackAction(user);
	case DASH_ACTION:
		return new DashAction(user);
	case DISENGAGE_ACTION:
		return new DisengageAction(user);
	case DODGE_ACTION:
		return new DodgeAction(user);
	case SPELL_ACTION:
		throw std::runtime_error("Error: Cannot create spell action through newAction. Use SpellAction constructor.");
	case FEATURE_ACTION:
		throw std::runtime_error("Error: Cannot create feature action through newAction. Use FeatureAction constructor.");
	default:
		throw std::runtime_error("Error: Action type not supported.");
	}
}