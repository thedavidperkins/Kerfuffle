#include "Creature.h"
#include "FeatureAction.h"


FeatureAction::FeatureAction(FEATURE_BIT feature, Creature* user, ActionFeatureTrkr* tracker)
	: Action(FEATURE_ACTION, user)
	, m_feature(feature)
	, m_featureTracker(tracker)
{}


bool FeatureAction::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	return m_featureTracker->isUsable(friends, enemies);
}


bool FeatureAction::invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) {
	bool invoked = m_featureTracker->invoke();
	m_user->usedAction();
	return invoked;
}


void FeatureAction::setPriorityWeight(ARCHETYPE arch) {
	m_weight = 100;
}