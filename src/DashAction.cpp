#include "DashAction.h"
#include "Logger.h"
#include "Creature.h"

DashAction::DashAction(Creature* user) : Action(DASH, user) {}

bool DashAction::isUsable(const std::vector<Creature*>&, const std::vector<Creature*>&) {
	delete this;
	return false;

}

void DashAction::invoke(std::vector<Creature*>&, std::vector<Creature*>&) {
	LOG(m_user->getName() + " dashes.");
}
