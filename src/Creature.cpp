#include <algorithm>

#include "Action.h"
#include "Attack.h"
#include "Creature.h"
#include "Dice.h"
#include "FeatureAction.h"
#include "Logger.h"
#include "Ring.h"
#include "SpellAction.h"


constexpr float DEFAULT_SPEED = 25.f;
constexpr uint16_t MIN_THREAT = 1;
constexpr uint16_t MAX_THREAT = 9;
constexpr uint32_t DAMAGE_TO_THREAT_FACTOR = 5;

Creature::Creature()
	: m_name()
	, m_abilityMods{ 0, 0, 0, 0, 0, 0 }
	, m_savingThrowProfs{ 0, 0, 0, 0, 0, 0 }
	, m_chkProfs{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	, m_AC(0)
	, m_spellDC(0)
	, m_spellModifier(0)
	, m_level(1)
	, m_init(0)
	, m_alive(true)
	, m_dead(false)
	, m_stable(false)
	, m_archetype(BRAWLER)
	, m_actions(1)
	, m_reaction(1)
	, m_spellCast(1)
	, m_actionsAvailable()
	, m_deathSaves(0)
	, m_deathFails(0)
	, m_maxHP(0)
	, m_HP(m_maxHP)
	, m_tempHP(0)
	, m_spellSlots{ 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	, m_spellList()
	, m_spellbook()
	, m_dodge(false)
	, m_copy(nullptr)
	, m_features(0)
	, m_ftreTrkrs()
	, m_condition(0)
	, m_hadAdvantage(false)
	, m_hadDisadvantage(false)
	, m_friends(nullptr)
	, m_atkdThisTurn(false)
	, m_tookDmgThisTurn(false)
	, m_x(0)
	, m_y(0)
	, m_cell(nullptr)
	, m_speed(DEFAULT_SPEED)
	, m_movementRemaining((float)m_speed)
	, m_threatLevel(MIN_THREAT)
{
	_loadDefaultActions();
}


Creature::Creature(const Creature* rhs)
	: m_name(rhs->m_name)
	, m_abilityMods{
		rhs->m_abilityMods[STR], 
		rhs->m_abilityMods[DEX], 
		rhs->m_abilityMods[CON], 
		rhs->m_abilityMods[INT], 
		rhs->m_abilityMods[WIS], 
		rhs->m_abilityMods[CHA]
	}
	, m_savingThrowProfs{
		rhs->m_savingThrowProfs[STR],
		rhs->m_savingThrowProfs[DEX],
		rhs->m_savingThrowProfs[CON],
		rhs->m_savingThrowProfs[INT],
		rhs->m_savingThrowProfs[WIS],
		rhs->m_savingThrowProfs[CHA]
	}
	, m_chkProfs{
		rhs->m_chkProfs[ACRO_CHK],
		rhs->m_chkProfs[ANIM_CHK],
		rhs->m_chkProfs[ARCA_CHK],
		rhs->m_chkProfs[ATHL_CHK],
		rhs->m_chkProfs[DECE_CHK],
		rhs->m_chkProfs[HIST_CHK],
		rhs->m_chkProfs[INSI_CHK],
		rhs->m_chkProfs[INTI_CHK],
		rhs->m_chkProfs[INVE_CHK],
		rhs->m_chkProfs[MEDI_CHK],
		rhs->m_chkProfs[NATU_CHK],
		rhs->m_chkProfs[PERC_CHK],
		rhs->m_chkProfs[PERF_CHK],
		rhs->m_chkProfs[PERS_CHK],
		rhs->m_chkProfs[RELI_CHK],
		rhs->m_chkProfs[SLEI_CHK],
		rhs->m_chkProfs[STEA_CHK],
		rhs->m_chkProfs[SURV_CHK]
	}
	, m_AC(rhs->m_AC)
	, m_spellDC(rhs->m_spellDC)
	, m_spellModifier(rhs->m_spellModifier)
	, m_level(rhs->m_level)
	, m_init(0)
	, m_alive(true)
	, m_dead(false)
	, m_stable(false)
	, m_archetype(rhs->m_archetype)
	, m_actions(1)
	, m_reaction(1)
	, m_spellCast(1)
	, m_actionsAvailable()
	, m_deathSaves(0)
	, m_deathFails(0)
	, m_maxHP(rhs->m_maxHP)
	, m_HP(m_maxHP)
	, m_tempHP(0)
	, m_spellSlots{
		rhs->m_spellSlots[0],
		rhs->m_spellSlots[1],
		rhs->m_spellSlots[2],
		rhs->m_spellSlots[3],
		rhs->m_spellSlots[4],
		rhs->m_spellSlots[5],
		rhs->m_spellSlots[6],
		rhs->m_spellSlots[7],
		rhs->m_spellSlots[8],
	}
	, m_spellList(rhs->m_spellList)
	, m_spellbook(rhs->m_spellbook)
	, m_dodge(false)
	, m_copy(nullptr)
	, m_features(rhs->m_features)
	, m_ftreTrkrs()
	, m_condition(rhs->m_condition)
	, m_hadAdvantage(false)
	, m_hadDisadvantage(false)
	, m_friends(nullptr)
	, m_atkdThisTurn(false)
	, m_tookDmgThisTurn(false)
	, m_x(rhs->m_x)
	, m_y(rhs->m_y)
	, m_cell(nullptr)
	, m_speed(rhs->m_speed)
	, m_movementRemaining((float)m_speed)
	, m_threatLevel(rhs->m_threatLevel)
{
	_loadDefaultActions();
}


void Creature::_loadDefaultActions() {
	m_actionsAvailable.push_back(Action::newAction(ATTACK_ACTION, this));
	m_actionsAvailable.push_back(Action::newAction(DASH_ACTION, this));
	m_actionsAvailable.push_back(Action::newAction(DISENGAGE_ACTION, this));
	m_actionsAvailable.push_back(Action::newAction(DODGE_ACTION, this));
}


void Creature::init() {
	_setupFtreTrkrs();
	_idCell();
}


Creature::~Creature() {
	for (Action* action : m_actionsAvailable)
	{
		delete action;
	}

	if (m_copy != nullptr) {
		delete m_copy;
		m_copy = nullptr;
	}
	for (auto& item : m_ftreTrkrs) {
		delete item.second;
	}
	for (auto& spell : m_spellbook) {
		delete spell.second;
	}
	if (m_cell != nullptr) {
		m_cell->removeOccupant(this);
	}
}


void Creature::initRoll() {
	m_init = abilityCheck(DEX);
	LOG(m_name + " initiative roll: " + std::to_string(m_init));
}


SPELL_LEVELS Creature::getSlotsAvailableMask() const {
	uint16_t iter = 0;
	SPELL_LEVELS mask = 0;
	for (uint16_t bit = L1; bit <= L9; bit <<= 1) {
		if (m_spellSlots[iter++] > 0) {
			mask |= bit;
		}
	}

	return mask;
}


SPELL_LEVEL_BITS Creature::getHighestAvailableSpellLevel() const {
	SPELL_LEVELS mask = getSlotsAvailableMask();
	for (uint16_t bit = L9; bit >= L1; bit >>= 1) {
		if (mask & bit) {
			return (SPELL_LEVEL_BITS)bit;
		}
	}

	return CANTRIP;
}


bool Creature::checkHit(Attack* attack) {
	if (m_dodge) {
		attack->setDisadvantage(true);
	}
	if (m_condition & C_PRONE) {
		if (attack->isMelee()) {
			attack->setAdvantage(true);
		}
		else {
			attack->setDisadvantage(true);
		}
	}
	int roll = attack->atk();
	LOG("Target " + m_name + " has AC " + std::to_string(m_AC));
	return roll >= m_AC;
}


void Creature::takeDamage(int damage, DMG_TYPE type, Creature* agent) {
	// reduce m_tempHP first
	int diff = m_tempHP - damage;
	if (diff > 0) {
		m_tempHP = diff;
	}
	else if (diff == 0) {
		m_tempHP = 0;
	}
	else {
		m_HP += diff;
	}
	LOG(m_name + " takes " + std::to_string(damage) + " " + dmgTypeToString(type) + " damage.");
	LOG(m_name + " is left with " + std::to_string(m_HP) + " hit points.");

	agent->adjustThreat(damage);
}


void Creature::takeDamage(Attack* attack, Creature* agent) {
	takeDamage(attack->dmg(this), attack->dmgType(), agent);
}


void Creature::takeTurn(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) {
	LOG(m_name + " taking turn: ");
	m_actions = m_reaction = 1;
	m_movementRemaining = (float)m_speed;
	m_dodge = false;
	m_tookDmgThisTurn = false;
	m_atkdThisTurn = false;

	for (Action* action : m_actionsAvailable)
	{
		action->resetActionState();
	}

	_resetFtreTrkrs();
	_resetSpellTrkrs();
	_setActionPriorities(friends, enemies);

	std::vector<Action*> actionsToSkip = {};

	while (m_actions > 0) {
		for (Action* action : m_actionsAvailable) {
			if (std::find(actionsToSkip.begin(), actionsToSkip.end(), action) != actionsToSkip.end())
			{
				continue;
			}

			if ( action->isUsable(friends, enemies)) {
				if (!action->invoke(friends, enemies))
				{
					actionsToSkip.push_back(action);
				}
				break;
			}
		}
	}

	_finishMove(friends, enemies);
}


void Creature::bumpName() {
	while (m_name.length() < 8) {
		m_name.append(" ");
	}

	if (m_name[7] == ' ') {
		m_name[7] = '1';
	}
	else {
		m_name[7] = m_name[7] + 1;
	}
}


// Choose a random adjacent target or nearest target otherwise
Creature* Creature::chooseAttackTarget(const std::vector<Creature*>& enemies) {
	// get adjacent enemy list
	std::vector<Creature*> adj = getAdjCreatures(enemies);
	if (!adj.empty()) {
		size_t nAdj = adj.size();
		size_t index = randInt(nAdj) - 1;
		Creature* ret;
		for (size_t iter = 0; iter < nAdj; ++iter) {
			ret = adj[(iter + index) % nAdj];
			if (ret->isStanding()) return ret;
		}
	}
	
	// If we're here, there are no adjacent enemies. Choose the closest.
	// TODO: Implement targeting based on threat rather than random or range
	Ring& r = Ring::getInstance();
	for (auto& c : r.sortCellsByDistance(m_cell)) {
		for (auto& cr : c->getOccupants()) {
			if (cr->isStanding() && (std::find(enemies.begin(), enemies.end(), cr) != enemies.end())) {
				return cr;
			}
		}
	}

	return nullptr;
}


// later on we will use context to set weights but for now we just set them inline with the priority sorting
void Creature::_setActionPriorities(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies) {
	for (Action* a : m_actionsAvailable)
	{
		a->setPriorityWeight(m_archetype, friends, enemies);
	}

	std::sort(m_actionsAvailable.begin(), m_actionsAvailable.end(), [](Action *a, Action* b) {
		return a->getWeight() > b->getWeight();
	});
}


int Creature::rolld20(ROLL_TYPE rollType) {
	LOG(m_name + " rolls a d20.");
	m_hadAdvantage = false;
	m_hadDisadvantage = false;
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
	LOG(m_name + " rolls a d20 with advantage.");
	m_hadAdvantage = true;
	m_hadDisadvantage = false;
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
	LOG(m_name + " rolls a d20 with disadvantage.");
	m_hadAdvantage = false;
	m_hadDisadvantage = true;
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


int Creature::savingThrow(ABILITY_SCORES sc, CONDITION threat, bool isMagic) {
	bool hasAdvantage = false;
	bool hasDisadvantage = false;

	if (
		((threat & C_FRIGHTENED)    &&     (m_features & F_BRAVE))                ||
		((threat & C_CHARMED)       &&     (m_features & F_FEY_ANCESTRY))         ||
		((threat & C_POISONED)      &&     (m_features & F_DWARVEN_RESILIENCE))
		) {
		hasAdvantage = true;
	}
	else if ((m_features & F_BARBARIAN_RAGE) && (sc == STR)) {
		if (getTrkr<BarbarianRageTrkr>()->isActive()) {
			hasAdvantage = true;
		}
	}
	else if (isMagic && (m_features & F_GNOME_CUNNING) & (sc == INT || sc == WIS || sc == CHA)) {
		hasAdvantage = true;
	}
	
	int roll;

	if (hasAdvantage && !hasDisadvantage) {
		roll = rolld20Adv(R_ABILITY_CHECK);
	}
	else if (hasDisadvantage && !hasAdvantage) {
		roll = rolld20Dis(R_ABILITY_CHECK);
	}
	else {
		roll = rolld20(R_ABILITY_CHECK);
	}

	uint32_t fullRoll = roll + m_abilityMods[sc] + m_savingThrowProfs[sc];
	LOG(m_name + " rolled a saving throw of " + std::to_string(fullRoll));
	return fullRoll;
}


int Creature::abilityCheck(const std::vector<CHECK_TYPE>& abilities) {
	int maxBonus = -std::numeric_limits<int>::max();
	ABILITY_SCORES ability = N_ABILITY_SCORES;
	for (auto& a : abilities) {
		int bonus = m_abilityMods[checkAbility(a)] + m_chkProfs[a];
		if (maxBonus < bonus)
		{
			maxBonus = bonus;
			ability = checkAbility(a);
		}
	}

	return abilityCheck(ability);
}


int Creature::abilityCheck(ABILITY_SCORES sc) {
	bool hasAdvantage = false;
	bool hasDisadvantage = false;

	if (m_condition & C_POISONED) {
		hasDisadvantage = true;
	}
	else if ((m_features & F_BARBARIAN_RAGE) && (sc == STR)) {
		if (getTrkr<BarbarianRageTrkr>()->isActive()){
			hasAdvantage = true;
		}
	}

	if( hasAdvantage && !hasDisadvantage) {
		return rolld20Adv(R_ABILITY_CHECK) + m_abilityMods[sc];
	}
	else if (hasDisadvantage && !hasAdvantage) {
		return rolld20Dis(R_ABILITY_CHECK) + m_abilityMods[sc];
	}
	else {
		return rolld20(R_ABILITY_CHECK) + m_abilityMods[sc];
	}
}


void Creature::_setupFtreTrkrs() {
	for (FEATURE_BIT i = 1; i <= m_features; i <<= 1) {
		if (m_features & i) {
			FeatureTrkr* trkr = FeatureTrkr::makeTracker(i, this);
			if (trkr->isEmpty()) {
				delete trkr;
			}
			else {
				m_ftreTrkrs.insert({ i, trkr });
				if (getFeatureActionTiming(i) == ACTION_TIMING_ACTION) {
					ActionFeatureTrkr* actionTracker = dynamic_cast<ActionFeatureTrkr*>(trkr);
					if (actionTracker == nullptr) {
						throw std::runtime_error("Action tracker not found for action feature");
					}

					m_actionsAvailable.push_back(new FeatureAction(i, this, actionTracker));
				}
			}
		}
	}
}


void Creature::_resetFtreTrkrs() {
	for (auto f : m_ftreTrkrs) {
		f.second->reset();
	}
}


void Creature::_setupSpellTrkrs() {
	for (SPELLS s : m_spellList) {
		m_spellbook[s] = Spell::makeSpell(s, this);
		if (actionTimingFromSpl(s) & ACTION_TIMING_ACTION) {
			m_actionsAvailable.push_back(new SpellAction(s, this, m_spellbook[s]));
		}
	}
}


void Creature::_resetSpellTrkrs() {
	for (auto& s : m_spellbook) {
		s.second->reset();
	}
}


void Creature::_idCell() {
	Ring& r = Ring::getInstance();
	m_cell = r.assignCell(m_x, m_y, this);
	if (m_cell == nullptr) {
		throw std::runtime_error("Couldn't identify a cell in the ring.");
	}
}


std::vector<Creature*> Creature::getAdjCreatures() {
	Ring& r = Ring::getInstance();
	std::vector<Creature*> ret;
	if (m_cell->nOccupants() > 1) {
		for (auto cr : m_cell->getOccupants()) {
			if (cr != this) {
				ret.push_back(cr);
			}
		}
	}
	for (auto c : r.getNeighboringCells(m_cell)) {
		for (auto cr : c->getOccupants()) {
			ret.push_back(cr);
		}
	}
	return ret;
}


std::vector<Creature*> Creature::getAdjCreatures(const std::vector<Creature*>& candidates) {
	std::vector<Creature*> ret = getAdjCreatures();
	ret.erase(std::remove_if(ret.begin(), ret.end(), [&](const Creature* c) {
		return (std::find(candidates.begin(), candidates.end(), c) == candidates.end());
	}), ret.end());
	return ret;
}


std::vector<Creature*> Creature::getCreaturesInRange(float range) const {
	Ring& r = Ring::getInstance();
	std::vector<Creature*> ret;
	for (auto c : r.cellsInRange(m_cell, range)) {
		for (auto cr : c->getOccupants()) {
			ret.push_back(cr);
		}
	}
	return ret;
}


std::vector<Creature*> Creature::getCreaturesInRange(const std::vector<Creature*>& candidates, float range) const {
	std::vector<Creature*> ret = getCreaturesInRange(range);
	ret.erase(std::remove_if(ret.begin(), ret.end(), [&](const Creature* c) {
		return (std::find(candidates.begin(), candidates.end(), c) == candidates.end());
	}), ret.end());
	return ret;
}


float Creature::getRemainingRange() {
	return m_movementRemaining;
}


bool Creature::prepNextAttack(Attack* atk, Creature* target) {
	atk->restore(); // specifically for foes, whose attacks may be changed but changes shouldn't persist
	
	if (!getAdjCreatures({ target }).empty()) { // if target is adjacent, do nothing special
		return true;
	}
	
	if (atk->getProps() & (RANGED | REACH) ) {
		return _checkRangedAttack(atk, target);
	}

	// If we are here, we either have a melee or thrown attack.
	// Never throw if you can melee, so try to move to melee range.
	if (moveToAdjacent(target)) return true;

	// Finally, see if a thrown attack will work
	if (atk->getProps() & THROWN) {
		if (_checkRangedAttack(atk, target)) {
			atk->subtractProp(MELEE);
			return true;
		}
	}

	return false;
}


bool Creature::_checkRangedAttack(Attack* atk, Creature* target) {
	float distance = dist(m_cell, target->getCell());
	float min, max, dis;
	atk->getMinMaxDisRange(min, max, dis);
	if (distance < min) {
		atk->setDisadvantage(true);
		return true;
	}
	if (distance > max) {
		if (!moveToRangeOf(target, max)) {
			atk->setDisadvantage(true);
			return dist(m_cell, target->getCell()) <= dis;
		}
	}
	return true;
}


bool Creature::moveToRangeOf(Creature* target, float range) {
	if (dist(target->getCell(), m_cell) <= range) return true;
	Ring& r = Ring::getInstance();
	Cell* dest = nullptr;
	for (auto& c : r.sortCellsByDistance(m_cell)) {
		if (dist(c, target->getCell()) < range) {
			if (c->nOccupants() == 0) {
				dest = c;
				break;
			}
		}
	}
	if (dest != nullptr) { // found a suitable destination -- this should only fail if all cells in the range are occupied (very unlikely)
		if (!moveToCell(dest)) return false;
	}
	return dist(target->getCell(), m_cell) <= range;
}


bool Creature::moveToAdjacent(Creature* target) {
	Ring& r = Ring::getInstance();
	Cell* dest = nullptr;
	auto adj = r.getNeighboringCells(target->getCell());
	for (auto& c : r.sortCellsByDistance(m_cell)) {
		if (std::find(adj.begin(), adj.end(), c) != adj.end()) {
			if (c->nOccupants() == 0) {
				dest = c;
				break;
			}
		}
	}
	if (dest == nullptr) return false; // all spaces were occupied around the target
	if (!moveToCell(dest)) return false;
	return (std::find(adj.begin(), adj.end(), m_cell) != adj.end());
}


bool Creature::moveToCell(Cell* dest) {
	if (m_condition & C_PRONE) {
		if (m_movementRemaining < (m_speed / 2)) {
			return false;
		}
		else {
			LOG(m_name + " must use half their movement to stand");
			m_movementRemaining -= (m_speed / 2);
			removeCondition(C_PRONE);
		}
	}
	if (m_movementRemaining < 5) {
		return false;
	}
	LOG(m_name + " attempts to move from cell (" + m_cell->coordString() + ") to cell (" + dest->coordString() + ")");
	Ring& r = Ring::getInstance();
	bool ret = true;
	float distanceToTravel = dist(dest, m_cell);
	LOG(m_name + " has distance " + std::to_string(distanceToTravel) + " to travel and " + std::to_string(m_movementRemaining) + " movement remaining.");
	if (distanceToTravel > m_movementRemaining) { // can't get all the way there
		ret = false;
		for (auto& c : r.sortCellsByDistance(dest)) {
			if (dist(c, m_cell) <= m_movementRemaining) {
				dest = c;
				if (dest->nOccupants() == 0) {
					distanceToTravel = dist(dest, m_cell);
					break;
				}
			}
		}
	}
	m_movementRemaining -= distanceToTravel;
	m_cell->removeOccupant(this);
	dest->addOccupant(this);
	m_cell = dest;
	LOG(m_name + " arrived at " + m_cell->coordString() + " with " + std::to_string(m_movementRemaining) + " movement remaining.");
	return ret;
}


void Creature::dash() {
	m_movementRemaining += m_speed;
}


void Creature::cancelDash() {
	m_movementRemaining -= m_speed;
}


void Creature::_finishMove(const std::vector<Creature*>& friends, const std::vector<Creature*> enemies) {
	if ((m_condition & C_PRONE) && (m_movementRemaining > (m_speed / 2))) {
		m_movementRemaining -= m_speed / 2;
		removeCondition(C_PRONE);
		LOG(m_name + " stands and is no longer prone.");
	}
	if (m_movementRemaining >= 5) {
		float distance;
		Creature* en = findNearest(enemies, distance);
		if (en != nullptr) {
			moveToRangeOf(en, m_speed / 2);
		}
	}
}


Creature* Creature::findNearest(const std::vector<Creature*> crtrs, float& distance) {
	distance = std::numeric_limits<float>::max();
	Creature* ret = nullptr;
	for (auto& c : crtrs) {
		if (c->isStanding()) {
			float thisDistance = dist(m_cell, c->getCell());
			if (thisDistance < distance) {
				distance = thisDistance;
				ret = c;
			}
		}
	}
	return ret;
}


void Creature::adjustThreat(uint32_t damageDealt) {
	uint16_t tentativeThreat = damageDealt / DAMAGE_TO_THREAT_FACTOR;
	if (tentativeThreat > m_threatLevel) {
		m_threatLevel = tentativeThreat;
	}

	if (m_threatLevel > MAX_THREAT) {
		m_threatLevel = MAX_THREAT;
	}
}


static std::map<std::string, ABILITY_SCORES> lAblStrings {
	{ "STR", STR },
	{ "DEX", DEX },
	{ "CON", CON },
	{ "INT", INT },
	{ "WIS", WIS },
	{ "CHA", CHA },
};


static std::map<std::string, CHECK_TYPE> lCheckStrings {
	{ "ACROBATICS", ACRO_CHK },
	{ "ANIMAL_HANDLING", ANIM_CHK },
	{ "ARCANA", ARCA_CHK },
	{ "ATHLETICS", ATHL_CHK },
	{ "DECEPTION", DECE_CHK },
	{ "HISTORY", HIST_CHK },
	{ "INSIGHT", INSI_CHK },
	{ "INTIMIDATION", INTI_CHK },
	{ "INVESTIGATION", INVE_CHK },
	{ "MEDICINE", MEDI_CHK },
	{ "NATURE", NATU_CHK },
	{ "PERCEPTION", PERC_CHK },
	{ "PERFORMANCE", PERF_CHK },
	{ "PERSUASION", PERS_CHK },
	{ "RELIGION", RELI_CHK },
	{ "SLEIGHT_OF_HAND", SLEI_CHK },
	{ "STEALTH", STEA_CHK },
	{ "SURVIVAL", SURV_CHK }
};


static std::map<CHECK_TYPE, ABILITY_SCORES> lCheckAbilities{
	{ ACRO_CHK, DEX },
	{ ANIM_CHK, WIS },
	{ ARCA_CHK, INT },
	{ ATHL_CHK, STR },
	{ DECE_CHK, CHA },
	{ HIST_CHK, INT },
	{ INSI_CHK, WIS },
	{ INTI_CHK, CHA },
	{ INVE_CHK, INT },
	{ MEDI_CHK, WIS },
	{ NATU_CHK, INT },
	{ PERC_CHK, WIS },
	{ PERF_CHK, CHA },
	{ PERS_CHK, CHA },
	{ RELI_CHK, INT },
	{ SLEI_CHK, DEX },
	{ STEA_CHK, DEX },
	{ SURV_CHK, WIS }
};


bool checkProfsFromString(const std::string& token, CHECK_TYPE& val) {
	auto i = lCheckStrings.find(token);
	if (i != lCheckStrings.end()) {
		val = i->second;
		return true;
	}
	return false;
}


bool saveProfsFromString(const std::string& token, ABILITY_SCORES& val) {
	auto i = lAblStrings.find(token);
	if (i != lAblStrings.end()) {
		val = i->second;
		return true;
	}
	return false;
}


std::string abilToString(ABILITY_SCORES score)
{
	for (auto& item : lAblStrings) {
		if (item.second == score) {
			return item.first;
		}
	}
	return "UNKNOWN_ABILITY";
}


ABILITY_SCORES checkAbility(CHECK_TYPE chk) {
	return lCheckAbilities.at(chk);
}


FeatureTrkr* Creature::getTrkr(FEATURE_BIT ftre) {
	FeatureTrkr* ret = m_ftreTrkrs.at(ftre);
	if (ret == nullptr) {
		throw std::runtime_error("Error: no feature found with bit code " + std::to_string(ftre));
	}
	return ret;
}


std::vector<Creature*> sortCreaturesBy(const std::vector<Creature*> list, std::function<bool(Creature*, Creature*)> pred) {
	std::vector<Creature*> out = list;
	std::sort(out.begin(), out.end(), pred);
	return out;
}


std::vector<Creature*> sortCreaturesByLeastHealth(const std::vector<Creature*> list) {
	return sortCreaturesBy(list, [](Creature* a, Creature* b) {
		int ahp = a->getHP();
		int bhp = b->getHP();
		if (ahp == bhp) return a->getHealthLost() > b->getHealthLost();
		return ahp < bhp;
	});
}


std::vector<Creature*> sortCreaturesByMostThreat(const std::vector<Creature*> list) {
	return sortCreaturesBy(list, [](Creature* a, Creature* b) {
		int athr = a->getThreatLevel();
		int bthr = b->getThreatLevel();
		if (athr == bthr) return a->getHealthLost() > b->getHealthLost();
		return athr > bthr;
	});
}


void Creature::healBy(int healing) {
	m_HP += healing;
	if (m_HP > m_maxHP) m_HP = m_maxHP;
	if (!m_alive && m_HP > 0) {
		m_alive = true;
	}
}


bool Creature::hasDmgResistance(DMG_TYPE dmgType) {
	bool isRaging = false;
	if (m_features & F_BARBARIAN_RAGE) {
		isRaging = getTrkr<BarbarianRageTrkr>()->isActive();
	}

	switch (dmgType)
	{
	case BLUDGEONING:
	case PIERCING:
	case SLASHING:
		return isRaging;
		break;
	case POISON:
		return ( m_features & F_DWARVEN_RESILIENCE );
		break;
	default:
		return false;
	}
}