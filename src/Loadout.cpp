#include "Loadout.h"
#include "Player.h"

int Loadout::counter = 0;

Loadout::Loadout(WEAPON_TYPE wep, Player* owner, bool prof, bool shield, WEAPON_TYPE dual, bool dualProf) :
	m_wepType(wep),
	m_dualWepType(dual),
	m_atkBonus(0),
	m_dualAtkBonus(0),
	m_dmgBonus(0),
	m_dmgDice(gWeaponDmg[wep]),
	m_dualDmgDice(gWeaponDmg[dual]),
	m_dmgType(gDamageTypes[wep]),
	m_dualDmgType(gDamageTypes[dual]),
	m_wepProps(gWeaponProps[wep]),
	m_dualWepProps(gWeaponProps[dual]),
	m_shield(shield),
	m_dual(dual != UNARMED),
	m_usingTwoHanded(false),
	m_owner(owner),
	m_maxDamage((isLoading(wep) ? 1 : owner->nAttacks()) * maxDamage(wep)),
	m_acBonus(shield ? 2 : 0),
	m_bonusMaxDamage(maxDamage(dual) / 2),
	m_id(counter++)
{
	// Determine attack/damage bonus
	int abltyScore = _getAbltyScore(m_wepProps);
	m_atkBonus = abltyScore + (prof ? owner->getProfBonus() : 0);
	m_dmgBonus = abltyScore;
	if (m_dual) {
		m_dualAtkBonus = _getAbltyScore(m_dualWepProps) + (dualProf ? owner->getProfBonus() : 0);
	}

	// check for versatile weapon
	if (!shield && !m_dual && isVersatile(wep)) {
		m_dmgDice = gVersWeaponDmg[wep];
		m_usingTwoHanded = true;
	}
}

Loadout::Loadout(const Loadout* rhs) :
	m_wepType(rhs->m_wepType),
	m_dualWepType(rhs->m_dualWepType),
	m_atkBonus(rhs->m_atkBonus),
	m_dualAtkBonus(rhs->m_dualAtkBonus),
	m_dmgBonus(rhs->m_dmgBonus),
	m_dmgDice(rhs->m_dmgDice),
	m_dualDmgDice(rhs->m_dualDmgDice),
	m_dmgType(rhs->m_dmgType),
	m_dualDmgType(rhs->m_dualDmgType),
	m_wepProps(rhs->m_wepProps),
	m_dualWepProps(rhs->m_dualWepProps),
	m_shield(rhs->m_shield),
	m_dual(rhs->m_dual),
	m_usingTwoHanded(rhs->m_usingTwoHanded),
	m_owner(rhs->m_owner),
	m_maxDamage(rhs->m_maxDamage),
	m_acBonus(rhs->m_acBonus),
	m_bonusMaxDamage(rhs->m_bonusMaxDamage),
	m_id(rhs->m_id)
{}

int Loadout::_getAbltyScore(WEAPON_PROPS props) {
	int str = m_owner->getStr();
	int dex = m_owner->getDex();
	int abltyScore = str;
	if ((props & FINESSE) && (dex > str)) {
		abltyScore = dex;
	}
	else if (props & RANGED) {
		abltyScore = dex;
	}
	return abltyScore;
}

bool Loadout::checkValid() {
	if (m_shield) {
		if (m_dual) return false;
		if (m_wepProps & TWO_HANDED) return false;
	}
	else if (m_dual && !(m_wepProps & m_dualWepProps & LIGHT)) {
		return false;
	}
	return true;
}

bool Loadout::isEquiv(const Loadout* other) const {
	return (
		m_wepType == other->m_wepType &&
		m_dualWepType == other->m_dualWepType &&
		m_atkBonus == other->m_atkBonus &&
		m_dualAtkBonus == other->m_dualAtkBonus &&
		m_dmgBonus == other->m_dmgBonus &&
		m_dmgType == other->m_dmgType &&
		m_dualDmgType == other->m_dualDmgType &&
		m_wepProps == other->m_wepProps &&
		m_dualWepProps == other->m_dualWepProps &&
		m_shield == other->m_shield &&
		m_dual == other->m_dual &&
		m_usingTwoHanded == other->m_usingTwoHanded
	);
}

int Loadout::getScore(LOADOUT_PRIORITY priority) const {
	switch (priority)
	{
	case MELEE_DAMAGE:
		return m_maxDamage + m_bonusMaxDamage + 3 * m_atkBonus - 10 * (m_wepProps & RANGED); // strongly weight attack bonus, avoid ranged
	case MELEE_DEFENSE:
		return getScore(MELEE_DAMAGE) + 10 * m_shield; // favor shield use
	case RANGED_DAMAGE:
		return m_maxDamage + m_bonusMaxDamage + 3 * m_atkBonus - 10 * (m_wepProps & MELEE); // strongly weight attack bonus, avoid melee
	default:
		return 0;
		break;
	}
}

WEAPON_PROPS Loadout::getProps(bool dual) const {
	if (dual) return m_dualWepProps;
	else return m_wepProps;
}

int Loadout::getAtkBonus(bool dual) const {
	if (dual) return m_dualAtkBonus;
	else return m_atkBonus;
}

int Loadout::getDmgBonus(bool dual) const {
	if (dual) return 0;
	else return m_dmgBonus;
}

std::function<int(void)> Loadout::getDmgDice(bool dual) const {
	if (dual) return m_dualDmgDice;
	else return m_dmgDice;
}

DMG_TYPE Loadout::getDmgType(bool dual) const {
	if (dual) return m_dualDmgType;
	else return m_dmgType;
}

WEAPON_TYPE Loadout::getWepType(bool dual) const {
	if (dual) return m_dualWepType;
	else return m_wepType;
}