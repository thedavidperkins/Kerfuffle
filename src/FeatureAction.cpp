#include "Creature.h"
#include "FeatureAction.h"

static std::vector<FEATURE_BIT> lFeaturesWithActions{
	F_LAY_ON_HANDS
};

FeatureAction::FeatureAction(Creature* user) :
	Action(FEATURE_ACTION, user),
	m_toUse(nullptr)
{}

bool FeatureAction::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	for (auto& f : lFeaturesWithActions) {
		if (f & m_user->getFeatures()) {
			m_toUse = dynamic_cast<ActionFeatureTrkr*>(m_user->getTrkr(f));
			if (m_toUse->isUsable(friends, enemies)) return true;
		}
	}

	delete this;
	return false;
}

void FeatureAction::invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) {
	m_toUse->invoke();
	m_user->usedAction();
	delete this;
}