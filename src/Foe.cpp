#include "Foe.h"

Foe::Foe() : 
	Creature(),
	m_attacks()
{}

Foe::Foe(const Foe* rhs) :
	Creature(rhs),
	m_attacks()
{
	for (auto& atk : rhs->m_attacks) {
		Attack* cpyAtk = new Attack(this);
		cpyAtk->load(*atk);
		m_attacks.push_back(cpyAtk);
	}
}

Foe::Foe(std::stringstream& defStream) : Foe() {
	std::string errStatus;
	if (!_defineFromStream(defStream, errStatus)) throw std::runtime_error("Error in foe definition: " + errStatus);
}

Foe::~Foe() {
	for (auto& atk : m_attacks) {
		if (atk != nullptr) {
			delete atk;
			atk = nullptr;
		}
	}
}

bool Foe::_defineFromStream(std::stringstream& defStream, std::string& errStatus) {
	std::string line, token;
	while (defStream) {
		std::getline(defStream, line);
		std::stringstream procLine(line);
		procLine >> token;
		if (token == "FOE") {
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
		else if (token == "SPELLS") {
			std::getline(defStream, line);
			if (line.find("ENDSPELLS") != 0) {
				errStatus = "Error: spell initialization not yet implemented.";
				return false;
			}
		}
		else if (token == "ATK") {
			int atkBonus = 0;
			int dmgBonus = 0;
			std::function<int(void)> dmgRoll = d0;
			DMG_TYPE type = BLUDGEONING;
			while (line.find("ENDATK") != 0) {
				std::getline(defStream, line);
				if (!defStream) {
					errStatus = "Error: Stream ended during attack definition.";
					return false;
				}
				procLine.clear();
				procLine.str(line);
				procLine >> token;
				if (token == "ATKBONUS") {
					procLine >> atkBonus;
				}
				else if (token == "DMGROLL") {
					procLine >> token;
					dmgRoll = funcFromStr(token);
				}
				else if (token == "TYPE") {
					procLine >> token;
					if (!dmgTypeFromString(token, type)) {
						errStatus = "Invalid damage type: " + token;
						return false;
					}
				}
				else if (token == "ENDATK") {
					Attack* atk = new Attack(this);
					atk->load(atkBonus, dmgRoll, type);
					m_attacks.push_back(atk);
				}
			}
		}
		else if (token == "ENDFOE") {
			return true;
		}
	}
	errStatus = "Error: no end foe flag found.";
	return false;
}

void Foe::getAttackList(std::vector<Attack*>& atks) {
	atks = m_attacks;
}

void Foe::cleanupAttackList(std::vector<Attack*>& atks) {}

Foe* Foe::makeCopy() {
	if (m_copy != nullptr) {
		delete m_copy;
		m_copy = nullptr;
	}
	Foe* ret = new Foe(this);
	m_copy = ret;
	return ret;
}

void Foe::takeTurn(std::vector<Creature*>& party, std::vector<Creature*>& foes) {
	m_spellCast = 1;
	Creature::takeTurn(foes, party);
}

void Foe::takeDamage(Attack* attack) {
	Creature::takeDamage(attack);
	if (m_HP <= 0) {
		m_alive = false;
		m_dead = true;
	}
}