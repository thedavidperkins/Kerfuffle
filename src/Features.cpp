#include <map>

#include "Features.h"
#include "Creature.h"
#include "Logger.h"
#include "Player.h"
#include "Ring.h"

#define FEATURE_DEF(className, bitName, bitVal, isAction) \
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
	case F_LAY_ON_HANDS:
		return new LayOnHandsTrkr(owner);
	case F_RELENTLESS_ENDURANCE:
		return new RelentlessEnduranceTrkr(owner);
	case F_SNEAK_ATTACK:
		return new SneakAttackTrkr(owner);
	case F_LUCKY:
	case F_BRAVE:
	case F_SAVAGE_ATTACKS:
	case F_FEY_ANCESTRY:
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

//===========================================================================

LayOnHandsTrkr::LayOnHandsTrkr(Creature* owner) : 
	ActionFeatureTrkr(F_LAY_ON_HANDS, owner),
	m_pool(5 * dynamic_cast<Player*>(owner)->getLvl()),
	m_target(nullptr)
{}

bool LayOnHandsTrkr::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	if (m_pool < 2) return false; // don't waste your time on 2 hp or less of healing available

	// is there a need?
	std::vector<Creature*> sorted = sortCreaturesByLeastHealth(friends);
	for (auto& f : sorted) {
		if (f->getHealthLost() < f->getHP()) { // Don't waste a turn on this unless the target is bloodied
			continue;
		}
		if (f == m_owner) { // can always LoH self
			m_target = f;
			return true;
		}

		if (!m_owner->getAdjCreatures({ f }).empty()) {
			m_target = f;
			return true;
		}
		if (!m_owner->getAdjCreatures(enemies).empty()) { // don't risk an opportunity attack-- TODO: allow risk of opportunity attack for exigency or if all opportunity attacks are exhausted 
		//TODO: Allow motion when it means staying close to the enemy
			continue;
		}
		if (dist(f->getCell(), m_owner->getCell()) <= m_owner->getRemainingRange() + 5) {
			m_target = f;
			return true;
		}
	}

	return false;
}

bool LayOnHandsTrkr::invoke() {
	if (m_target != m_owner) {
		if (m_owner->getAdjCreatures({ m_target }).empty()) {
			m_owner->moveToAdjacent(m_target);
		}

		if (m_owner->getAdjCreatures({ m_target }).empty()) {
			LOG(m_owner->getName() + " failed to get close enough to use Lay on Hands on " + m_target->getName());
			m_target = nullptr;
			return false;
		}
	}

	int toHeal = m_target->getHealthLost();
	if (toHeal > m_pool) {
		toHeal = m_pool;
	}
	m_pool -= toHeal;
	m_target->healBy(toHeal);
	LOG(m_owner->getName() + " uses Lay On Hands to heal " + m_target->getName() + " for " + std::to_string(toHeal) + " HP.");
	m_target = nullptr;
	return true;
}


#define FEATURE_DEF(className, bitName, bitVal, isAction)			\
	case F_##bitName:												\
		return isAction;


bool isActionFeature(FEATURE_BIT feature) {
	switch (feature)
	{
#include "FeatureDefs.inl"
	default:
		return false;
	}
}