#ifndef KERF_PLAYER_H
#define KERF_PLAYER_H

#include <sstream>

#include "Creature.h"

class BonusAction;

class Player : public Creature {
public:
	Player(std::stringstream& defStream);

	virtual void takeTurn(std::vector<Creature*>& party, std::vector<Creature*>& foes) override;
	virtual void takeDamage(Attack* attack) override;
	virtual bool deathCheck() override;
	virtual bool loadNextAttack(Attack* atk) override;
	virtual void getAttackList(std::vector<Attack*>& atks) override;
	virtual void cleanupAttackList(std::vector<Attack*>& atks) override;
	
	void loadOffhandAttack(Attack& atk);
	int dmgBonus(WEAPON_TYPE weapon);
	int atkBonus(WEAPON_TYPE weapon);
	void usedBonusAction() { m_bonus--; }

	virtual Player* makeCopy();
private:
	virtual bool _defineFromStream(std::stringstream& defStream, std::string& errStatus);
	Player(const Player* rhs);
	Player();

	bool _isProficient(WEAPON_TYPE weapon);
	int _getAbltyMod(WEAPON_TYPE weapon);
	WEAPON_TYPE _chooseWeaponForAttack();

	// bonus actions
	int m_bonus;
	std::vector<BonusAction*> m_bonusActions;

	// weapons and gear
	std::vector<WEAPON_TYPE> m_weaponsOwned;
	std::vector<WEAPON_TYPE> m_weaponProfs;
	WEAPON_TYPE m_offhandWeapon;
	WEAPON_PROPS m_propProfs;
	bool m_hasShield; // only used for weapon choices/abilities -- ac boost should be rolled into total ac (no swapping out, for now)
	bool m_hasOffhandWeapon;
	bool m_hasSpecialWeapon;
	bool m_hasOffhandSpecialWeapon;
	Attack m_specialWeaponAttack;
	Attack m_specialOffhandWeaponAttack;
	WEAPON_PROPS m_specialWeaponProps;
	WEAPON_PROPS m_specialOffhandWeaponProps;

	bool m_noMoreAttacks; //used for canceling attacks after using a loaded weapon
	bool m_firstAttack;
	int m_nAttacks

	int m_profBonus;
};

#endif // !KERF_PLAYER_H
