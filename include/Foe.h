#ifndef KERF_FOE_H
#define KERF_FOE_H

#include <sstream>

#include "Creature.h"

class Foe : public Creature {
public:
	Foe(std::stringstream& defStream);
	~Foe();

	virtual void takeTurn(std::vector<Creature*>& party, std::vector<Creature*>& foes) override;
	virtual void takeDamage(Attack* attack) override;
	virtual bool deathCheck() { return false; }
	virtual bool prepNextAttack(Attack* atk, Creature* target);
	virtual void getAttackList(std::vector<Attack*>& atks) override;
	virtual void cleanupAttackList(std::vector<Attack*>& atks) override;
	virtual Foe* makeCopy();
	virtual bool hasAttackProp(WEAPON_PROPS_BITS prop, bool dual = false);
	virtual int getMaxAtkRange(bool dual = false);

private:
	virtual bool _defineFromStream(std::stringstream& defStream, std::string& errStatus);
	Foe(const Foe* rhs);
	Foe();

	std::vector<Attack*> m_attacks;
};

#endif // !KERF_FOE_H

