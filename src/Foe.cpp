#include "Attack.h"
#include "Foe.h"
#include "Ring.h"


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
			procLine >> m_name >> m_level;
		}
		else if (token == "SPEED") {
			procLine >> m_speed;
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
			procLine
				>> m_abilityMods[STR]
				>> m_abilityMods[DEX]
				>> m_abilityMods[CON]
				>> m_abilityMods[INT]
				>> m_abilityMods[WIS]
				>> m_abilityMods[CHA];
		}
		else if (token == "ACDCMOD") {
			procLine >> m_AC >> m_spellDC >> m_spellModifier;
		}
		else if (token == "HP") {
			procLine >> m_maxHP;
			m_HP = m_maxHP;
		}
		else if (token == "SAVEPROFS") {
			while (line.find("ENDSAVEPROFS") != 0) {
				std::getline(defStream, line);
				if (!defStream) {
					errStatus = "Error: stream ended in save bonuses.";
					return false;
				}
				procLine.clear();
				procLine.str(line);
				procLine >> token;
				ABILITY_SCORES val;
				if (saveProfsFromString(token, val)) {
					int in;
					procLine >> in;
					m_savingThrowProfs[val] = in;
				}
			}
		}
		else if (token == "ABILITYPROFS") {
			while (line.find("ENDABILITYPROFS") != 0) {
				std::getline(defStream, line);
				if (!defStream) {
					errStatus = "Error: stream ended in ability proficiencies.";
					return false;
				}
				procLine.clear();
				procLine.str(line);
				procLine >> token;
				CHECK_TYPE val;
				if (checkProfsFromString(token, val)) {
					int in;
					procLine >> in;
					m_chkProfs[val] = in;
				}
			}
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
			while (line.find("ENDSPELLS") != 0) {
				if (!defStream) {
					errStatus = "Error: met premature end of definition stream. (Spell list)";
					return false;
				}
				SPELLS spellType = S_INVALID_SPELL;
				procLine.clear();
				procLine.str(line);
				procLine >> token;
				if (!splFrmStr(token, spellType) || spellType == S_INVALID_SPELL || spellType >= N_SPELLS)
				{
					errStatus = "Error: unrecognized spell token: " + token;
					return false;
				}
				m_spellList.push_back(spellType);

				std::getline(defStream, line);
			}
		}
		else if (token == "ATK") {
			int atkBonus = 0;
			std::function<int(void)> dmgRoll = d0;
			std::string dmgString;
			DMG_TYPE type = BLUDGEONING;
			WEAPON_PROPS props = 0;
			float minRange = 0, maxRange = 5, disRange = 5;
			std::vector<AttackEffect> attackEffects;
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
					procLine >> dmgString;
				}
				else if (token == "TYPE") {
					procLine >> token;
					if (!dmgTypeFromString(token, type)) {
						errStatus = "Invalid damage type: " + token;
						return false;
					}
				}
				else if (token == "RANGE") {
					procLine >> minRange >> maxRange >> disRange;
				}
				else if (token == "PROPS") {
					while (line.find("ENDPROPS") != 0) {
						std::getline(defStream, line);
						if (!defStream) {
							errStatus = "Error: stream ended in attack properties.";
							return false;
						}
						procLine.clear();
						procLine.str(line);
						procLine >> token;
						WEAPON_PROPS_BITS bit;
						if (propFromString(token, bit)) props |= bit;
					}
				}
				else if (token == "EFFECT") {
					procLine >> token;
					ATTACK_EFFECT_TYPE type;
					if (!atkEffectFromString(token, type)) {
						errStatus = "Unrecognized attack effect: " + token;
						return false;
					}
					int dc;
					procLine >> dc;
					if (dc < 0 || dc > 30)
					{
						errStatus = "Invalid attack save dc: " + std::to_string(dc);
						return false;
					}
					procLine >> token;
					ABILITY_SCORES saveType;
					if (!saveProfsFromString(token, saveType))
					{
						errStatus = "Invalid attack save ability score: " + token;
						return false;
					}
					bool isMagic = (procLine >> token && token == "MAGIC");

					attackEffects.push_back({ type, dc, saveType, isMagic });
				}
				else if (token == "ENDATK") {
					Attack* atk = new Attack(this);
					atk->load(type, atkBonus, dmgString, -1, props, minRange, maxRange, disRange, attackEffects);
					m_attacks.push_back(atk);
				}
			}
		}
		else if (token == "ENDFOE") {
			m_x = RING_SIZE - 1;
			m_y = RING_SIZE - 1;
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


void Foe::_checkDmg() {
	if (m_HP <= 0) {
		m_alive = false;
		m_dead = true;
		m_name[0] = 'x';
		m_name[1] = 'x';
	}
}


void Foe::takeDamage(int damage, DMG_TYPE type, Creature* agent) {
	Creature::takeDamage(damage, type, agent);
	_checkDmg();
}


void Foe::takeDamage(Attack* attack, Creature* agent) {
	Creature::takeDamage(attack, agent);
	_checkDmg();
}


bool Foe::hasAttackProp(WEAPON_PROPS_BITS prop, bool) {
	// only check first attack, because that will be the first used 
	// (until we implement intelligent reordering of attacks, if ever)
	return m_attacks[0]->getProps() & prop;
}


float Foe::getMaxAtkRange(bool) {
	float max = 0;
	float mx, mn, dis;
	for (auto& a : m_attacks) {
		a->getMinMaxDisRange(mn, mx, dis);
		if (mx > max) max = mx;
		if (dis > max) max = dis;
	}
	return max;
}


bool Foe::prepNextAttack(Attack* atk, Creature* target) {
	return Creature::prepNextAttack(atk, target);
}