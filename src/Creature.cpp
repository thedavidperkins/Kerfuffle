#include "Attack.h"
#include "Creature.h"
#include "Dice.h"
#include "Action.h"
#include "Logger.h"

Creature::Creature() :
	m_name(),
	m_abilityMods{0, 0, 0, 0, 0, 0},
	m_AC(0),
	m_spellDC(0),
	m_init(0),
	m_alive(true),
	m_dead(false),
	m_stable(false),
	m_archetype(BRAWLER),
	m_actions(1),
	m_reaction(1),
	m_spellCast(1),
	m_actionPriorities{SPELL, ATTACK, DASH, DISENGAGE, DODGE},
	m_deathSaves(0),
	m_deathFails(0),
	m_maxHP(0),
	m_HP(m_maxHP),
	m_tempHP(0),
	m_spellSlots{0, 0, 0, 0, 0, 0, 0, 0, 0},
	m_spellbook(),
	m_spellPriorities(),
	m_dodge(false),
	m_copy(nullptr),
	m_features(0),
	m_ftreTrkrs(),
	m_condition(0),
	m_hadAdvantage(false)
{}

Creature::Creature(const Creature* rhs) :
	m_name(rhs->m_name),
	m_abilityMods{
		rhs->m_abilityMods[STR], 
		rhs->m_abilityMods[DEX], 
		rhs->m_abilityMods[CON], 
		rhs->m_abilityMods[INT], 
		rhs->m_abilityMods[WIS], 
		rhs->m_abilityMods[CHA]
	},
	m_AC(rhs->m_AC),
	m_spellDC(rhs->m_spellDC),
	m_init(0),
	m_alive(true),
	m_dead(false),
	m_stable(false),
	m_archetype(rhs->m_archetype),
	m_actions(1),
	m_reaction(1),
	m_spellCast(1),
	m_actionPriorities{ 
		rhs->m_actionPriorities[0],
		rhs->m_actionPriorities[1],
		rhs->m_actionPriorities[2], 
		rhs->m_actionPriorities[3], 
		rhs->m_actionPriorities[4]
	},
	m_deathSaves(0),
	m_deathFails(0),
	m_maxHP(rhs->m_maxHP),
	m_HP(m_maxHP),
	m_tempHP(0),
	m_spellSlots{
		rhs->m_spellSlots[0],
		rhs->m_spellSlots[1],
		rhs->m_spellSlots[2],
		rhs->m_spellSlots[3],
		rhs->m_spellSlots[4],
		rhs->m_spellSlots[5],
		rhs->m_spellSlots[6],
		rhs->m_spellSlots[7],
		rhs->m_spellSlots[8],
	},
	m_spellbook(rhs->m_spellbook),
	m_spellPriorities(rhs->m_spellPriorities),
	m_dodge(false),
	m_copy(nullptr),
	m_features(rhs->m_features),
	m_ftreTrkrs(),
	m_condition(rhs->m_condition),
	m_hadAdvantage(false)
{
	_setupFtreTrkrs();
}

Creature::~Creature() {
	if (m_copy != nullptr) {
		delete m_copy;
		m_copy = nullptr;
	}
	for (auto& item : m_ftreTrkrs) {
		delete item.second;
	}
}

void Creature::initRoll() {
	m_init = d20() + m_abilityMods[DEX];
	LOG(m_name + " initiative roll: " + std::to_string(m_init));
}

bool Creature::checkHit(Attack* attack) {
	if (m_dodge) attack->setDisadvantage(true);
	int crit = 0;
	int roll = attack->atk();
	LOG("Target " + m_name + " has AC " + std::to_string(m_AC));
	return roll >= m_AC;
}

void Creature::takeDamage(Attack* attack) {
	// reduce m_tempHP first
	int roll = attack->dmg(this);
	int diff = m_tempHP - roll;
	if (diff > 0) {
		m_tempHP = diff;
	}
	else if (diff == 0) {
		m_tempHP = 0;
	}
	else {
		m_HP += diff;
	}
	LOG(m_name + " takes " + std::to_string(roll) + " " + dmgTypeToString(attack->dmgType()) + " damage.");
	LOG(m_name + " is left with " + std::to_string(m_HP) + " hit points.");
}

void Creature::takeTurn(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) {
	LOG(m_name + " taking turn: ");
	m_actions = m_reaction = 1;
	m_dodge = false;
	_resetFtreTrkrs();
	_setActionPriorities(friends, enemies);

	while (m_actions > 0) {
		for (auto& p : m_actionPriorities) {
			Action* act = Action::tryAction(p, this);
			if (act->isUsable(friends, enemies)) {
				act->invoke(friends, enemies);
				break;
			}
		}
	}
}

// Choose a random standing target
Creature* Creature::chooseAttackTarget(const std::vector<Creature*>& enemies) {
	size_t nEnemies = enemies.size();
	size_t index = randInt(nEnemies) - 1;
	Creature* ret;
	for (size_t iter = 0; iter < nEnemies; ++iter) {
		ret = enemies[(iter + index) % nEnemies];
		if (ret->m_alive) return ret;
	}
	return nullptr;
}

bool Creature::loadNextAttack(Attack* atk) {
	return true;
}

// later on this may be more context sensitive
void Creature::_setSpellPriorities(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	m_spellPriorities.clear();
	if (m_archetype == SUPPORT_CASTER) {
		m_spellPriorities.push_back(HEAL_PRIORITY);
		m_spellPriorities.push_back(SELF_HEAL_PRIORITY);
		m_spellPriorities.push_back(SUPP_PRIORITY);
		m_spellPriorities.push_back(SELF_SUPP_PRIORITY);
		m_spellPriorities.push_back(HARASS_PRIORITY);
		m_spellPriorities.push_back(DMG_PRIORITY);
	}
	m_spellPriorities.push_back(DMG_PRIORITY);
	m_spellPriorities.push_back(SELF_HEAL_PRIORITY);
	m_spellPriorities.push_back(HARASS_PRIORITY);
	m_spellPriorities.push_back(HEAL_PRIORITY);
	m_spellPriorities.push_back(SUPP_PRIORITY);
	m_spellPriorities.push_back(SELF_SUPP_PRIORITY);
}

// later on this may be more context sensitive
void Creature::_setActionPriorities(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	m_actionPriorities[0] = SPELL;
	m_actionPriorities[1] = ATTACK;
	m_actionPriorities[2] = DODGE;
	m_actionPriorities[3] = DISENGAGE;
	m_actionPriorities[4] = DASH;
}

Spell* Creature::chooseSpell(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	if (m_spellCast == 0) return nullptr;
	_setSpellPriorities(friends, enemies);
	std::vector<Spell*> spellChoices;

	int maxLvl = 0;
	for (int iter = 0; iter < SPELL_LVL_COUNT; ++iter) {
		if (m_spellSlots[iter] > 0) maxLvl = iter + 1;
	}

	// choose spells according to priority
	for (auto& pr : m_spellPriorities) {
		for (auto& spl : m_spellbook) {
			// exclude any spells for which no slots are available
			if (spl->lvl() <= maxLvl) {
				if(spl->priority() == pr) spellChoices.push_back(spl);
			}
		}
	}

	for (auto& spl : spellChoices) {
		if (spl->identifyTargets(friends, enemies)) {
			return spl;
		}
	}

	return nullptr;
}

int Creature::rolld20(ROLL_TYPE rollType) {
	m_hadAdvantage = false;
	int roll = d20();
	if (roll == 1) {
		if (m_features & F_LUCKY) {
			LOG(m_name + " rolled a 1 and rerolled with the Lucky feature.");
			roll = d20();
		}
	}
	return roll;
}

int Creature::rolld20Adv(ROLL_TYPE rollType) {
	m_hadAdvantage = true;
	int roll1 = d20(), roll2 = d20();
	int* oneRoll = nullptr;
	if (roll1 == 1) oneRoll = &roll1;
	else if (roll2 == 1) oneRoll = &roll2;
	if (oneRoll != nullptr) {
		if (m_features & F_LUCKY) {
			LOG(m_name + " rolled a 1 and rerolled with the Lucky feature.");
			*oneRoll = d20();
		}
	}
	return roll1 > roll2 ? roll1 : roll2;
}

int Creature::rolld20Dis(ROLL_TYPE rollType) {
	m_hadAdvantage = false;
	int roll1 = d20(), roll2 = d20();
	int* oneRoll = nullptr;
	if (roll1 == 1) oneRoll = &roll1;
	else if (roll2 == 1) oneRoll = &roll2;
	if (oneRoll != nullptr) {
		if (m_features & F_LUCKY) {
			LOG(m_name + " rolled a 1 and rerolled with the Lucky feature.");
			*oneRoll = d20();
		}
	}
	return roll1 < roll2 ? roll1 : roll2;
}

int Creature::savingThrow(ABILITY_SCORES sc, CONDITION threat) {
	int roll;
	if ((threat & C_FRIGHTENED) && (m_features & F_BRAVE)) {
		roll = rolld20Adv(R_SAVING_THROW);
	}
	else {
		roll = rolld20(R_SAVING_THROW);
	}
	return roll;
}

void Creature::_setupFtreTrkrs() {
	for (FEATURE_BIT i = 1; i <= m_features; i <<= 1) {
		if (m_features & i) {
			FeatureTrkr* trkr = FeatureTrkr::makeTracker(i, this);
			if (dynamic_cast<EmptyTrkr*>(trkr) != nullptr) {
				delete trkr;
			}
			else {
				m_ftreTrkrs.insert({ i, trkr });
			}
		}
	}
}

void Creature::_resetFtreTrkrs() {
	for (auto f : m_ftreTrkrs) {
		f.second->reset();
	}
}