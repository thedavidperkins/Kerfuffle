#ifndef KERF_CREATURE_H
#define KERF_CREATURE_H

#include <vector>
#include <functional>
#include <sstream>
#include <map>

#include "enums_common.h"
#include "Features.h"
#include "Spell.h"
#include "Weapons.h"

class Cell;
class Action;
class Attack;

enum CHECK_TYPE {
	ACRO_CHK = 0,
	ANIM_CHK,
	ARCA_CHK,
	ATHL_CHK,
	DECE_CHK,
	HIST_CHK,
	INSI_CHK,
	INTI_CHK,
	INVE_CHK,
	MEDI_CHK,
	NATU_CHK,
	PERC_CHK,
	PERF_CHK,
	PERS_CHK,
	RELI_CHK,
	SLEI_CHK,
	STEA_CHK,
	SURV_CHK,
	N_CHECK_TYPE
};

enum CONDITION_BITS {
	C_FRIGHTENED = 0x1,
	C_CHARMED = 0x2,
	C_POISONED = 0x4,
	C_PRONE = 0x8,
};
typedef unsigned int CONDITION;

class Creature {
public:
	virtual ~Creature();
	void init();
	virtual void takeTurn(std::vector<Creature*>& friends, std::vector<Creature*>& enemies) = 0;
	virtual void takeDamage(int damage, DMG_TYPE type, Creature* agent);
	virtual void takeDamage(Attack* attack, Creature* agent);
	virtual bool deathCheck() = 0;
	virtual bool prepNextAttack(Attack* atk, Creature* target) = 0;
	virtual void getAttackList(std::vector<Attack*>& atks) = 0;
	virtual void cleanupAttackList(std::vector<Attack*>& atks) = 0;
	virtual bool hasAttackProp(WEAPON_PROPS_BITS prop, bool dual = false) = 0;
	virtual void incentivizeProp(WEAPON_PROPS_BITS prop) {}
	virtual float getMaxAtkRange(bool dual = false) = 0;
	int getLvl() const { return m_level; }

	bool checkHit(Attack* attack);
	
	void initRoll();

	void usedSpell() { m_spellCast--; }
	void usedAction() { m_actions--; }

	SPELL_LEVELS getSlotsAvailableMask() const;
	SPELL_LEVEL_BITS getHighestAvailableSpellLevel() const;

	bool isStanding() const { return m_alive; }
	bool isDead() const { return m_dead; }
	bool isStable() const { return m_stable; }
	int getInit() const { return m_init; }
	int getAMod(ABILITY_SCORES sc) const { return m_abilityMods[sc]; }
	int getSpellMod() const { return m_spellModifier; }
	int getSpellDC() const { return m_spellDC; }
	bool hadAdvantage() const { return m_hadAdvantage; }
	bool hadDisadvantage() const { return m_hadDisadvantage; }
	float getSpeed() const { return m_speed; }
	float getDistanceTraveled() const { return m_speed - m_movementRemaining; }
	int getHP() const { return m_HP; }
	int getHPTP() const { return m_HP + m_tempHP; }
	int getMaxHP() const { return m_maxHP; }
	int getHealthLost() const { return m_maxHP - m_HP; }
	bool isBloodied() const { return getHealthLost() > getHP(); }
	bool isNearDeath() const { return getHealthLost() >= 3 * getHP(); }
	CONDITION getCondition() const { return m_condition; }
	void addCondition(CONDITION_BITS condition) { m_condition |= condition; }
	void removeCondition(CONDITION_BITS condition) { m_condition &= ~condition; }
	void healBy(int healing);

	std::string getName() const { return m_name; }
	void bumpName();

	Creature* chooseAttackTarget(const std::vector<Creature*>& enemies);

	void setDodge(bool d) { m_dodge = d; }

	virtual Creature* makeCopy() = 0;

	int rolld20(ROLL_TYPE rollType); // Allow player features to intervene on rolls
	int rolld20Adv(ROLL_TYPE rollType); // Allow player features to intervene on rolls
	int rolld20Dis(ROLL_TYPE rollType); // Allow player features to intervene on rolls
	int savingThrow(ABILITY_SCORES sc, CONDITION threat = 0, bool isMagic = false);
	int abilityCheck(const std::vector<CHECK_TYPE>& abilities);
	int abilityCheck(ABILITY_SCORES sc);

	FEATURES getFeatures() const { return m_features; }

	FeatureTrkr* getTrkr(FEATURE_BIT ftre);

	template <class T>
	T* getTrkr() {
		T* ret = dynamic_cast<T*>(m_ftreTrkrs.at(classBit<T>()));
		if (ret == nullptr) {
			throw std::runtime_error("Error in feature tracker cast.");
		}
		return ret;
	}

	std::vector<Creature*> getAdjCreatures();
	std::vector<Creature*> getAdjCreatures(const std::vector<Creature*>& candidates); // restrict list to intersection with candidates
	std::vector<Creature*> getCreaturesInRange(float range) const;
	std::vector<Creature*> getCreaturesInRange(const std::vector<Creature*>& candidates, float range) const; // restrict list to intersection with candidates

	float getRemainingRange();
	Cell* getCell() { return m_cell; }
	bool moveToRangeOf(Creature* target, float range);
	bool moveToAdjacent(Creature* target);
	bool moveToCell(Cell* dest);

	void dash();
	void cancelDash();

	Creature* findNearest(const std::vector<Creature*> crtrs, float& distance);

	void setFriends(std::vector<Creature*>* friends) { m_friends = friends; }
	std::vector<Creature*>* getFriends() { return m_friends; }

	bool hasDmgResistance(DMG_TYPE dmgType);

	bool atkdThisTurn() { return m_atkdThisTurn; }
	void setAtkdThisTurn() { m_atkdThisTurn = true; }
	bool tookDmgThisTurn() { return m_tookDmgThisTurn; }
	void setTookDmgThisTurn() { m_tookDmgThisTurn = true; }

	void adjustThreat(uint32_t damageDealt);
	uint16_t getThreatLevel() { return m_threatLevel; }

protected:
	Creature();
	Creature(const Creature* rhs);

	void _loadDefaultActions();
	virtual bool _defineFromStream(std::stringstream& defStream, std::string& errStatus) = 0;
	void _setupFtreTrkrs();
	void _resetFtreTrkrs();
	void _setupSpellTrkrs();
	void _resetSpellTrkrs();
	void _setActionPriorities(const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies);
	void _idCell();
	bool _checkRangedAttack(Attack* atk, Creature* target);

	void _finishMove(const std::vector<Creature*>& friends, const std::vector<Creature*> enemies);

	std::string m_name;

	// ability modifiers
	int m_abilityMods[N_ABILITY_SCORES];
	int m_savingThrowProfs[N_ABILITY_SCORES];
	int m_chkProfs[N_CHECK_TYPE];

	// difficulty classes
	int m_AC;
	int m_spellDC;
	int m_spellModifier;
	int m_level;

	// initiative -- held during turn order sort in Encounter::fight()
	int m_init;

	// standing, dead, or on the ground stabilized
	bool m_alive;
	bool m_dead;
	bool m_stable;

	// archetype -- useful for choosing action priority (later on, for now, use universal preset)
	ARCHETYPE m_archetype;

	// action flags
	int m_actions;
	int m_reaction;
	int m_spellCast;

	std::vector<Action*> m_actionsAvailable;

	// death save counters
	int m_deathSaves;
	int m_deathFails;

	// health
	int m_maxHP;
	int m_HP;
	int m_tempHP;

	// spells
	int m_spellSlots[SPELL_LVL_COUNT];
	std::vector<SPELLS> m_spellList;
	std::map<SPELLS, Spell*> m_spellbook;

	// dodge
	bool m_dodge;

	// used by prototype copies to manage used copies
	Creature* m_copy;

	// Features
	FEATURES m_features;
	std::map<FEATURE_BIT, FeatureTrkr*> m_ftreTrkrs;

	// Condition
	CONDITION m_condition;

	// Miscellaneous state tracking
	bool m_hadAdvantage;
	bool m_hadDisadvantage;
	std::vector<Creature*>* m_friends;
	bool m_atkdThisTurn;
	bool m_tookDmgThisTurn;

	// Position
	int m_x;
	int m_y;
	Cell* m_cell;
	float m_speed;
	float m_movementRemaining;

	uint16_t m_threatLevel;
};

bool checkProfsFromString(const std::string& token, CHECK_TYPE& val);
bool saveProfsFromString(const std::string& token, ABILITY_SCORES& val);
std::string abilToString(ABILITY_SCORES score);
ABILITY_SCORES checkAbility(CHECK_TYPE chk);

std::vector<Creature*> sortCreaturesBy(const std::vector<Creature*> list, std::function<bool(Creature*, Creature*)> pred);
std::vector<Creature*> sortCreaturesByLeastHealth(const std::vector<Creature*> list);
std::vector<Creature*> sortCreaturesByMostThreat(const std::vector<Creature*> list);

#endif//KERF_CREATURE_H

