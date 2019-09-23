#include "AreaAttack.h"

AreaAttack::AreaAttack(Creature* user)
	: m_user(user)
	, m_type()
	, m_dmgDice()
	, m_dmgBonus(0)
	, m_dc()
	, m_saveType(DEX)
	, m_halfOnSave(false)
{}


void AreaAttack::load(DMG_TYPE type, std::string dmgString, int dc,
	ABILITY_SCORES saveType, bool halfOnSave) {
	m_type = type;

	std::function<int(void)> dummyFunc;

	m_dmgDice = funcFromStr(dmgString, m_dmgBonus, dummyFunc);
	m_dc = dc;
	m_saveType = saveType;
	m_halfOnSave = halfOnSave;
}


// dmgBonus will probably be zero for most area attacks, but it's there just in case
int AreaAttack::rollDmg() {
	return m_dmgDice() + m_dmgBonus;
}