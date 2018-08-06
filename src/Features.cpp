#include <map>

#include "Features.h"
#include "Creature.h"
#include "Logger.h"
#include "Player.h"

#define FEATURE_DEF(className, bitName, bitVal) \
{ #bitName, F_##bitName },

static std::map<std::string, FEATURE_BIT> lFeatureNames{
#include "FeatureDefs.inl"
	{ "Invalid", 0 }
};

bool ftrFrmStr(const std::string& str, FEATURE_BIT& bit) {
	auto i = lFeatureNames.find(str);
	if (i == lFeatureNames.end()) return false;
	bit = i->second;
	return true;
}

FeatureTrkr* FeatureTrkr::makeTracker(FEATURE_BIT type, Creature* owner) {
	switch (type)
	{
	case F_SNEAK_ATTACK:
		return new SneakAttackTrkr(owner);
	case F_LUCKY:
	case F_BRAVE:
	default:
		return new EmptyTrkr(type, owner);
	}
}

//===========================================================================

bool SneakAttackTrkr::_targetDistracted(Creature* target, const std::vector<Creature*>& friends) {
	for (auto& c : target->getAdjCreatures(friends)) {
		if (c != m_owner && c->isStanding()) {
			return true;
		}
	}
	return false;
}

bool SneakAttackTrkr::isUsable(Creature* target, const std::vector<Creature*>& friends) {
	return (!m_used) && (m_owner->hadAdvantage() || (!m_owner->hadDisadvantage() && _targetDistracted(target, friends)));
}

int SneakAttackTrkr::use() {
	m_used = true;
	int numDice = snkAtkDiceCount(dynamic_cast<Player*>(m_owner)->getLvl());
	int snkBonus = 0;
	for (int iter = 0; iter < numDice; ++iter) {
		snkBonus += d6();
	}
	LOG(m_owner->getName() + " rolls extra " + std::to_string(numDice) + "d6 for sneak attack and gets " + std::to_string(snkBonus));
	return snkBonus;
}