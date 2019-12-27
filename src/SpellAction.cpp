#include "SpellAction.h"
#include "Creature.h"
#include "Spell.h"
#include "Logger.h"

SpellAction::SpellAction(SPELLS spell, Creature* user, Spell* spelltracker)
	: Action(SPELL_ACTION, user)
	, m_spellType(spell)
	, m_spell(spelltracker) 
{}

bool SpellAction::isUsable(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	return m_spell->isUsable(friends, enemies);
}

bool SpellAction::invoke(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) {
	LOG(m_user->getName() + " casts spell " + m_spell->getName() + ".");
	m_user->usedSpell();
	return m_spell->cast();
}

void SpellAction::setPriorityWeight(ARCHETYPE arch, const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	switch (arch)
	{
	case BRAWLER:
	case ROGUE:
	case RANGER:
		m_weight = 50;
		break;
	case DAMAGE_CASTER:
	case SUPPORT_CASTER:
		m_weight = 90;
		break;
	default:
		throw std::runtime_error("Error: unrecognized archetype for action priority.");
	}

	m_spell->adjustWeight(m_weight, friends, enemies);
}