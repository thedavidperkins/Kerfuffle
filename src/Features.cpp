#include <map>

#include "Features.h"
#include "Creature.h"
#include "Logger.h"
#include "Player.h"

static std::map<std::string, FEATURE_BIT> lFeatureNames{
	{ "LUCKY", F_LUCKY },
	{ "BRAVE", F_BRAVE },
	{ "SNEAK_ATTACK", F_SNEAK_ATTACK }
//	{ "HALFLING_NIMBLENESS", F_HALFLING_NIMBLENESS }
//  { "NATURALLY_STEALTHY", F_NATURALLY_STEALTHY }
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

bool SneakAttackTrkr::_targetDistracted(Creature* target) {
	return true;
}

bool SneakAttackTrkr::isUsable(Creature* target) {
	return (!m_used) && (m_owner->hadAdvantage() || _targetDistracted(target));
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