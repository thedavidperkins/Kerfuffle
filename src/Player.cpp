#include "Player.h"
#include "Dice.h"
#include "Attack.h"

void Player::takeTurn(std::vector<Creature*>& party, std::vector<Creature*>& foes) {
	m_noMoreAttacks = false;
	Creature::takeTurn(party, foes);
}

void Player::takeDamage(Attack* attack) {
	Creature::takeDamage(attack);
	if (m_HP <= 0) {
		alive = false;
		stable = false;
		m_deathSaves = 0;
		m_deathFails = 0;
	}
	if (m_HP < (-m_maxHP / 2)) dead = true;
}

bool Player::deathCheck() {
	int roll = d20();
	if (roll == 1) {
		m_deathFails += 2;
	}
	else if (roll == 20) {
		alive = true;
		m_HP = 1;
		return true;
	}
	else if (roll < 11) {
		m_deathFails += 1;
	}
	else {
		m_deathSaves += 1;
	}

	if (m_deathFails == 3) {
		dead = true;
	}
	if (m_deathSaves == 3) {
		m_deathSaves = 0;
		m_deathFails = 0;
		stable = true;
	}
	return false;
}

// just choose the most damaging one for now-- intelligent 
//   choices for damage type and properties come later
WEAPON_TYPE Player::_chooseWeaponForAttack() {
	return mostDamage(m_weaponsOwned);
}

bool Player::loadNextAttack(Attack* atk) {
	if (m_noMoreAttacks) return false;
	if (m_hasSpecialWeapon) {
		atk->load(m_specialWeaponAttack);
		if (m_specialWeaponProps & LOADING) {
			m_noMoreAttacks = true;
		}
	}
	else {
		WEAPON_TYPE wep = _chooseWeaponForAttack();
		atk->load(wep);
		m_noMoreAttacks = isLoading(wep);
	}
	return true;
}

int Player::_getAbltyMod(WEAPON_TYPE weapon) {
	int bonus = str;
	if (isRanged(weapon)) {
		bonus = dex;
	}
	if (isFinesse(weapon) && dex > str) {
		bonus = dex;
	}
	return bonus;
}

bool Player::_isProficient(WEAPON_TYPE weapon) {
	if (m_propProfs & gWeaponProps[weapon]) return true;
	return std::find(m_weaponProfs.begin(), m_weaponProfs.end(), weapon) != m_weaponProfs.end();
}

int Player::dmgBonus(WEAPON_TYPE weapon) {
	return _getAbltyMod(weapon);
}

int Player::atkBonus(WEAPON_TYPE weapon) {
	int bonus = _getAbltyMod(weapon);
	if (_isProficient(weapon)) bonus += m_profBonus;
	return bonus;
}