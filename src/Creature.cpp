#include "Attack.h"
#include "Creature.h"
#include "Dice.h"
#include "Action.h"

void Creature::initRoll() {
	init = d20() + dex;
}

bool Creature::checkHit(Attack* attack) {
	if (m_dodge) attack->setDisadvantage(true);
	return attack->atk() >= ac;
}

void Creature::takeDamage(Attack* attack) {
	// reduce m_tempHP first
	int diff = m_tempHP - attack->dmg();
	if (diff > 0) {
		m_tempHP = diff;
	}
	else if (diff == 0) {
		m_tempHP = 0;
	}
	else {
		m_HP += diff;
	}
}

void Creature::takeTurn(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) {
	m_actions = m_reaction = m_bonus = m_spellCast = 1;
	m_dodge = false;
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
		if (ret->alive) return ret;
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
