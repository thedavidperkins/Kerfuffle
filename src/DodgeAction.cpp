#include "DodgeAction.h"
#include "Creature.h"

DodgeAction::DodgeAction(Creature* user) : Action(DODGE, user) {}

bool DodgeAction::isUsable(const std::vector<Creature*>&, const std::vector<Creature*>&) {
	return true;
}

void DodgeAction::invoke(std::vector<Creature*>&, std::vector<Creature*>&) {
	m_user->setDodge(true);
	m_user->usedAction();
	delete this;
}
