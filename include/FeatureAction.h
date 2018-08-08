#ifndef KERF_FEATURE_ACTION_H
#define KERF_FEATURE_ACTION_H

#include "Action.h"
#include "Features.h"

class FeatureAction : public Action {
public:
	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual void invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies);
	FeatureAction(Creature* user);
private:
	ActionFeatureTrkr* m_toUse;
};

#endif//KERF_FEATURE_ACTION_H

