#include <algorithm>

#include "Player.h"
#include "Dice.h"
#include "Attack.h"
#include "BonusAction.h"
#include "Logger.h"
#include "Loadout.h"
#include "OffhandAttack.h"

Player::Player() :
	Creature(),
	m_level(1),
	m_bonus(1),
	m_bonusActions(),
	m_loadPriority(MELEE_DAMAGE),
	m_loadouts(),
	m_chosenLoadout(-1),
	m_swappedLoadout(false),
	m_noMoreAttacks(false),
	m_firstAttack(true),
	m_nAttacks(1),
	m_profBonus(2)
{}

Player::Player(const Player* rhs) :
	Creature(rhs),
	m_level(rhs->m_level),
	m_bonus(1),
	m_bonusActions(rhs->m_bonusActions), // should be okay to shallow copy this list because it should have only persistent bonus actions in initial state
	m_loadPriority(rhs->m_loadPriority),
	m_loadouts(),
	m_chosenLoadout(rhs->m_chosenLoadout),
	m_swappedLoadout(false),
	m_noMoreAttacks(false),
	m_firstAttack(true),
	m_nAttacks(rhs->m_nAttacks),
	m_profBonus(rhs->m_profBonus)
{
	for (auto& l : rhs->m_loadouts) {
		_tryInsertLoadout(new Loadout(l));
	}
}

Player::Player(std::stringstream& defStream) : Player() {
	std::string errStatus;
	if (!_defineFromStream(defStream, errStatus)) throw std::runtime_error("Error in player definition: " + errStatus);
}

Player::~Player() {
	for (auto& l : m_loadouts) {
		delete l;
	}
}

bool Player::_defineFromStream(std::stringstream& defStream, std::string& errStatus) {
	std::string line, token;
	while (defStream) {
		std::getline(defStream, line);
		std::stringstream procLine(line);
		procLine >> token;
		if (token[0] == '#') continue;
		if (token == "PLAYER") {
			procLine >> m_name >> m_level;
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
			procLine >> m_str >> m_dex >> m_con >> m_int >> m_wis >> m_cha;
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
		else if (token == "LOADOUT") {
			std::vector<WEAPON_TYPE> weaponsOwned, weaponProfs;
			WEAPON_PROPS propProfs = 0;
			bool dualWield = false;
			bool shield = false;
			while (line.find("ENDLOADOUT") != 0) {
				std::getline(defStream, line);
				if (line[0] == '#') continue;
				if (!defStream) {
					errStatus = "Error: met premature end of definition stream.";
					return false;
				}
				procLine.clear();
				procLine.str(line);
				procLine >> token;
				if (token == "WEAPONS" || token == "WPROF") {
					std::string end("ENDWEAPONS");
					std::vector<WEAPON_TYPE>* vecToFill = &weaponsOwned;
					if (token == "WPROF") {
						end = "ENDWPROF";
						vecToFill = &weaponProfs;
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
						vecToFill->push_back(wep);
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
						propProfs |= bit;
						std::getline(defStream, line);
					}
				}
				else if (token == "DUAL_WIELD") {
					dualWield = true;
				}
				else if (token == "SHIELD") {
					shield = true;
				}
			}
			_populateLoadouts(weaponsOwned, weaponProfs, propProfs, dualWield, shield);
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
	m_noMoreAttacks = m_swappedLoadout = false;
	m_firstAttack = true;
	m_bonus = m_spellCast = 1;
	for (auto& b : m_bonusActions) {
		if ((b->timeToUse() & START_OF_TURN) && b->isUsable(party, foes)) {
			b->invoke();
			break;
		}
	}
	Creature::takeTurn(party, foes);
	if (m_loadouts[0]->hasOffhand()) {
		OffhandAttack* ohAtk = new OffhandAttack(this);
		m_bonusActions.push_back(ohAtk);
	}
	if (m_bonus) {
		for (auto& b : m_bonusActions) {
			if ((b->timeToUse() & END_OF_TURN) && b->isUsable(party, foes)) {
				b->invoke();
				break;
			}
		}
	}
	_cleanupBonusActions();
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
	LOG(m_name + " rolls " + std::to_string(roll) + " on their death save.");
	if (roll == 1) {
		m_deathFails += 2;
	}
	else if (roll == 20) {
		m_alive = true;
		m_HP = 1;
		LOG(m_name + " crits on their death save and may take a turn!");
		return true;
	}
	else if (roll < 11) {
		m_deathFails += 1;
	}
	else {
		m_deathSaves += 1;
	}

	if (m_deathFails == 3) {
		LOG(m_name + " dies.");
		m_dead = true;
	}
	if (m_deathSaves == 3) {
		LOG(m_name + " stabilizes.");
		m_deathSaves = 0;
		m_deathFails = 0;
		m_stable = true;
	}
	return false;
}

void Player::_checkLoadoutSwap() {
	if (!m_swappedLoadout) {
		_sortLoadouts();
		if (m_chosenLoadout != m_loadouts[0]->getID()) {
			m_chosenLoadout = m_loadouts[0]->getID();
			m_swappedLoadout = true;
		}
	}
}

bool Player::loadNextAttack(Attack* atk) {
	if (m_noMoreAttacks) return false;
	if (!m_swappedLoadout) {
		_checkLoadoutSwap();
		if (m_swappedLoadout) {
			// don't switch to a loading weapon after first attack
			if (!m_firstAttack && (m_loadouts[0]->getProps() & LOADING)) {
				auto l = std::find_if(m_loadouts.begin(), m_loadouts.end(), [](const Loadout* check) {
					return !(check->getProps() & LOADING);
				});
				if (l == m_loadouts.end()) return false;
				else {
					std::iter_swap(l, m_loadouts.begin());
				}
			}
		}
	}
	atk->load(m_loadouts[0]);
	if (m_loadouts[0]->getProps() & LOADING) m_noMoreAttacks = true;
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

void Player::loadOffhandAttack(Attack& atk) {
	atk.load(m_loadouts[0], true);
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

static bool isProficient(WEAPON_TYPE weapon, WEAPON_PROPS propProfs, const std::vector<WEAPON_TYPE>& wepProfs) {
	if (propProfs & gWeaponProps[weapon]) return true;
	return std::find(wepProfs.begin(), wepProfs.end(), weapon) != wepProfs.end();
}

bool Player::_tryInsertLoadout(Loadout* l) {
	if (std::find_if(m_loadouts.begin(), m_loadouts.end(), [&](const Loadout* item) {
		return l->isEquiv(item);
	}) == m_loadouts.end()) {
		m_loadouts.push_back(l);
		return true;
	}
	delete l;
	return false;
}

void Player::_populateLoadouts(
	const std::vector<WEAPON_TYPE>& weps,
	const std::vector<WEAPON_TYPE>& profs,
	WEAPON_PROPS propProfs,
	bool dual,
	bool shield
) {
	for (auto p = weps.begin(); p != weps.end(); ++p) {
		WEAPON_TYPE wep = *p;
		_tryInsertLoadout(new Loadout(wep, this, isProficient(wep, propProfs, profs), false));
		if (shield) {
			_tryInsertLoadout(new Loadout(wep, this, isProficient(wep, propProfs, profs), true));
		}
		if (dual) {
			for (auto q = weps.begin(); q != weps.end(); ++q) {
				if (q == p) continue;
				_tryInsertLoadout(new Loadout(wep, this, isProficient(wep, propProfs, profs), false, *q, isProficient(*q, propProfs, profs)));
			}
		}
	}

	// choose an initial best loadout
	switch (m_archetype)
	{
	case BRAWLER:
		m_loadPriority = MELEE_DEFENSE;
		break;
	case ROGUE:
		m_loadPriority = MELEE_DAMAGE;
		break;
	case RANGER:
	case DAMAGE_CASTER:
	case SUPPORT_CASTER:
		m_loadPriority = RANGED_DAMAGE;
	default:
		break;
	}
	_sortLoadouts();
	m_chosenLoadout = m_loadouts[0]->getID();
}

void Player::_sortLoadouts() {
	std::sort(m_loadouts.begin(), m_loadouts.end(), [=](const Loadout* a, const Loadout* b) {
		return a->getScore(m_loadPriority) > b->getScore(m_loadPriority);
	});
}

void Player::_cleanupBonusActions() {
	for (auto& b : m_bonusActions) {
		if (!b->isPersistent()) {
			delete b;
			b = nullptr;
		}
	}
	m_bonusActions.erase(std::remove_if(m_bonusActions.begin(), m_bonusActions.end(), [](const BonusAction* b) {
		return b == nullptr;
	}), m_bonusActions.end());
}