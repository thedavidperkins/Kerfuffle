#ifndef KERF_ATTACK_H
#define KERF_ATTACK_H

#include <functional>

class Loadout;

#include "Action.h"
#include "Weapons.h"

class Attack {
public:
	Attack(Creature* agent);
	void load(Loadout* loadout, bool dual = false);
	void load(DMG_TYPE type, int atkBonus, const std::string& dmgString, 
		int dmgBonus = -1,
		WEAPON_PROPS props = 0,
		int minRange = 0,
		int maxRange = 5,
		int disRange = 5
	);
	void load(const Attack& rhs);
	void unload();
	int atk();
	int dmg(Creature* target);
	DMG_TYPE dmgType();
	bool isLoaded() const { return m_loaded; }
	void setDisadvantage(bool dis) { m_disadvantage = dis; }
	void setAdvantage(bool adv) { m_advantage = adv; }
	WEAPON_PROPS getProps() const { return m_props; }
	void getMinMaxDisRange(int& min, int& max, int& dis);

	std::string getUser() const;

	Attack(const Attack& rhs) = delete;
	Attack& operator=(const Attack& rhs) = delete;
private:
	Creature* m_agent;
	int m_atkBonus;
	std::function<int(void)> m_dmgDice;
	int m_dmgBonus;
	std::string m_dmgString;
	DMG_TYPE m_type;
	WEAPON_PROPS m_props;
	bool m_loaded;
	bool m_advantage;
	bool m_disadvantage;
	bool m_crit;
	int m_maxRange;
	int m_minRange;
	int m_disRange;
};

#endif//KERF_ATTACK_H
