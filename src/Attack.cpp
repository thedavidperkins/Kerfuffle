#include "Attack.h"
#include "Creature.h"
#include "Player.h"

Attack::Attack(Creature* agent) :
	m_agent(agent),
	m_atkBonus(0),
	m_dmgFunc(d0),
	m_type(BLUDGEONING),
	m_props(0),
	m_loaded(false),
	m_advantage(false),
	m_disadvantage(false)
{}

void Attack::load(WEAPON_TYPE weapon) {
	Player* plyr = dynamic_cast<Player*>(m_agent);
	if (plyr == nullptr) {
		throw std::runtime_error("Error: Can't assign weapon to non-player creature.");
	}
	m_props = gWeaponProps[weapon];
	m_type = gDamageTypes[weapon];
	m_dmgFunc = [=]() {
		return gWeaponDmg[weapon]() + plyr->dmgBonus(weapon);
	};
	m_atkBonus = plyr->atkBonus(weapon);
	m_loaded = true;
}

void Attack::load(int atkBonus, std::function<int(void)> dmgFunc, DMG_TYPE type) {
	m_props = 0;
	m_atkBonus = atkBonus;
	m_dmgFunc = dmgFunc;
	m_type = type;
	m_loaded = true;
}

void Attack::load(const Attack& rhs) {
	m_props = rhs.m_props;
	m_atkBonus = rhs.m_atkBonus;
	m_dmgFunc = rhs.m_dmgFunc;
	m_type = rhs.m_type;
	m_loaded = true;
}

void Attack::unload() {
	m_atkBonus = 0;
	m_dmgFunc = d0;
	m_type = BLUDGEONING;
	m_props = 0;
	m_loaded = false;
}

int Attack::atk() {
	if (m_advantage && !m_disadvantage) {
		return d20adv() + m_atkBonus;
	}
	else if (!m_advantage && m_disadvantage) {
		return d20dis() + m_atkBonus;
	}
	else {
		return d20() + m_atkBonus;
	}
}

int Attack::dmg() {
	return m_dmgFunc();
}

DMG_TYPE Attack::dmgType() {
	return m_type;
}