#include <algorithm>
#include <sstream>
#include <iostream>

#include "Encounter.h"
#include "Creature.h"
#include "Dice.h"
#include "Player.h"
#include "Foe.h"
#include "Logger.h"
#include "Ring.h"

Encounter::Encounter(const std::string& defFile) :
	m_party(),
	m_foes(),
	m_partyInitial(),
	m_foesInitial()
{
	if (!_defineFromFile(defFile)) {
		throw std::runtime_error("Failed to construct the desired encounter. Check the file input for syntax.");
	}
}

bool Encounter::_defineFromFile(const std::string& defFile) {
	std::vector<std::string> fileNames;
	std::ifstream in(defFile);
	std::string line;
	std::getline(in, line);
	while (in) {
		fileNames.push_back(line);
		std::getline(in, line);
	}
	for (auto& fl : fileNames) {
		if(!_procFile(fl)) return false;
	}
	return true;
}

bool Encounter::_procFile(const std::string& fileName) {
	std::ifstream file(fileName);
	std::string line;
	std::stringstream out;
	while (file) {
		std::getline(file, line);
		if (line.find("PLAYER") == 0) {
			out << line << std::endl;
			while (line.find("ENDPLAYER") != 0) {
				std::getline(file, line);
				if (!file) return false;
				if ((line[0] == '#') || (line.size() == 0)) continue;
				out << line << std::endl;
			}
			Player* plyr = new Player(out);
			m_partyInitial.push_back(plyr);
			out.clear(); out.str(std::string());
		}
		else if (line.find("FOE") == 0) {
			out << line << std::endl;
			while (line.find("ENDFOE") != 0) {
				std::getline(file, line);
				if (!file) return false;
				if (line[0] == '#') continue;
				out << line << std::endl;
			}
			Foe* foe = new Foe(out);
			m_foesInitial.push_back(foe);
			out.clear(); out.str(std::string());
		}
	}
	file.close();
	return true;
}

Encounter::~Encounter() {
	_depopulateTeams();
	_depopulateInitialTeams();
}

void Encounter::_populateTeams() {
	_depopulateTeams();
	for (auto& plyr : m_partyInitial) {
		m_party.push_back(plyr->makeCopy());
	}
	for (auto& foe : m_foesInitial) {
		m_foes.push_back(foe->makeCopy());
	}

	for (auto& plyr : m_party) {
		plyr->setFriends(&m_party);
		plyr->init();
	}
	for (auto& foe : m_foes) {
		foe->setFriends(&m_foes);
		foe->init();
	}
}

void Encounter::_depopulateTeams() {
	m_party.clear();
	m_foes.clear();
}

void Encounter::_depopulateInitialTeams() {
	for (auto& plyr : m_partyInitial) {
		delete plyr;
	}
	m_partyInitial.clear();
	for (auto& foe : m_foesInitial) {
		delete foe;
	}
	m_foesInitial.clear();
}

bool Encounter::_foesAlive() {
	for (auto& f : m_foes) {
		if (f->isStanding()) return true;
	}
	//std::cout << "All foes have died." << std::endl;
	return false;
}

bool Encounter::_partyAlive() {
	for (auto& p : m_party) {
		if (p->isDead()) {
			//std::cout << p->getName() << " has died." << std::endl;
			return false;
		}
	}
	return true;
}

bool Encounter::fight(int write) {
	Ring& r = Ring::getInstance();
	r.reset();
	_populateTeams();
	if (write >= 0) LOG.openFile("output\\EncLog" + std::to_string(write) + ".txt");
	LOG("==============================================================");
	LOG("                    FIGHT START                               ");
	LOG("==============================================================");

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

	std::sort(turnOrder.begin(), turnOrder.end(), [](Creature* a, Creature* b)->bool {
		if (a->getInit() == b->getInit()) {
			return a->getAMod(DEX) > b->getAMod(DEX);
		}
		return (a->getInit() > b->getInit());
	});

	//=============================================================================
	// Main fight loop -- go until all foes or all party are dead
	//=============================================================================
	int counter = 0;
	bool ret = false;
	while (counter++ < timeOut) {

		LOG("- - - - - - - - - - - - - - - - - - - - - - - - - - -");
		for (auto& cur : turnOrder) {
			if (cur->isStanding()) {
				LOG("++++++++++++++++++++++++++");
				cur->takeTurn(m_party, m_foes);
				LOG(r.draw());
			}
			else if (!cur->isDead() && !cur->isStable()) {
				LOG("++++++++++++++++++++++++++");
				if (cur->deathCheck()) {
					cur->takeTurn(m_party, m_foes);
				}
			}
		}

		// Check for end of fight; return true if party wins or false if they lose
		if (!_partyAlive()) {
			ret = false;
			break;
		}
		if (!_foesAlive()) {
			ret = true;
			break;
		}
	}

	LOG("==============================================================");
	LOG(std::string("                    FIGHT END - ") + (ret ? "PLAYERS WIN" : "FOES WIN"));
	LOG("==============================================================");

	if (write >= 0) LOG.closeFile();
	return ret;
}