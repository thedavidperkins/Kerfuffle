#include "Loadout.h"
#include "Player.h"
#include "Creature.h"
#include "Ring.h"

int Loadout::counter = 0;

Loadout::Loadout(WEAPON_TYPE wep, Player* owner, bool prof, bool shield, WEAPON_TYPE dual, bool dualProf, bool usingTwoHanded) :
	m_wepType(wep),
	m_dualWepType(dual),
	m_atkBonus(0),
	m_dualAtkBonus(0),
	m_dmgBonus(0),
	m_dmgDice(gWeaponDmg[wep]),
	m_singleDie(wepSingleDie(wep)),
	m_dualDmgDice(gWeaponDmg[dual]),
	m_dualSingleDie(wepSingleDie(dual)),
	m_dmgType(gDamageTypes[wep]),
	m_dualDmgType(gDamageTypes[dual]),
	m_wepProps(gWeaponProps[wep]),
	m_dualWepProps(gWeaponProps[dual]),
	m_shield(shield),
	m_dual(dual != UNARMED),
	m_usingTwoHanded(usingTwoHanded),
	m_owner(owner),
	m_maxDamage((isLoading(wep) ? 1 : owner->nAttacks()) * maxDamage(wep)),
	m_acBonus(shield ? 2 : 0),
	m_bonusMaxDamage(m_dual ? maxDamage(dual) : 0),
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
	if (m_usingTwoHanded && (m_wepProps & VERSATILE)) {
		if (shield || m_dual) {
			throw std::runtime_error("Can't use a two-handed weapon with shield or off-hand");
		}
		if (m_wepProps & THROWN) m_wepProps -= THROWN;
		m_dmgDice = gVersWeaponDmg[wep];
		m_maxDamage += 2;
	}
}

Loadout::Loadout(const Loadout* rhs) :
	m_wepType(rhs->m_wepType),
	m_dualWepType(rhs->m_dualWepType),
	m_atkBonus(rhs->m_atkBonus),
	m_dualAtkBonus(rhs->m_dualAtkBonus),
	m_dmgBonus(rhs->m_dmgBonus),
	m_dmgDice(rhs->m_dmgDice),
	m_singleDie(rhs->m_singleDie),
	m_dualDmgDice(rhs->m_dualDmgDice),
	m_dualSingleDie(rhs->m_dualSingleDie),
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
	int str = m_owner->getAMod(STR);
	int dex = m_owner->getAMod(DEX);
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
		m_usingTwoHanded == other->m_usingTwoHanded &&
		m_maxDamage == other->m_maxDamage
	);
}

int Loadout::getScore(Creature* target, LOADOUT_PRIORITY priority, WEAPON_PROPS propsIncentives) const {
	switch (priority)
	{
	case P_DAMAGE: {
		int ret = 3 * m_maxDamage;
		ret += 2 * m_bonusMaxDamage;
		ret += 5 * m_atkBonus;
		if (!(propsIncentives & RANGED)) {
			ret -= (m_wepProps & RANGED ? 20 : 0);
		}
		if (target != nullptr) { // check appropriate range of weapon to hit target
			int min, max, dis;
			wepMinMaxDisRange(m_wepType, min, max, dis);
			float distance = dist(m_owner->getCell(), target->getCell());
			float moveRange = m_owner->getRemainingRange();
			if (distance < min) ret -= 15;
			if (distance - moveRange > max) ret -= 15;
			if (distance - moveRange > dis) ret -= 85;
		}
		ret += 20 * _countIncentivesSatisfied(propsIncentives);

		return ret;
	}
	case P_DEFENSE:
		return getScore(target, P_DAMAGE, propsIncentives) + 40 * m_shield; // favor shield use
	default:
		return 0;
		break;
	}
}

int Loadout::_countIncentivesSatisfied(WEAPON_PROPS incentives) const {
	WEAPON_PROPS intersection = incentives & m_wepProps;
	int ret = 0;
	while (intersection > 0) {
		ret += intersection & 1;
		intersection >>= 1;
	}
	return ret;
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

std::function<int(void)> Loadout::getSingleDie(bool dual) const {
	if (dual) return m_dualSingleDie;
	else return m_singleDie;
}

DMG_TYPE Loadout::getDmgType(bool dual) const {
	if (dual) return m_dualDmgType;
	else return m_dmgType;
}

WEAPON_TYPE Loadout::getWepType(bool dual) const {
	if (dual) return m_dualWepType;
	else return m_wepType;
}