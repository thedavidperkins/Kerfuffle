#ifndef KERF_CREATURE_H
#define KERF_CREATURE_H

#include <vector>
#include <functional>

#include "Weapons.h"
#include "Attack.h"
#include "Spell.h"

class Attack;

enum ARCHETYPE {
	BRAWLER,
	ROGUE,
	RANGER,
	DAMAGE_CASTER,
	SUPPORT_CASTER
};

class Creature {
public:
	// ability modifiers
	int str, dex, con, wis, ntl, cha;
	
	// difficulty classes
	int ac, spell_dc;
	
	// initiative -- held during turn order sort in Encounter::fight()
	int init;
	
	// still alive -- dead is for tracking full deadness after death saves or massive attacks
	bool alive, dead, stable;

	void initRoll();
	virtual void takeTurn(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) = 0;
	bool checkHit(Attack* attack);
	virtual void takeDamage(Attack* attack);
	virtual bool deathCheck() = 0;
	virtual bool loadNextAttack(Attack* atk);

	void usedSpell() { m_spellCast--; }
	void usedAction() { m_actions--; }

	Creature* chooseAttackTarget(const std::vector<Creature*>& enemies);
	virtual std::vector<Attack*> getAttackList() = 0;

	Spell* chooseSpell(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);

	void setDodge(bool d) { m_dodge = d; }
protected:
	void _setSpellPriorities(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	void _setActionPriorities(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);

	// archetype -- useful for choosing action priority (later on, for now, use universal preset)
	ARCHETYPE m_archetype;

	// action flags
	int m_actions;
	int m_bonus;
	int m_reaction;
	int m_spellCast;

	ACTION_TYPE m_actionPriorities[N_ACTION_TYPE];

	// death save counters
	int m_deathSaves;
	int m_deathFails;

	// health
	int m_maxHP;
	int m_HP;
	int m_tempHP;

	// spells
	int m_spellSlots[SPELL_LVL_COUNT];
	std::vector<Spell*> m_spellbook;
	std::vector<SPELL_PRIORITY> m_spellPriorities;

	// dodge
	bool m_dodge;
};

#endif//KERF_CREATURE_H

