#ifndef KERF_ENCOUNTER_H
#define KERF_ENCOUNTER_H

#include <vector>
#include <string>
#include <fstream>

class Creature;

class Encounter {
public:
	Encounter(const std::string& defFile);
	~Encounter();
	bool fight(int write = -1);
private:
	bool _defineFromFile(const std::string& defFile);
	bool _procFile(const std::string& fileName);
	void _populateTeams();
	void _depopulateTeams();
	void _depopulateInitialTeams();

	bool _partyAlive();
	bool _foesAlive();

	std::vector<Creature*> m_party;
	std::vector<Creature*> m_foes;

	std::vector<Creature*> m_partyInitial;
	std::vector<Creature*> m_foesInitial;
};

#endif // !KERF_ENCOUNTER_H
