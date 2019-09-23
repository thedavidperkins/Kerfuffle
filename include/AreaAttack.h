#ifndef KERF_AREA_ATTACK_H
#define KERF_AREA_ATTACK_H

#include "Weapons.h"
#include "enums_common.h"

class Creature;

class AreaAttack {
public:
	AreaAttack(Creature* user);
	void load(DMG_TYPE type, std::string dmgString, int dc,
		ABILITY_SCORES saveType, bool halfOnSave);

	DMG_TYPE getDmgType() { return m_type; }
	int getDC() { return m_dc; }
	ABILITY_SCORES getSaveType() { return m_saveType; }

	int rollDmg();
protected:
	Creature* m_user;

	DMG_TYPE m_type;
	std::function<int(void)> m_dmgDice;
	int m_dmgBonus;
	int m_dc;
	ABILITY_SCORES m_saveType;
	bool m_halfOnSave;
};



#endif // KERF_AREA_ATTACK_H
