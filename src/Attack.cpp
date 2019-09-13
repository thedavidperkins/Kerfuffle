#include "Attack.h"
#include "Creature.h"
#include "Player.h"
#include "Logger.h"
#include "Loadout.h"


static std::string lAttackEffectNames[N_ATTACK_EFFECT_TYPES] = {
	"POISONING",
};


Attack::Attack(Creature* agent)
	: m_agent(agent)
	, m_atkBonus(0)
	, m_dmgDice(d0)
	, m_singleDie(d0)
	, m_dmgBonus(0)
	, m_dmgString()
	, m_type(BLUDGEONING)
	, m_props(0)
	, m_curProps(0)
	, m_loaded(false)
	, m_advantage(false)
	, m_disadvantage(false)
	, m_crit(false) 
	, m_maxRange(5)
	, m_minRange(0)
	, m_disRange(5)
	, m_attackEffects()
{}


void Attack::load(Loadout* loadout, bool dual) {
	m_props = loadout->getProps(dual);
	m_atkBonus = loadout->getAtkBonus(dual);
	m_dmgDice = loadout->getDmgDice(dual);
	m_singleDie = loadout->getSingleDie(dual);
	m_dmgBonus = loadout->getDmgBonus(dual);
	if (loadout->usingTwoHanded()) m_dmgString = wepDmgStringVersatile(loadout->getWepType());
	else m_dmgString = wepDmgString(loadout->getWepType(dual));
	m_type = loadout->getDmgType(dual);
	m_loaded = true;
	m_crit = false;
	wepMinMaxDisRange(loadout->getWepType(), m_minRange, m_maxRange, m_disRange);
}


void Attack::load(DMG_TYPE type, int atkBonus, const std::string& dmgString,
	int dmgBonus,
	WEAPON_PROPS props,
	int minRange,
	int maxRange,
	int disRange,
	const std::vector<AttackEffect>& effects
) 
{
	m_props = props;
	m_atkBonus = atkBonus;
	m_dmgDice = funcFromStr(dmgString, m_dmgBonus, m_singleDie);
	if (dmgBonus >= 0) {
		m_dmgBonus = dmgBonus;
	}
	m_dmgString = dmgString;
	m_type = type;
	m_loaded = true;
	m_crit = false;
	m_minRange = minRange;
	m_maxRange = maxRange;
	m_disRange = disRange;

	m_attackEffects = effects;
}


void Attack::load(const Attack& rhs) {
	m_props = rhs.m_props;
	m_atkBonus = rhs.m_atkBonus;
	m_dmgDice = rhs.m_dmgDice;
	m_singleDie = rhs.m_singleDie;
	m_dmgBonus = rhs.m_dmgBonus;
	m_dmgString = rhs.m_dmgString;
	m_type = rhs.m_type;
	m_loaded = true;
	m_crit = false;
	m_minRange = rhs.m_minRange;
	m_maxRange = rhs.m_maxRange;
	m_disRange = rhs.m_disRange;
	m_attackEffects = rhs.m_attackEffects;
}


void Attack::unload() {
	m_atkBonus = 0;
	m_dmgDice = d0;
	m_singleDie = d0;
	m_dmgBonus = 0;
	m_type = BLUDGEONING;
	m_props = 0;
	m_loaded = false;
	m_crit = false;
	m_minRange = 0;
	m_maxRange = 0;
	m_disRange = 0;
}


int Attack::atk() {
	int ret;
	if (m_agent->getCondition() & C_POISONED)
	{
		m_disadvantage = true;
	}
	if (m_advantage && !m_disadvantage) {
		ret = m_agent->rolld20Adv(R_ATTACK_ROLL);
	}
	else if (!m_advantage && m_disadvantage) {
		ret = m_agent->rolld20Dis(R_ATTACK_ROLL);
	}
	else {
		ret = m_agent->rolld20(R_ATTACK_ROLL);
	}
	if (ret == 1) {
		LOG(m_agent->getName() + " attacked and rolled a natural 1.");
		return -1;
	}
	if (m_crit = (ret == 20)) {
		LOG(m_agent->getName() + " attacked and rolled a natural 20!");
		return std::numeric_limits<int>::max();
	}
	ret += m_atkBonus;
	LOG(m_agent->getName() + " attacked and rolled " + std::to_string(ret));
	return ret;
}


int Attack::dmg(Creature* target) {

	int baseAttack = m_dmgDice();
	LOG(m_agent->getName() + " rolls the dice in " + m_dmgString + " and gets " + std::to_string(baseAttack));
	if (m_crit) {
		int toAdd = m_dmgDice();
		LOG(m_agent->getName() + " rolls the dice again for critting and gets " + std::to_string(toAdd));
		baseAttack += toAdd;
		if (m_agent->getFeatures() & F_SAVAGE_ATTACKS) {
			toAdd = m_singleDie();
			LOG(m_agent->getName() + " rolls one more die for Savage Attacks and gets " + std::to_string(toAdd));
			baseAttack += toAdd;
		}
		m_crit = false;
	}
	LOG(m_agent->getName() + " adds damage bonus " + std::to_string(m_dmgBonus));

	// Check for sneak attack
	if (m_agent->getFeatures() & F_SNEAK_ATTACK) {
		auto sat = m_agent->getTrkr<SneakAttackTrkr>();
		if (sat->isUsable(target, *m_agent->getFriends())) {
			baseAttack += sat->use();
		}
	}

	// Apply attack hit effects
	for (const AttackEffect& effect : m_attackEffects) {
		if (effect.type == A_POISONING && !(target->getCondition() & C_POISONED)) {
			LOG(target->getName() + " makes a " + abilToString(effect.saveType) + " saving throw against effect poisoning dc " + std::to_string(effect.dc));
			if (target->savingThrow(effect.saveType, C_POISONED) < effect.dc) {
				LOG(target->getName() + " failed the saving throw and is now poisoned!");
				target->addCondition(C_POISONED);
			}
			else {
				LOG(target->getName() + " succeeded and is not poisoned!");
			}
		}
	}

	int totalAttack = baseAttack + m_dmgBonus;

	// Check for resistance-- for now only support a single type of dmg per attack
	if (target->hasDmgResistance(m_type)) {
		LOG(target->getName() + " has resistance to " + dmgTypeToString(m_type) + " and only takes half damage.");
		totalAttack /= 2;
	}

	return totalAttack;
}


DMG_TYPE Attack::dmgType() {
	return m_type;
}


std::string Attack::getUser() const { 
	return m_agent->getName();
}


void Attack::getMinMaxDisRange(int& min, int& max, int& dis) {
	min = m_minRange;
	max = m_maxRange;
	dis = m_disRange;
}


bool atkActionUsable(Creature* user, const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies, bool dual) {
	// check all adjacent enemies
	for (const auto& en : user->getAdjCreatures(enemies)) {
		if (en->isStanding()) {
			return true;
		}
	}

	// If we've made it this far, no standing enemies are adjacent 
	// (i.e. in the eight cells immediately surrounding the user's cell)

	// Next check for reach attack
	if (user->hasAttackProp(REACH, dual)) {
		for (const auto& en : user->getCreaturesInRange(enemies, 10)) {
			if (en->isStanding()) {
				if (!dual) user->incentivizeProp(REACH);
				return true;
			}
		}
	}

	float range = user->getRemainingRange(); // movement range
	int atkRange = user->getMaxAtkRange(); // attack range

										   // Next check for ranged attack
	if (user->hasAttackProp(RANGED, dual)) {
		for (const auto& en : user->getCreaturesInRange(enemies, range + atkRange)) {
			if (en->isStanding()) {
				if (!dual) user->incentivizeProp(RANGED);
				return true;
			}
		}
	}

	// Next check if you can close in on a target (need to get within 5 feet)
	for (const auto& en : user->getCreaturesInRange(enemies, range + 5)) {
		if (en->isStanding()) {
			return true;
		}
	}

	// Next check if you can close in on a target with reach (need to get within 10 feet)
	if (user->hasAttackProp(REACH, dual)) {
		for (const auto& en : user->getCreaturesInRange(enemies, range + 10)) {
			if (en->isStanding()) {
				if (!dual) user->incentivizeProp(REACH);
				return true;
			}
		}
	}

	// Finally, check if you can do a thrown attack
	if (user->hasAttackProp(THROWN, dual)) {
		for (const auto& en : user->getCreaturesInRange(enemies, range + atkRange)) {
			if (en->isStanding()) {
				if (!dual) user->incentivizeProp(THROWN);
				return true;
			}
		}
	}

	return false;
}


bool atkEffectFromString(const std::string& effectName, ATTACK_EFFECT_TYPE& effect)
{
	for (uint32_t effectIter = 0; effectIter < N_ATTACK_EFFECT_TYPES; ++effectIter)
	{
		if (effectName == lAttackEffectNames[effectIter])
		{
			effect = static_cast<ATTACK_EFFECT_TYPE>(effectIter);
			return true;
		}
	}

	effect = N_ATTACK_EFFECT_TYPES;
	return false;
}