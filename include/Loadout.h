#ifndef KERF_LOADOUT_H
#define KERF_LOADOUT_H

#include <vector>
#include <functional>

#include "Weapons.h"

class Player;

enum LOADOUT_PRIORITY {
	MELEE_DAMAGE,
	MELEE_DEFENSE,
	RANGED_DAMAGE
};

class Loadout {
public:
	Loadout(
		WEAPON_TYPE wep,
		Player* owner,
		bool prof,
		bool shield,
		WEAPON_TYPE dual = UNARMED,
		bool dualProf = false
	);
	Loadout(const Loadout* rhs);

	bool checkValid();
	bool isEquiv(const Loadout* other) const;
	bool hasShield() const { return m_shield; }
	bool hasOffhand() const { return m_dual; }
	int getID() const { return m_id; }
	WEAPON_PROPS getProps(bool dual = false) const;
	int getAtkBonus(bool dual = false) const;
	int getDmgBonus(bool dual = false) const;
	std::function<int(void)> getDmgDice(bool dual = false) const;
	DMG_TYPE getDmgType(bool dual = false) const;
	WEAPON_TYPE getWepType(bool dual = false) const;

	// for priority sorting
	int getScore(LOADOUT_PRIORITY priority) const;
private:
	int _getAbltyScore(WEAPON_PROPS props);
	
	WEAPON_TYPE m_wepType;
	WEAPON_TYPE m_dualWepType;
	int m_atkBonus;
	int m_dualAtkBonus;
	int m_dmgBonus;
	std::function<int(void)> m_dmgDice;
	std::function<int(void)> m_dualDmgDice;
	DMG_TYPE m_dmgType;
	DMG_TYPE m_dualDmgType;
	WEAPON_PROPS m_wepProps;
	WEAPON_PROPS m_dualWepProps;
	bool m_shield;
	bool m_dual;
	bool m_usingTwoHanded; // if is versatile

	Player* m_owner;

	// scoring parameters
	int m_maxDamage;
	int m_acBonus;
	int m_bonusMaxDamage;

	// unique identifier
	int m_id;
	static int counter;
};

#endif//KERF_LOADOUT_H
