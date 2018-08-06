#ifndef KERF_PLAYER_H
#define KERF_PLAYER_H

#include <sstream>

#include "Creature.h"
#include "Loadout.h"

class BonusAction;

class Player : public Creature {
public:
	Player(std::stringstream& defStream);
	virtual ~Player();

	virtual void takeTurn(std::vector<Creature*>& party, std::vector<Creature*>& foes) override;
	virtual void takeDamage(Attack* attack) override;
	virtual bool deathCheck() override;
	virtual bool prepNextAttack(Attack* atk, Creature* target) override;
	virtual void getAttackList(std::vector<Attack*>& atks) override;
	virtual void cleanupAttackList(std::vector<Attack*>& atks) override;
	virtual bool hasAttackProp(WEAPON_PROPS_BITS prop);
	virtual void incentivizeProp(WEAPON_PROPS_BITS prop);
	virtual int getMaxAtkRange();

	void loadOffhandAttack(Attack& atk);
	void usedBonusAction() { m_bonus--; }

	int nAttacks() const { return m_nAttacks; }
	int getLvl() const { return m_level; }

	int getProfBonus() { return m_profBonus; }

	virtual Player* makeCopy();
private:
	virtual bool _defineFromStream(std::stringstream& defStream, std::string& errStatus);
	Player(const Player* rhs);
	Player();

	bool _tryInsertLoadout(Loadout* l);
	int _getAbltyMod(WEAPON_TYPE weapon);
	void _checkLoadoutSwap();
	void _populateLoadouts(
		const std::vector<WEAPON_TYPE>& weps,
		const std::vector<WEAPON_TYPE>& profs,
		WEAPON_PROPS propProfs,
		bool dual,
		bool shield
	);
	void _sortLoadouts(Creature* target = nullptr);
	void _cleanupBonusActions();

	int m_level;

	// bonus actions
	int m_bonus;
	std::vector<BonusAction*> m_bonusActions;

	// weapons and gear
	LOADOUT_PRIORITY m_loadPriority;
	std::vector<Loadout*> m_loadouts;
	int m_chosenLoadout;
	bool m_swappedLoadout;
	WEAPON_PROPS m_propIncentives;

	bool m_noMoreAttacks; //used for canceling attacks after using a loaded weapon
	bool m_firstAttack;
	int m_nAttacks;

	int m_profBonus;
};

#endif // !KERF_PLAYER_H
