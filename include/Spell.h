#ifndef KERF_SPELL_H
#define KERF_SPELL_H

#include <vector>

const int SPELL_LVL_COUNT = 9;

enum SPELL_PRIORITY_BITS {
	DMG_PRIORITY = 0x1,
	SUPP_PRIORITY = 0x2,
	HEAL_PRIORITY = 0x4,
	HARASS_PRIORITY = 0x8,
	SELF_HEAL_PRIORITY = 0x10,
	SELF_SUPP_PRIORITY = 0x20
};
typedef unsigned int SPELL_PRIORITY;

enum TARGET_TYPE {
	SINGLE,
	MULTI,
	AREA
};

class Creature;

class Spell {
public:
	int lvl() const { return m_lvl; }
	SPELL_PRIORITY priority() const { return m_priority; }

	virtual void cast() = 0;
	virtual bool identifyTargets(const std::vector<Creature*>& friends, const std::vector<Creature*> enemies) = 0;
protected:
	Creature* m_user;
	std::vector<Creature*> m_targets;
	int m_lvl;
	SPELL_PRIORITY m_priority;
	TARGET_TYPE m_targetType;
};

#endif // !KERF_SPELL_H

