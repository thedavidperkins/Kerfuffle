#ifndef KERF_PLAYER_H
#define KERF_PLAYER_H

#include "Creature.h"

class Player : public Creature {
public:
	virtual void takeTurn(std::vector<Creature*>& party, std::vector<Creature*>& foes) override;
	virtual void takeDamage(Attack* attack) override;
	virtual bool deathCheck() override;
	virtual bool loadNextAttack(Attack* atk) override;
	virtual std::vector<Attack*> getAttackList() override;

	int dmgBonus(WEAPON_TYPE weapon);
	int atkBonus(WEAPON_TYPE weapon);

private:
	bool _isProficient(WEAPON_TYPE weapon);
	int _getAbltyMod(WEAPON_TYPE weapon);
	WEAPON_TYPE _chooseWeaponForAttack();

	// weapons and gear
	std::vector<WEAPON_TYPE> m_weaponsOwned;
	std::vector<WEAPON_TYPE> m_weaponProfs;
	WEAPON_PROPS m_propProfs;
	bool m_hasShield;
	bool m_hasSpecialWeapon;
	bool m_hasOffhandSpecialWeapon;
	Attack m_specialWeaponAttack;
	WEAPON_PROPS m_specialWeaponProps;

	bool m_noMoreAttacks; //used for canceling attacks after using a loaded weapon

	int m_profBonus;
};

#endif // !KERF_PLAYER_H
