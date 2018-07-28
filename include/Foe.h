#ifndef KERF_FOE_H
#define KERF_FOE_H

#include "Creature.h"

class Foe : public Creature {
public:
	virtual void takeTurn(std::vector<Creature*>& party, std::vector<Creature*>& foes);
	virtual void takeDamage(Attack* attack) override;
	virtual bool deathCheck() { return false; }
	virtual std::vector<Attack*> getAttackList();
};

#endif // !KERF_FOE_H

