#include "DisengageAction.h"
#include "Logger.h"
#include "Creature.h"

DisengageAction::DisengageAction(Creature* user) : Action(DASH_ACTION, user) {}

bool DisengageAction::isUsable(const std::vector<Creature*>&, const std::vector<Creature*>&) {
	delete this;
	return false;

}

void DisengageAction::invoke(std::vector<Creature*>&, std::vector<Creature*>&) {
	LOG(m_user->getName() + " disengages.");
}
