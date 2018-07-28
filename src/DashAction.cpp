#include "DashAction.h"

DashAction::DashAction(Creature* user) : Action(DASH, user) {}

bool DashAction::isUsable(const std::vector<Creature*>&, const std::vector<Creature*>&) {
	delete this;
	return false;

}

void DashAction::invoke(std::vector<Creature*>&, std::vector<Creature*>&) {}
