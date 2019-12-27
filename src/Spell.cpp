#include <algorithm>
#include <vector>

#include "Creature.h"
#include "Ring.h"
#include "Spell.h"
#include "Logger.h"

#define SPELL_DEF(_className_, spellName, actionTiming, levelBits, range) \
{ #spellName, S_##spellName, actionTiming, levelBits, range },


struct SpellDetails {
	std::string name;
	SPELLS spell;
	ACTION_TIMING timing;
	SPELL_LEVELS lvls;
	float range;
};


static std::vector<SpellDetails> lSpellDetails{
#include "SpellDefs.inl"
	{ "Invalid", S_INVALID_SPELL, ACTION_TIMING_INVALID, CANTRIP, 0.f }
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


static std::vector<Creature*> getPriorityTargetsInRange(
	const Creature* agent,
	float range,
	const std::vector<Creature*>& targets,
	std::function<bool(Creature*)> meetsPriority,
	std::function<bool(Creature*, Creature*)> orderFunc )
{
	std::vector<Creature*> targetsInRange = agent->getCreaturesInRange(targets, range);
	std::sort(targetsInRange.begin(), targetsInRange.end(), orderFunc);
	std::vector<Creature*> validTargets;
	for (auto iter = targetsInRange.begin(); iter != targetsInRange.end(); --iter)
	{
		if (meetsPriority(*iter))
		{
			validTargets.push_back(*iter);
		}
		else
		{
			break;
		}
	}
	return validTargets;
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


ACTION_TIMING actionTimingFromSpl(SPELLS spl) {
	return getSpellDetails(spl).timing;
}


SPELL_LEVELS levelsFromSpl(SPELLS spl) {
	return getSpellDetails(spl).lvls;
}


float rangeFromSpl(SPELLS spl) {
	return getSpellDetails(spl).range;
}


#define SPELL_DEF(className, spellName, _actionTiming_, _levelBits_, _range_)		\
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
			en->takeDamage(dmg, m_attack.getDmgType());
		}
	}

	return true;
}


//===========================================================

FireBoltSpell::FireBoltSpell(Creature* user)
	: Spell(S_FIRE_BOLT, user)
	, m_attack(user)
{
	std::string dmgString = "1d10";
	int lvl = user->getLvl();
	if (lvl >= 17)
	{
		dmgString = "4d10";
	}
	else if (lvl >= 11)
	{
		dmgString = "3d10";
	}
	else if (lvl >= 5)
	{
		dmgString = "2d10";
	}
	m_attack.load(FIRE, user->getSpellMod(), dmgString, 0, 0, 0, 120, 120);
}


FireBoltSpell::~FireBoltSpell() {}


bool FireBoltSpell::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	float dummy1 = -1.f;
	float dummy2 = -1.f;
	float maxRange = 0.f;
	m_attack.getMinMaxDisRange(dummy1, maxRange, dummy2);
	std::vector<Creature*> validTargets =
		getPriorityTargetsInRange(m_user, maxRange + m_user->getRemainingRange(), enemies,
			[](Creature* target) {
				return target->
			},
		)
}


bool FireBoltSpell::cast() {
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
