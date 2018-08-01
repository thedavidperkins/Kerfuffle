#include "Player.h"
#include "Dice.h"
#include "Attack.h"
#include "BonusAction.h"

Player::Player() :
	Creature(),
	m_bonus(1),
	m_bonusActions(),
	m_weaponsOwned(),
	m_weaponProfs(),
	m_offhandWeapon(UNARMED),
	m_propProfs(0),
	m_hasShield(false),
	m_hasOffhandWeapon(false),
	m_hasOffhandSpecialWeapon(false),
	m_specialWeaponAttack(this),
	m_specialOffhandWeaponAttack(this),
	m_specialWeaponProps(0),
	m_specialOffhandWeaponProps(0),
	m_noMoreAttacks(false),
	m_firstAttack(true),
	m_nAttacks(1),
	m_profBonus(2)
{}

Player::Player(const Player* rhs) :
	Creature(rhs),
	m_bonus(1),
	m_bonusActions(rhs->m_bonusActions),
	m_weaponsOwned(rhs->m_weaponsOwned),
	m_weaponProfs(rhs->m_weaponProfs),
	m_offhandWeapon(rhs->m_offhandWeapon),
	m_propProfs(rhs->m_propProfs),
	m_hasShield(rhs->m_hasShield),
	m_hasOffhandWeapon(rhs->m_hasOffhandWeapon),
	m_hasOffhandSpecialWeapon(rhs->m_hasOffhandSpecialWeapon),
	m_specialWeaponAttack(this),
	m_specialOffhandWeaponAttack(this),
	m_specialWeaponProps(rhs->m_specialWeaponProps),
	m_specialOffhandWeaponProps(rhs->m_specialOffhandWeaponProps),
	m_noMoreAttacks(false),
	m_firstAttack(true),
	m_nAttacks(rhs->m_nAttacks),
	m_profBonus(rhs->m_profBonus)
{}

Player::Player(std::stringstream& defStream) : Player() {
	std::string errStatus;
	if (!_defineFromStream(defStream, errStatus)) throw std::runtime_error("Error in player definition: " + errStatus);
}

bool Player::_defineFromStream(std::stringstream& defStream, std::string& errStatus) {
	std::string line, token;
	while (defStream) {
		std::getline(defStream, line);
		std::stringstream procLine(line);
		procLine >> token;
		if (token[0] == '#') continue;
		if (token == "PLAYER") {
			procLine >> m_name;
		}
		else if (token == "ARCH") {
			procLine >> token;
			if (token == "BRAWLER") {
				m_archetype = BRAWLER;
			}
			else if (token == "ROGUE") {
				m_archetype = ROGUE;
			}
			else if (token == "RANGER") {
				m_archetype = RANGER;
			}
			else if (token == "DAMAGE_CASTER") {
				m_archetype = DAMAGE_CASTER;
			}
			else if (token == "SUPPORT_CASTER") {
				m_archetype = SUPPORT_CASTER;
			}
			else {
				errStatus = "Archetype not recognized";
				return false;
			}
		}
		else if (token == "ATTR") {
			procLine >> m_str >> m_dex >> m_con >> m_wis >> m_int >> m_cha;
		}
		else if (token == "ACDC") {
			procLine >> m_AC >> m_spellDC;
		}
		else if (token == "HP") {
			procLine >> m_maxHP;
			m_HP = m_maxHP;
		}
		else if (token == "PROFBONUS") {
			procLine >> m_profBonus;
		}
		else if (token == "SLOTS") {
			procLine
				>> m_spellSlots[0]
				>> m_spellSlots[1]
				>> m_spellSlots[2]
				>> m_spellSlots[3]
				>> m_spellSlots[4]
				>> m_spellSlots[5]
				>> m_spellSlots[6]
				>> m_spellSlots[7]
				>> m_spellSlots[8];
		}
		else if (token == "NATTACKS") {
			procLine >> m_nAttacks;
		}
		else if (token == "SPELLS") {
			std::getline(defStream, line);
			if (line.find("ENDSPELLS") != 0) {
				errStatus = "Error: spell initialization not yet implemented.";
				return false;
			}
		}
		else if (token == "WEAPONS" || token == "WPROF") {
			std::string end("ENDWEAPONS");
			std::vector<WEAPON_TYPE>& vecToFill = m_weaponsOwned;
			if (token == "WPROF") {
				end = "ENDWPROF";
				vecToFill = m_weaponProfs;
			}
			std::getline(defStream, line);
			while (line.find(end) != 0) {
				if (!defStream) {
					errStatus = "Error: met premature end of definition stream.";
					return false;
				}
				WEAPON_TYPE wep = UNARMED;
				if (!weaponFromString(line, wep)) {
					errStatus = "Error: unknown weapon type " + line;
					return false;
				}
				vecToFill.push_back(wep);
				std::getline(defStream, line);
			}
		}
		else if (token == "PPROF") {
			std::getline(defStream, line);
			while (line.find("ENDPPROF") != 0) {
				if (!defStream) {
					errStatus = "Error: met premature end of definition stream.";
					return false;
				}
				WEAPON_PROPS_BITS bit = SIMPLE;
				if (!propFromString(line, bit)) {
					errStatus = "Error: unknown weapon property " + line;
					return false;
				}
				m_propProfs |= bit;
				std::getline(defStream, line);
			}
		}
		else if (token == "ENDPLAYER") {
			return true;
		}
	}
	errStatus = "Error: no end player flag found.";
	return false;
}

Player* Player::makeCopy() {
	if (m_copy != nullptr) {
		delete m_copy;
		m_copy = nullptr;
	}
	Player* ret = new Player(this);
	m_copy = ret;
	return ret;
}

void Player::takeTurn(std::vector<Creature*>& party, std::vector<Creature*>& foes) {
	m_noMoreAttacks = false;
	m_firstAttack = true;
	m_bonus = m_spellCast = 1;
	for (auto& b : m_bonusActions) {
		if ((b->timeToUse() & START_OF_TURN) && b->isUsable(party, foes)) {
			b->invoke();
			break;
		}
	}
	Creature::takeTurn(party, foes);
	if (m_bonus) {
		for (auto& b : m_bonusActions) {
			if ((b->timeToUse() & END_OF_TURN) && b->isUsable(party, foes)) {
				b->invoke();
				break;
			}
		}
	}
}

void Player::takeDamage(Attack* attack) {
	Creature::takeDamage(attack);
	if (m_HP <= 0) {
		m_alive = false;
		m_stable = false;
		m_deathSaves = 0;
		m_deathFails = 0;
	}
	if (m_HP < (-m_maxHP / 2)) m_dead = true;
}

bool Player::deathCheck() {
	int roll = d20();
	if (roll == 1) {
		m_deathFails += 2;
	}
	else if (roll == 20) {
		m_alive = true;
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
		m_dead = true;
	}
	if (m_deathSaves == 3) {
		m_deathSaves = 0;
		m_deathFails = 0;
		m_stable = true;
	}
	return false;
}

static void eliminateWeapons(std::vector<WEAPON_TYPE>& weapons, std::function<bool(WEAPON_TYPE)> condition) {
	std::vector<WEAPON_TYPE> hold;
	for (auto& wep : weapons) {
		if (!condition(wep)) hold.push_back(wep);
	}
	weapons = hold;
}

// just choose the most damaging one for now-- intelligent 
//   choices for damage type and properties come later
WEAPON_TYPE Player::_chooseWeaponForAttack() {
	std::vector<WEAPON_TYPE> weaponsValid = m_weaponsOwned;
	if (!m_firstAttack) eliminateWeapons(weaponsValid, isLoading);
	if (m_hasShield) eliminateWeapons(weaponsValid, isTwoHanded);
	if (m_hasOffhandWeapon) eliminateWeapons(weaponsValid, [&](WEAPON_TYPE t) {
		return !isLight(t);
	});
	WEAPON_TYPE ret = mostDamage(weaponsValid);
	if (!_isProficient(ret)) {
		eliminateWeapons(weaponsValid, [&](WEAPON_TYPE t) {
			return !_isProficient(t);
		});
		WEAPON_TYPE ret2 = mostDamage(weaponsValid);
		if (ret2 != UNARMED) ret = ret2;
	}
	return ret;
}

bool Player::loadNextAttack(Attack* atk) {
	if (m_noMoreAttacks) return false;
	if (m_hasSpecialWeapon && ((!(m_specialWeaponProps & LOADING)) || m_firstAttack)) {
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
	m_firstAttack = false;
	return true;
}

int Player::_getAbltyMod(WEAPON_TYPE weapon) {
	int bonus = m_str;
	if (isRanged(weapon)) {
		bonus = m_dex;
	}
	if (isFinesse(weapon) && m_dex > m_str) {
		bonus = m_dex;
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

void Player::loadOffhandAttack(Attack& atk) {
	if (!m_hasOffhandWeapon) {
		throw std::runtime_error("Error: player has no off-hand weapon");
	}
	if (m_hasOffhandSpecialWeapon) {
		atk.load(m_specialOffhandWeaponAttack);
	}
	else {
		atk.load(atkBonus(m_offhandWeapon), gWeaponDmg[m_offhandWeapon], gDamageTypes[m_offhandWeapon]);
	}
}

void Player::getAttackList(std::vector<Attack*>& atks) {
	for (int iter = 0; iter < m_nAttacks; ++iter) {
		atks.push_back(new Attack(this));
	}
}

void Player::cleanupAttackList(std::vector<Attack*>& atks) {
	for (auto& atk : atks) {
		delete atk;
	}
	atks.clear();
}
