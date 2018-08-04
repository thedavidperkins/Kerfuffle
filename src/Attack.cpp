#include "Attack.h"
#include "Creature.h"
#include "Player.h"
#include "Logger.h"
#include "Loadout.h"

Attack::Attack(Creature* agent) :
	m_agent(agent),
	m_atkBonus(0),
	m_dmgDice(d0),
	m_dmgBonus(0),
	m_dmgString(),
	m_type(BLUDGEONING),
	m_props(0),
	m_loaded(false),
	m_advantage(false),
	m_disadvantage(false),
	m_crit(false)
{}

void Attack::load(Loadout* loadout, bool dual) {
	m_props = loadout->getProps(dual);
	m_atkBonus = loadout->getAtkBonus(dual);
	m_dmgDice = loadout->getDmgDice(dual);
	m_dmgBonus = loadout->getDmgBonus(dual);
	m_dmgString = wepDmgString(loadout->getWepType(dual));
	m_type = loadout->getDmgType(dual);
	m_loaded = true;
	m_crit = false;
}

void Attack::load(DMG_TYPE type, int atkBonus, const std::string& dmgString, int dmgBonus) {
	m_props = 0;
	m_atkBonus = atkBonus;
	m_dmgDice = funcFromStr(dmgString, m_dmgBonus);
	if (dmgBonus >= 0) {
		m_dmgBonus = dmgBonus;
	}
	m_dmgString = dmgString;
	m_type = type;
	m_loaded = true;
	m_crit = false;
}

void Attack::load(const Attack& rhs) {
	m_props = rhs.m_props;
	m_atkBonus = rhs.m_atkBonus;
	m_dmgDice = rhs.m_dmgDice;
	m_dmgBonus = rhs.m_dmgBonus;
	m_dmgString = rhs.m_dmgString;
	m_type = rhs.m_type;
	m_loaded = true;
	m_crit = false;
}

void Attack::unload() {
	m_atkBonus = 0;
	m_dmgDice = d0;
	m_dmgBonus = 0;
	m_type = BLUDGEONING;
	m_props = 0;
	m_loaded = false;
	m_crit = false;
}

int Attack::atk() {
	int ret;
	if (m_advantage && !m_disadvantage) {
		ret = d20adv();
	}
	else if (!m_advantage && m_disadvantage) {
		ret = d20dis();
	}
	else {
		ret = d20();
	}
	if (ret == 1) {
		LOG(m_agent->getName() + " attacked and rolled a natural 1.");
		return -1;
	}
	if (m_crit = (ret == 20)) {
		LOG(m_agent->getName() + " attacked and rolled a natural 20!");
		return std::numeric_limits<int>::max();
	}
	ret += m_atkBonus;
	LOG(m_agent->getName() + " attacked and rolled " + std::to_string(ret));
	return ret;
}

int Attack::dmg() {
	int ret = m_dmgDice();
	LOG(m_agent->getName() + " rolls the dice in " + m_dmgString + " and gets " + std::to_string(ret));
	if (m_crit) {
		int toAdd = m_dmgDice();
		LOG(m_agent->getName() + " rolls the dice again for critting and gets " + std::to_string(toAdd));
		ret += toAdd;
		m_crit = false;
	}
	LOG(m_agent->getName() + " adds damage bonus " + std::to_string(m_dmgBonus));
	return ret + m_dmgBonus;
}

DMG_TYPE Attack::dmgType() {
	return m_type;
}

std::string Attack::getUser() const { 
	return m_agent->getName();
}