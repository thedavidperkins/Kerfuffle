#include <algorithm>

#include "Encounter.h"
#include "Creature.h"
#include "Dice.h"

bool Encounter::_foesAlive() {
	for (auto& f : m_foes) {
		if (f->alive) return true;
	}
	return false;
}

bool Encounter::_partyAlive() {
	for (auto& p : m_party) {
		if (p->alive) return true;
	}
	return false;
}

bool Encounter::fight() {
	int timeOut = 1000; // prevent infinite fights 
	//=============================================================================
	// Determine initiative order -- load a turn order list, initiative roll on all 
	//	elements, and sort largest to smallest
	//=============================================================================
	std::vector<Creature*> turnOrder = m_party;
	turnOrder.insert(turnOrder.end(), m_foes.begin(), m_foes.end());
	
	for (auto& c : turnOrder) {
		c->initRoll();
	}

	std::sort(turnOrder.begin(), turnOrder.end(), [](Creature* a, Creature* b) {
		if (a->init == b->init) {
			return a->dex >= b->dex;
		}
		return a->init > b->init;
	});

	//=============================================================================
	// Main fight loop -- go until all foes or all party are dead
	//=============================================================================
	int counter = 0;
	while (counter++ < timeOut) {
		for (auto& cur : turnOrder) {
			if (cur->alive) {
				cur->takeTurn(m_party, m_foes);
			}
			else if (!cur->dead && !cur->stable) {
				if (cur->deathCheck()) {
					cur->takeTurn(m_party, m_foes);
				}
			}
		}

		// Check for end of fight; return true if party wins or false if they lose
		if (!_partyAlive()) return false;
		if (!_foesAlive()) return true;
	}

	return false;
}