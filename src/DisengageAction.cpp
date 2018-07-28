#include "DisengageAction.h"

DisengageAction::DisengageAction(Creature* user) : Action(DASH, user) {}

bool DisengageAction::isUsable(const std::vector<Creature*>&, const std::vector<Creature*>&) {
	delete this;
	return false;

}

void DisengageAction::invoke(std::vector<Creature*>&, std::vector<Creature*>&) {}
