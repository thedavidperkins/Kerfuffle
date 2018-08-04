#ifndef KERF_CREATURE_H
#define KERF_CREATURE_H

#include <vector>
#include <functional>
#include <sstream>

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
	virtual ~Creature();

	void initRoll();
	virtual void takeTurn(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) = 0;
	bool checkHit(Attack* attack);
	virtual void takeDamage(Attack* attack);
	virtual bool deathCheck() = 0;
	virtual bool loadNextAttack(Attack* atk);

	void usedSpell() { m_spellCast--; }
	void usedAction() { m_actions--; }

	bool isStanding() const { return m_alive; }
	bool isDead() const { return m_dead; }
	bool isStable() const { return m_stable; }
	int getInit() const { return m_init; }
	int getStr() const { return m_str; }
	int getDex() const { return m_dex; }
	int getCon() const { return m_con; }
	int getInt() const { return m_int; }
	int getWis() const { return m_wis; }
	int getCha() const { return m_cha; }
	std::string getName() const { return m_name; }

	Creature* chooseAttackTarget(const std::vector<Creature*>& enemies);
	virtual void getAttackList(std::vector<Attack*>& atks) = 0;
	virtual void cleanupAttackList(std::vector<Attack*>& atks) = 0;

	Spell* chooseSpell(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);

	void setDodge(bool d) { m_dodge = d; }

	virtual Creature* makeCopy() = 0;
protected:
	Creature();
	Creature(const Creature* rhs);

	virtual bool _defineFromStream(std::stringstream& defStream, std::string& errStatus) = 0;
	void _setSpellPriorities(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	void _setActionPriorities(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);

	std::string m_name;

	// ability modifiers
	int m_str;
	int m_dex;
	int m_con;
	int m_int;
	int m_wis;
	int m_cha;

	// difficulty classes
	int m_AC;
	int m_spellDC;

	// initiative -- held during turn order sort in Encounter::fight()
	int m_init;

	// standing, dead, or on the ground stabilized
	bool m_alive;
	bool m_dead;
	bool m_stable;

	// archetype -- useful for choosing action priority (later on, for now, use universal preset)
	ARCHETYPE m_archetype;

	// action flags
	int m_actions;
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

	// used by prototype copies to manage used copies
	Creature* m_copy;
};

#endif//KERF_CREATURE_H

