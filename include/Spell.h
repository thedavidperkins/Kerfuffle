#ifndef KERF_SPELL_H
#define KERF_SPELL_H

#include <vector>
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


#define SPELL_DEF(_className_, spellName, _actionTiming_, _levelBits_ ) \
	S_##spellName,

enum SPELLS {
	S_INVALID_SPELL = 0,
#include "SpellDefs.inl"
	N_SPELLS
};


bool splFrmStr(const std::string& str, SPELLS& spl);
std::string strFromSpl(SPELLS spl);
SPELL_LEVELS levelsFromSpl(SPELLS spl);
ACTION_TIMING actionTimingFromSpl(SPELLS spl);


class Spell {
public:
	static Spell* makeSpell(SPELLS spl, Creature* user);

	Spell(SPELLS spl, Creature* user)
		: m_spl(spl)
		, m_lvlBits(levelsFromSpl(spl))
		, m_user(user)
		, m_name(strFromSpl(spl))
		, m_actionTiming(actionTimingFromSpl(spl))
	{}

	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) = 0;
	virtual bool cast() = 0;
	std::string getName() const { return m_name; }
	SPELLS getSpellType() const { return m_spl; }
	virtual void reset() {}																				\
private:
	SPELLS m_spl;
	SPELL_LEVELS m_lvlBits; // spell levels available
	Creature* m_user;
	std::string m_name;
	ACTION_TIMING m_actionTiming;
};


#define SPELL_DEF(className, spellName, _actionTiming_, _levelBits_)														\
class className##Spell : public Spell {																		\
public:																										\
	className##Spell(Creature* user) : Spell(S_##spellName, user) {}										\
	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);	\
	virtual bool cast();																					\
};


#include "SpellDefs.inl"


#endif//KERF_SPELL_H

