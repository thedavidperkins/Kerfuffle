#ifndef KERF_FEATURE_ACTION_H
#define KERF_FEATURE_ACTION_H

#include "Action.h"
#include "Features.h"

class FeatureAction : public Action {
public:
	FeatureAction(FEATURE_BIT feature, Creature* user, ActionFeatureTrkr* tracker);

	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual bool invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies);

	// in the long run priority should be set by the spell tracker
	virtual void setPriorityWeight(ARCHETYPE arch, const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);

	// No need to implement resetActionState for feature actions; this is handled by Creature::_resetFtreTrkrs()
private:
	FEATURE_BIT m_feature;
	ActionFeatureTrkr* m_featureTracker;
};

#endif//KERF_FEATURE_ACTION_H

