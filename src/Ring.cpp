#include <algorithm>
#include <sstream>

#include "Creature.h"
#include "Ring.h"

Cell::Cell() :
	m_x(0),
	m_y(0),
	m_occupants()
{}

void Cell::removeOccupant(Creature* c) {
	m_occupants.erase(std::remove_if(m_occupants.begin(), m_occupants.end(), [&](Creature* test) {
		return c == test;
	}), m_occupants.end());
}

std::string Cell::coordString() {
	return std::to_string(m_x) + ", " + std::to_string(m_y);
}

Ring::Ring() :
	m_arena()
{
	for (int i = 0; i < RING_SIZE; ++i) {
		for (int j = 0; j < RING_SIZE; ++j) {
			m_arena[i][j].setCoords(i, j);
		}
	}
}

void Ring::reset() {
	for (int i = 0; i < RING_SIZE; ++i) {
		for (int j = 0; j < RING_SIZE; ++j) {
			m_arena[i][j].clear();
		}
	}
}

static std::string shortName(const std::string& name) {
	std::string ret = name;
	ret.resize(8);
	return ret;
}

std::string Ring::draw() {
	std::stringstream s;
	for (int i = 0; i < RING_SIZE; ++i) {
		s << "-------------------------------------------------------------------------------------------" << std::endl;
		size_t maxCount = 0;
		size_t rowIter = 0;
		while (rowIter++ <= maxCount) {
			for (int j = 0; j < RING_SIZE; ++j) {
				size_t myCount = m_arena[i][j].nOccupants();
				if (myCount > maxCount) maxCount = myCount;
				s << "|" << ((myCount > (rowIter - 1)) ? shortName(m_arena[i][j].getOccupants()[rowIter - 1]->getName()) : "        ");
			}
			s << "|" << std::endl;
		}
		for (int j = 0; j < RING_SIZE; ++j) {
			s << "|  " << m_arena[i][j].coordString() << "  ";
		}
		s << "|" << std::endl;
	}
	s << "-------------------------------------------------------------------------------------------" << std::endl;
	return s.str();
}

Cell* Ring::assignCell(int x, int y, Creature* c) {
	if (m_arena[x][y].nOccupants() == 0) {
		m_arena[x][y].addOccupant(c);
		return &m_arena[x][y];
	}
	std::vector<Cell*> ncs = sortCellsByDistance(&m_arena[x][y]);
	for (auto cell : ncs) {
		if (cell->nOccupants() == 0) {
			cell->addOccupant(c);
			return cell;
		}
	}
	return nullptr;
}

std::vector<Cell*> Ring::getNeighboringCells(int x, int y) {
	int left = x - 1;
	if (left < 0) left = 0;
	int right = x + 1;
	if (right > RING_SIZE - 1) right = RING_SIZE - 1;
	int down = y - 1;
	if (down < 0) down = 0;
	int up = y + 1;
	if (up > RING_SIZE - 1) up = RING_SIZE - 1;

	std::vector<Cell*> ret;

	for (int i = left; i <= right; ++i) {
		for (int j = down; j <= up; ++j) {
			if (i == x && j == y) continue;
			ret.push_back(&m_arena[i][j]);
		}
	}

	// favor cells nearer the center of the arena
	std::sort(ret.begin(), ret.end(), [&](const Cell* a, const Cell* b) {
		return dist(a, RING_SIZE / 2, RING_SIZE / 2) < dist(b, RING_SIZE / 2, RING_SIZE / 2);
	});

	return ret;
}

std::vector<Cell*> Ring::getNeighboringCells(Cell* c) {
	return getNeighboringCells(c->x(), c->y());
}

std::vector<Cell*> Ring::sortCellsBy(std::function<bool(const Cell*, const Cell*)> pred) {
	std::vector<Cell*> ret;
	for (int i = 0; i < RING_SIZE; ++i) {
		for (int j = 0; j < RING_SIZE; ++j) {
			ret.push_back(&m_arena[i][j]);
		}
	}
	std::sort(ret.begin(), ret.end(), pred);
	return ret;
}

std::vector<Cell*> Ring::sortCellsByDistance(Cell* c) {
	return sortCellsBy([&](const Cell* a, const Cell* b) {
		int da = dist(a, c);
		int db = dist(b, c);
		if (da == db) {
			return dist(a, RING_SIZE / 2, RING_SIZE / 2) < dist(b, RING_SIZE / 2, RING_SIZE / 2);
		}
		return da < db;
	});
}

std::vector<Cell*> Ring::cellsInRange(Cell* c, int range) {
	std::vector<Cell*> ret;
	for (int i = 0; i < RING_SIZE; ++i) {
		bool foundInRow = false;
		for (int j = 0; j < RING_SIZE; ++j) {
			Cell* test = &m_arena[i][j];
			if (dist(test, c) <= range) {
				foundInRow = true;
				ret.push_back(test);
			}
			else if (foundInRow) {
				// if we've already found some in the row, and then we don't,
				// we're guaranteed not to find more in this row because a 
				// circle doesn't work that way.
				break;
			}
		}
	}
	return ret;
}

int dist(int x1, int y1, int x2, int y2) {
	int xd2 = (x2 - x1) * (x2 - x1);
	int yd2 = (y2 - y1) * (y2 - y1);
	return (int)(5 * (sqrt((double)xd2 + (double)yd2)));
}

int dist(const Cell* a, int x, int y) {
	return dist(a->x(), a->y(), x, y);
}

int dist(const Cell* a, const Cell* b) {
	return dist(a->x(), a->y(), b->x(), b->y());
}