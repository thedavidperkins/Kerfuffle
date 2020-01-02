#ifndef KERF_SPELL_H
#define KERF_SPELL_H

#include <vector>
#include "Attack.h"
#include "AreaAttack.h"
#include "enums_common.h"
#include "priorities.h"

class Creature;

const int SPELL_LVL_COUNT = 9;


enum SPELL_LEVEL_BITS : uint16_t {
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


#define SPELL_DEF(_className_, _dmgString_, _dmgType_, spellName, _actionTiming_, _levelBits_, _range_, _effects ) \
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
	virtual void reset() {}

	virtual void adjustWeight(uint32_t& weight, const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
protected:
	Creature* m_user;

	std::string m_name;
	SPELLS m_spl;
	ACTION_TIMING m_actionTiming;
	SPELL_LEVELS m_lvlBits; // spell levels available
	float m_range;

	std::vector<Creature*> m_targets;
};


class AttackSpell : public Spell
{
public:
	AttackSpell(SPELL spell, Creature* user)
		: Spell(spell, user)
		, m_attack()
	{
		m_attack.load(dmgTypeFromSpl(spell), user->getSpellMod(), dmgFromSpl(spell), 0, 0, 0, m_range, m_range, effectsFromSpl(spell));
	}

	virtual ~AttackSpell() {}
	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual bool cast();
protected:
	Attack m_attack;
};

class AreaAttackSpell : public Spell
{
public:
	AreaAttackSpell(SPELL spell, Creature* user)
		: Spell(spell, user)
		, m_attack(user)
	{
		m_attack.load(dmgTypeFromSpl(spell), dmgFromSpl(spell), user->getSpellDC(), 0, 0, m_range, m_range, effectsFromSpl(spell));
	}

	virtual ~AreaAttackSpell() {}
	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	virtual bool cast();
protected:
	AreaAttack m_attack;
};


#define BEGIN_SPELL_CLASS(className, spellName)																\
class className##Spell : public Spell {																		\
public:																										\
	className##Spell(Creature* user)																		\
		: Spell(spellName, user) {}																			\
	virtual ~className##Spell();																			\
	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);	\
	virtual bool cast();

#define END_SPELL_CLASS };


#define BEGIN_ATTACK_SPELL_CLASS(className, spellName)	\
class className##Spell : public AttackSpell				\
{														\
	className##Spell(Creature* user)					\
		: AttackSpell(spellName, user) {}				\
	virtual ~className##Spell {}						

#define END_ATTACK_SPELL_CLASS END_SPELL_CLASS

//===========================================================


BEGIN_SPELL_CLASS(AcidSplash)
protected:
	AreaAttack m_attack;
END_SPELL_CLASS


//===========================================================


BEGIN_SPELL_CLASS(FireBolt)
protected:
	Attack m_attack;
END_SPELL_CLASS


//===========================================================


// Dummy definition for virtual spell functions awaiting full definition
#define SPELL_DEF_BASIC(className)	\
BEGIN_SPELL_CLASS(className)			\
END_SPELL_CLASS


SPELL_DEF_BASIC(RayOfFrost)
SPELL_DEF_BASIC(ShockingGrasp)
SPELL_DEF_BASIC(Thornwhip)
SPELL_DEF_BASIC(Grease)
SPELL_DEF_BASIC(MageArmor)
SPELL_DEF_BASIC(Shield)
SPELL_DEF_BASIC(BurningHands)
SPELL_DEF_BASIC(HealingWord)
SPELL_DEF_BASIC(MagicMissile)
SPELL_DEF_BASIC(Sleep)
SPELL_DEF_BASIC(Thunderwave)


#endif//KERF_SPELL_H

