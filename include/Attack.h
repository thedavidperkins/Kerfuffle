#ifndef KERF_ATTACK_H
#define KERF_ATTACK_H

#include <functional>

class Loadout;

#include "Action.h"
#include "Weapons.h"


class Creature;

enum ATTACK_EFFECT_TYPE
{
	A_POISONING = 0,
	A_CHARGE,
	N_ATTACK_EFFECT_TYPES
};


struct AttackEffect {
	ATTACK_EFFECT_TYPE type;
	int dc;
	ABILITY_SCORES saveType;
	bool isMagic;
};


class Attack {
public:
	Attack(Creature* agent);
	void load(Loadout* loadout, bool dual = false);
	void load(DMG_TYPE type, int atkBonus, const std::string& dmgString, 
		int dmgBonus = -1,
		WEAPON_PROPS props = 0,
		float minRange = 0,
		float maxRange = 5,
		float disRange = 5,
		const std::vector<AttackEffect>& effects = {}
	);
	void load(const Attack& rhs);
	void unload();
	int atk();
	int dmg(Creature* target);
	DMG_TYPE dmgType();
	bool isLoaded() const { return m_loaded; }
	bool isMelee() const { return m_maxRange <= 5; }
	void setDisadvantage(bool dis) { m_disadvantage = dis; }
	void setAdvantage(bool adv) { m_advantage = adv; }
	WEAPON_PROPS getProps() const { return m_curProps; }
	void subtractProp(WEAPON_PROPS_BITS prop) { if (m_curProps & prop) m_curProps -= prop; }
	void getMinMaxDisRange(float& min, float&max, float& dis);
	void restore() { m_curProps = m_props; }

	std::string getUser() const;

	Attack(const Attack& rhs) = delete;
	Attack& operator=(const Attack& rhs) = delete;
private:
	Creature* m_agent;
	int m_atkBonus;
	std::function<int(void)> m_dmgDice;
	std::function<int(void)> m_singleDie; // for savage attacks, etc, when the rule is one additional die on crit
	int m_dmgBonus;
	std::string m_dmgString;
	DMG_TYPE m_type;
	WEAPON_PROPS m_props;
	WEAPON_PROPS m_curProps; // stores changes in properties in specific turn
	bool m_loaded;
	bool m_advantage;
	bool m_disadvantage;
	bool m_crit;
	float m_maxRange;
	float m_minRange;
	float m_disRange;

	std::vector<AttackEffect> m_attackEffects;
};

bool atkActionUsable(Creature* user, const std::vector<Creature*>& friends, const std::vector<Creature*>& enemies, bool dual = false);
bool atkEffectFromString(const std::string& effectName, ATTACK_EFFECT_TYPE& effect);


#endif//KERF_ATTACK_H