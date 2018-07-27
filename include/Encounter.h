#ifndef KERF_ENCOUNTER_H
#define KERF_ENCOUNTER_H

#include <vector>

class Creature;

class Encounter {
public:
	bool fight();
private:
	bool _partyAlive();
	bool _foesAlive();

	std::vector<Creature*> m_party;
	std::vector<Creature*> m_foes;
};

#endif // !KERF_ENCOUNTER_H
