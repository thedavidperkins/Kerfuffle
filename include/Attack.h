#ifndef KERF_ATTACK_H
#define KERF_ATTACK_H

#include <functional>

#include "Action.h"
#include "Weapons.h"

class Attack {
public:
	Attack(Creature* agent);
	void load(WEAPON_TYPE weapon);
	void load(int atkBonus, std::function<int(void)> dmgFunc, DMG_TYPE type);
	void load(const Attack& rhs);
	void unload();
	int atk();
	int dmg();
	DMG_TYPE dmgType();
	bool isLoaded() { return m_loaded; }
	void setDisadvantage(bool dis) { m_disadvantage = dis; }
	void setAdvantage(bool adv) { m_advantage = adv; }

	Attack(const Attack& rhs) = delete;
	Attack& operator=(const Attack& rhs) = delete;
private:
	Creature* m_agent;
	int m_atkBonus;
	std::function<int(void)> m_dmgFunc;
	DMG_TYPE m_type;
	WEAPON_PROPS m_props;
	bool m_loaded;
	bool m_advantage;
	bool m_disadvantage;
};

#endif//KERF_ATTACK_H
