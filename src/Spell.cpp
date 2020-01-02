#include <algorithm>
#include <vector>

#include "Creature.h"
#include "Ring.h"
#include "Spell.h"
#include "Logger.h"

#define SPELL_DEF(_className_, dmgString, dmgType, spellName, actionTiming, levelBits, range, effects ) \
{ #spellName, dmgString, dmgType, S_##spellName, actionTiming, levelBits, range, effects },


struct SpellDetails {
	std::string name;
	std::string dmgString;
	DMG_TYPE dmgType;
	SPELLS spell;
	ACTION_TIMING timing;
	SPELL_LEVELS lvls;
	float range;
	std::vector<AttackEffect> effects;
};


static std::vector<SpellDetails> lSpellDetails{
#include "SpellDefs.inl"
	{ "Invalid", "", FORCE, S_INVALID_SPELL, ACTION_TIMING_INVALID, CANTRIP, 0.f, {} }
};


static const SpellDetails& getSpellDetails(std::function<bool(const SpellDetails&)> pred) {
	auto entry = std::find_if(lSpellDetails.begin(), lSpellDetails.end(), pred);

	if (entry != lSpellDetails.end()) {
		return *entry;
	}

	return lSpellDetails.back();
}


static const SpellDetails& getSpellDetails(SPELLS spl) {
	return getSpellDetails([spl](const SpellDetails& in) {
		return in.spell == spl;
	});
}


static const SpellDetails& getSpellDetails(std::string name) {
	return getSpellDetails([name](const SpellDetails& in) {
		return in.name == name;
	});
}


bool splFrmStr(const std::string& str, SPELLS& spl) {
	const SpellDetails& deets = getSpellDetails(str);
	if (deets.name != str) {
		spl = S_INVALID_SPELL;
		return false;
	}

	spl = deets.spell;
	return true;
}


std::string strFromSpl(SPELLS spl) {
	return getSpellDetails(spl).name;
}


std::string dmgFromSpl(SPELLS spl) {
	return getSpellDetails(spl).dmgString;
}


DMG_TYPE dmgTypeFromSpl(SPELLS spl) {
	return getSpellDetails(spl).dmgType;
}


ACTION_TIMING actionTimingFromSpl(SPELLS spl) {
	return getSpellDetails(spl).timing;
}


SPELL_LEVELS levelsFromSpl(SPELLS spl) {
	return getSpellDetails(spl).lvls;
}


float rangeFromSpl(SPELLS spl) {
	return getSpellDetails(spl).range;
}


const std::vector<AttackEffect>& effectsFromSpl(SPELLS spl) {
	return getSpellDetails(spl).effects;
}


#define SPELL_DEF(className, _dmgString_, _dmgType_, spellName, _actionTiming_, _levelBits_, _range_, _effects_ )		\
	case S_##spellName:									\
		return new className##Spell(user);

Spell* Spell::makeSpell(SPELLS spl, Creature* user) {
	switch (spl)
	{
#include "SpellDefs.inl"
	default:
		return nullptr;
	}
}


void Spell::adjustWeight(uint32_t& weight, const std::vector<Creature*>&, const std::vector<Creature*>& enemies) {
	//if((m_user->getHighestAvailableSpellLevel() & m_lvlBits))
	//TODO early exit if spell can't be cast

	auto threatList = sortCreaturesByMostThreat(enemies);
	Creature* topThreat = threatList.front();
	uint16_t threatLevel = topThreat->getThreatLevel();

	// Don't wast huge spells on near death enemies even if they are the top threat
	if (topThreat->isNearDeath()) {
		threatLevel /= 2;
	}

	// use threat to determine appropriate spell level: choose too weak a spell before too strong
	SPELL_LEVELS spellLevelBitForThreat = 1 << threatLevel;
	if (m_lvlBits & spellLevelBitForThreat) {
		// average with the weight set at the action level
		weight = (weight + TOP_PRIORITY) / 2;
	}
	else if (m_lvlBits < spellLevelBitForThreat) {
		// average with the weight set at the action level
		weight = (weight + HIGH_PRIORITY) / 2;
	}
	else {
		// average with the weight set at the action level
		weight = (weight + MID_PRIORITY) / 2;
	}
}

//===========================================================


AcidSplashSpell::AcidSplashSpell(Creature* user)
	: Spell(S_ACID_SPLASH, user)
	, m_attack(user)
{
	m_attack.load(ACID, "1d6", user->getSpellDC(), DEX, false);
}


AcidSplashSpell::~AcidSplashSpell() {}


// TODO: Prioritize hitting two targets if possible
bool AcidSplashSpell::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	m_targets.clear();

	Creature *en = m_user->chooseAttackTarget(enemies);
	if (en != nullptr) {
		if (m_range >= dist(en->getCell(), m_user->getCell())) {
			m_targets.push_back(en);
			std::vector<Creature*> adjacentEnemies = en->getAdjCreatures(enemies);
			if (!adjacentEnemies.empty()) {
				m_targets.push_back(adjacentEnemies.front());
			}
			return true;
		}
	}

	return false;
}


bool AcidSplashSpell::cast() {
	int dmg = m_attack.rollDmg();
	LOG(m_user->getName() + " casts " + m_name + " for " + std::to_string(dmg) + dmgTypeToString(m_attack.getDmgType()) + " damage.");
	for (Creature* en : m_targets) {
		if (en->savingThrow(m_attack.getSaveType(), 0, true) >= m_attack.getDC()) {
			LOG(en->getName() + " successfully saves and takes no damage.");
		}
		else {
			en->takeDamage(dmg, m_attack.getDmgType(), m_user);
		}
	}

	return true;
}


//===========================================================


bool AttackSpell::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	std::vector<Creature*> enemiesInRange =
		m_user->getCreaturesInRange(enemies, m_range + m_user->getRemainingRange());

	sortCreaturesByMostThreat(enemiesInRange);
	Creature* target = enemiesInRange.front();
	if (target != nullptr)
	{
		uint16_t mostThreat = target->getThreatLevel();
		if (mostThreat & m_lvlBits || mostThreat > m_lvlBits)
		{
			m_targets.push_back(target);
			return true;
		}
	}
	return false;
}


bool AttackSpell::cast() {
	Creature* target = m_targets.front();
	LOG(m_user->getName() + " attempts to cast " + m_name + " at " + target->getName());
	if (m_user->moveToRangeOf(m_targets.front(), m_range))
	{
		if (target->checkHit(&m_attack))
		{
			target->takeDamage(&m_attack, m_user);
		}
		return true;
	}
	LOG(m_user->getName() + " was unable to cast " + m_name);
	return false;
}

//===========================================================


// Dummy implementation for virtual spell functions awaiting full implementation
#define SPELL_SKELETON(className, spellName)														\
className##Spell::className##Spell(Creature* user) : Spell(S_##spellName, user) {}					\
className##Spell::~className##Spell() {}															\
bool className##Spell::isUsable(const std::vector<Creature*>&, const std::vector<Creature*>& ) {	\
	return false;																					\
}																									\
bool className##Spell::cast() { return false; }

SPELL_SKELETON(RayOfFrost, RAY_OF_FROST)
SPELL_SKELETON(ShockingGrasp, SHOCKING_GRASP)
SPELL_SKELETON(Thornwhip, THORNWHIP)
SPELL_SKELETON(Grease, GREASE)
SPELL_SKELETON(MageArmor, MAGE_ARMOR)
SPELL_SKELETON(Shield, SHIELD)
SPELL_SKELETON(BurningHands, BURNING_HANDS)
SPELL_SKELETON(HealingWord, HEALING_WORD)
SPELL_SKELETON(MagicMissile, MAGIC_MISSILE)
SPELL_SKELETON(Sleep, SLEEP)
SPELL_SKELETON(Thunderwave, THUNDERWAVE)
