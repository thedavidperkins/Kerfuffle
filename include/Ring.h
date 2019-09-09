#ifndef KERF_RING_H
#define KERF_RING_H

#include <string>

class Ring;

// Currently, Ring will be a static 50ft x 50ft room with no
// unusual characteristics. Each 5x5 square will be a 
// cell, and Creatures will be in melee range if they reside
// in a neighboring cell (including corner neighbors)
//
// Creatures may not share cells or move through enemy-occupied
// cells except as allowed by character features

const int RING_SIZE = 10; // 10 5ft cells on a side

class Creature;

class Cell {
public:
	Cell();

	void setCoords(int x, int y) { m_x = x; m_y = y; }
	std::vector<Creature*>& getOccupants() { return m_occupants; }
	void addOccupant(Creature* c) { m_occupants.push_back(c); }
	void removeOccupant(Creature* c);
	size_t nOccupants() const { return m_occupants.size(); }
	void clear() { m_occupants.clear(); }
	int x() const { return m_x; }
	int y() const { return m_y; }
	std::string coordString();
private:
	int m_x;
	int m_y;
	std::vector<Creature*> m_occupants;
};

class Ring {
public:
	static Ring& getInstance() {
		static Ring gRing;
		return gRing;
	}

	std::string draw(); // ASCII representation of ring

	Cell* assignCell(int x, int y, Creature* c);
	std::vector<Cell*> getNeighboringCells(int x, int y);
	std::vector<Cell*> getNeighboringCells(Cell* c);
	std::vector<Cell*> sortCellsBy(std::function<bool(const Cell*, const Cell*)> pred);
	std::vector<Cell*> sortCellsByDistance(Cell* c);
	std::vector<Cell*> cellsInRange(Cell* c, float range);

	void reset();
private:
	Ring();
	Ring(const Ring&) = delete;
	Ring& operator=(const Ring&) = delete;

	Cell m_arena[RING_SIZE][RING_SIZE];
};

float dist(int x1, int y1, int x2, int y2);
float dist(const Cell* a, int x, int y);
float dist(const Cell* a, const Cell* b);

#endif // !KERF_RING_H

