#ifndef KERF_ACTION_H
#define KERF_ACTION_H

#include <vector>

class Creature;

// Do not add to this enum list without adding a corresponding class for
//	the factory function tryAction (see implementation in Action.cpp)
enum ACTION_TYPE {
	ATTACK_ACTION = 0,
	SPELL_ACTION,
	FEATURE_ACTION,
	DASH_ACTION,
	DISENGAGE_ACTION,
	DODGE_ACTION,
	N_ACTION_TYPE
};

class Action {
public:
	static Action* tryAction(ACTION_TYPE type, Creature* user);
	virtual ~Action() {}
	Action(const Action& rhs) = delete;

	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) = 0;
	virtual void invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) = 0;
protected:
	Action(ACTION_TYPE type, Creature* user);

	ACTION_TYPE m_type;
	Creature* m_user;
};

#endif//KERF_ACTION_H
