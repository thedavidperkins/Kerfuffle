#include <map>

#include "Features.h"
#include "Creature.h"
#include "Logger.h"
#include "Player.h"
#include "Ring.h"


static constexpr int TURNS_PER_MINUTE = 10;


#define FEATURE_DEF(className, bitName, bitVal, actionTiming) \
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


#define FEATURE_DEF(className, bitName, bitVal, actionTiming)	\
	case F_##bitName:										\
		return new className##Trkr(owner);

FeatureTrkr* FeatureTrkr::makeTracker(FEATURE_BIT type, Creature* owner) {
	switch (type)
	{
#include "FeatureDefs.inl"
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


LayOnHandsTrkr::LayOnHandsTrkr(Creature* owner)
	: ActionFeatureTrkr(F_LAY_ON_HANDS, owner)
	, m_pool(5 * dynamic_cast<Player*>(owner)->getLvl())
	, m_target(nullptr)
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


//===========================================================================


static int getRagesForBarbarianLevel(int level)
{
	if (level < 3) {
		return 2;
	}
	else if (level < 6) {
		return 3;
	}
	else if (level < 12) {
		return 4;
	}
	else if (level < 17) {
		return 5;
	}
	else if (level < 20) {
		return 6;
	}

	return std::numeric_limits<int>::max();
}


BarbarianRageTrkr::BarbarianRageTrkr(Creature* owner)
	: FeatureTrkr(F_BARBARIAN_RAGE, owner)
	, m_turnsActive(-1)
	, m_usesRemaining(getRagesForBarbarianLevel(dynamic_cast<Player *>(owner)->getLvl()))
{}


bool BarbarianRageTrkr::isUsable(const std::vector<Creature*>&, const std::vector<Creature*>& enemies) {
	if (isActive() || m_usesRemaining < 1) {
		return false;
	}

	// Don't rage if you can't get a hit this turn
	return !m_owner->getCreaturesInRange(enemies, static_cast<float>(m_owner->getSpeed())).empty();
}


bool BarbarianRageTrkr::invoke() {
	m_turnsActive = 0;
	--m_usesRemaining;
	return true;
}


void BarbarianRageTrkr::reset() {
	if (
		(++m_turnsActive > TURNS_PER_MINUTE) ||
		(!m_owner->atkdThisTurn() && !m_owner->tookDmgThisTurn())
	) {
		cancelRage();
	}
}


int BarbarianRageTrkr::getDmgBonus() {
	int level = dynamic_cast<Player *>(m_owner)->getLvl();
	if (level < 9) {
		return 2;
	}
	if (level < 16) {
		return 3;
	}
	return 4;
}


//===========================================================================


#define FEATURE_DEF(className, bitName, bitVal, actionTiming)			\
	case F_##bitName:												\
		return actionTiming;


ACTION_TIMING getFeatureActionTiming(FEATURE_BIT feature) {
	switch (feature)
	{
#include "FeatureDefs.inl"
	default:
		return false;
	}
}