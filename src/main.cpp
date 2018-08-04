#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>

#include "Dice.h"
#include "Encounter.h"

int main() {
	try {
		Encounter e("resources\\defFiles.txt");
		int wincount = 0;
		int total = 10000;
		bool write = true;
		for (int iter = 0; iter < total; ++iter) {
			if (e.fight(write)) {
				++wincount;
				write = false;
			}
		}
		std::cout << "Win percentage: " << (double)wincount / (double)total << std::endl;
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}
}