#ifndef KERF_SPELL_H
#define KERF_SPELL_H

#include <vector>
#include "Attack.h"
#include "AreaAttack.h"
#include "enums_common.h"

class Creature;

const int SPELL_LVL_COUNT = 9;


enum SPELL_LEVEL_BITS {
	CANTRIP = 0,
	L1 = 1 << 0,
	L2 = 1 << 1,
	L3 = 1 << 2,
	L4 = 1 << 3,
	L5 = 1 << 4,
	L6 = 1 << 5,
	L7 = 1 << 6,
	L8 = 1 << 7,
	L9 = 1 << 8,
};
typedef uint16_t SPELL_LEVELS;
const SPELL_LEVELS L1_AND_UP = L1 | L2 | L3 | L4 | L5 | L6 | L7 | L8 | L9;


#define SPELL_DEF(_className_, spellName, _actionTiming_, _levelBits_, _range_ ) \
	S_##spellName,

enum SPELLS {
	S_INVALID_SPELL = 0,
#include "SpellDefs.inl"
	N_SPELLS
};


bool splFrmStr(const std::string& str, SPELLS& spl);
std::string strFromSpl(SPELLS spl);
ACTION_TIMING actionTimingFromSpl(SPELLS spl);
SPELL_LEVELS levelsFromSpl(SPELLS spl);
float rangeFromSpl(SPELLS spl);


class Spell {
public:
	static Spell* makeSpell(SPELLS spl, Creature* user);

	Spell(SPELLS spl, Creature* user)
		: m_user(user)
		, m_name(strFromSpl(spl))
		, m_spl(spl)
		, m_actionTiming(actionTimingFromSpl(spl))
		, m_lvlBits(levelsFromSpl(spl))
		, m_range(rangeFromSpl(spl))
		, m_targets()
	{}
	virtual ~Spell() {}

	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) = 0;
	virtual bool cast() = 0;
	std::string getName() const { return m_name; }
	SPELLS getSpellType() const { return m_spl; }
	virtual void reset() {}																				\
protected:
	Creature* m_user;

	std::string m_name;
	SPELLS m_spl;
	ACTION_TIMING m_actionTiming;
	SPELL_LEVELS m_lvlBits; // spell levels available
	float m_range;

	std::vector<Creature*> m_targets;
};


#define BEGIN_SPELL_CLASS(className, spellName)																\
class className##Spell : public Spell {																		\
public:																										\
	className##Spell(Creature* user);																		\
	virtual ~className##Spell();																			\
	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);	\
	virtual bool cast();

#define END_SPELL_CLASS };


//===========================================================


BEGIN_SPELL_CLASS(AcidSplash, ACID_SPLASH)
protected:
	AreaAttack m_attack;
END_SPELL_CLASS


//===========================================================


// Dummy definition for virtual spell functions awaiting full definition
#define SPELL_DEF_BASIC(className, spellName )	\
BEGIN_SPELL_CLASS(className, spellName)			\
END_SPELL_CLASS


SPELL_DEF_BASIC(FireBolt, FIRE_BOLT)
SPELL_DEF_BASIC(RayOfFrost, RAY_OF_FROST)
SPELL_DEF_BASIC(ShockingGrasp, SHOCKING_GRASP)
SPELL_DEF_BASIC(Thornwhip, THORNWHIP)
SPELL_DEF_BASIC(Grease, GREASE)
SPELL_DEF_BASIC(MageArmor, MAGE_ARMOR)
SPELL_DEF_BASIC(Shield, SHIELD)
SPELL_DEF_BASIC(BurningHands, BURNING_HANDS)
SPELL_DEF_BASIC(HealingWord, HEALING_WORD)
SPELL_DEF_BASIC(MagicMissile, MAGIC_MISSILE)
SPELL_DEF_BASIC(Sleep, SLEEP)
SPELL_DEF_BASIC(Thunderwave, THUNDERWAVE)


#endif//KERF_SPELL_H

