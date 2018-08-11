#ifndef KERF_SPELL_H
#define KERF_SPELL_H

#include <vector>

class Creature;

const int SPELL_LVL_COUNT = 9;

#define SPELL_DEF(className, spellName) \
	S_##spellName,

enum SPELLS {
	S_INVALID_SPELL = 0,
#include "SpellDefs.inl"
	N_SPELLS
};

class Spell {
public:
	Spell * makeSpell(SPELLS spl, Creature* user);

	Spell(SPELLS spl, Creature* user) : m_spl(spl), m_user(user) {}

	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*> enemies) = 0;
	virtual void cast() = 0;
private:
	SPELLS m_spl;
	int m_lvl; // spell level
	Creature* m_user;
	std::string m_name;
};

#define SPELL_DEF(className, spellName)																	\
class className##Spell : public Spell {																	\
public:																									\
	className##Spell(Creature* user) : Spell(S_##spellName, user) {}									\
	virtual bool isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*> enemies);	\
	virtual void cast();																				\
}

#include "SpellDefs.inl"

bool splFrmStr(const std::string& str, SPELLS spl);

#endif//KERF_SPELL_H

