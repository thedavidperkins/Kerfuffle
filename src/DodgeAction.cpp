#include "DodgeAction.h"
#include "Creature.h"
#include "Logger.h"


DodgeAction::DodgeAction(Creature* user) : Action(DODGE_ACTION, user) {}


bool DodgeAction::isUsable(const std::vector<Creature*>&, const std::vector<Creature*>&) {
	return true;
}


bool DodgeAction::invoke(std::vector<Creature*>&, std::vector<Creature*>&) {
	LOG(m_user->getName() + " dodges.");
	m_user->setDodge(true);
	m_user->usedAction();
	return true;
}


void DodgeAction::setPriorityWeight(ARCHETYPE, const std::vector<Creature*>&, const std::vector<Creature*>&) {
	m_weight = 5;
}