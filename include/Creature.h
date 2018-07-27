#ifndef KERF_CREATURE_H
#define KERF_CREATURE_H

#include <vector>
#include <functional>

#include "Enums.h"

class Creature {
public:
	// ability modifiers
	int str, dex, con, wis, ntl, cha;
	
	// difficulty classes
	int ac, spell_dc;
	
	// health
	int hp, maxhp, temphp;

	// initiative -- held during turn order sort in Encounter::fight()
	int init;
	
	// advantage/disadvantage flags
	bool hasInitAdvantage, hasInitDisadvantage;

	// still alive -- dead is for tracking full deadness after death saves or massive attacks
	bool alive, dead, stable;
	int deathSaves, deathFails;

	// attack info
	int attacksPerAction, attackBonus;
	std::function<int(void)> dmgFunc;

	DMG_TYPE dmgType;

	void initRoll();
	virtual void takeTurn(std::vector<Creature*>& party, std::vector<Creature*>& foes) = 0;
	bool checkHit(int attack);
	virtual void takeDamage(int dmg, DMG_TYPE type);
	int attack();
	int doDamage();
	virtual bool deathCheck() = 0;
};

class Player : public Creature {
public:
	virtual void takeTurn(std::vector<Creature*>& party, std::vector<Creature*>& foes);
	virtual void takeDamage(int dmg, DMG_TYPE type) override;
	virtual bool deathCheck();

	virtual void useWeapon(WEAPON_TYPE wep);
};

class Foe : public Creature {
public:
	virtual void takeTurn(std::vector<Creature*>& party, std::vector<Creature*>& foes);
	virtual void takeDamage(int dmg, DMG_TYPE type) override;
	virtual bool deathCheck() { return false; }
};

#endif//KERF_CREATURE_H

