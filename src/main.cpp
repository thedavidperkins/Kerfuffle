#include <iostream>
#include <vector>
#include <numeric>

#include "Dice.h"

int main() {
	int testN = 1000000;

	std::vector<int> twenties;
	std::vector<int> advs;
	std::vector<int> diss;

	for (int i = 0; i < testN; ++i) {
		twenties.push_back(d20());
		advs.push_back(d20adv());
		diss.push_back(d20dis());
	}

	int twenSum = std::accumulate(twenties.begin(), twenties.end(), 0);
	int advSum = std::accumulate(advs.begin(), advs.end(), 0);
	int disSum = std::accumulate(diss.begin(), diss.end(), 0);

	std::cout << "d20 avg: " << ((double)twenSum) / ((double)testN) << std::endl;
	std::cout << "d20 avg with advantage: " << ((double)advSum) / ((double)testN) << std::endl;
	std::cout << "d20 avg with disadvantage: " << ((double)disSum) / ((double)testN) << std::endl;
}