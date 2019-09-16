#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>

#include "Dice.h"
#include "Encounter.h"
#include "Ring.h"

int main() {
	/*try*/ {
		Encounter e("resources\\defFiles.txt");
		int wincount = 0;
		int total = 500;
		bool write = true;
		for (int iter = 0; iter < total; ++iter) {
			if (e.fight(iter)) {
				++wincount;
			}
			if (iter % 100 == 0) {
				std::cout << "\t\tPercentage complete: " << (double)iter / (double)total << std::endl;
			}
		}
		std::cout << "==========================================================" << std::endl;
		std::cout << "Win percentage: " << (double)wincount / (double)total << std::endl;
	}
	/*catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}*/
}