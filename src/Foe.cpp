#include "Foe.h"

void Foe::takeTurn(std::vector<Creature*>& party, std::vector<Creature*>& foes) {
	Creature::takeTurn(foes, party);
}

void Foe::takeDamage(Attack* attack) {
	Creature::takeDamage(attack);
	if (m_HP <= 0) {
		alive = false; dead = true;
	}
}