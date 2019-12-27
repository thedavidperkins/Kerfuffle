#include "DisengageAction.h"
#include "Logger.h"
#include "Creature.h"


DisengageAction::DisengageAction(Creature* user) : Action(DISENGAGE_ACTION, user) {}


// Skeleton implementation. This is not yet available as an action in the sim.
bool DisengageAction::isUsable(const std::vector<Creature*>&, const std::vector<Creature*>&) {
	return false;
}


bool DisengageAction::invoke(std::vector<Creature*>&, std::vector<Creature*>&) {
	LOG(m_user->getName() + " disengages.");
	return true;
}


void DisengageAction::setPriorityWeight(ARCHETYPE, const std::vector<Creature*>&, const std::vector<Creature*>&) {
	m_weight = 0;
}