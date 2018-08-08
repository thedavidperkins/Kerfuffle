#include <sstream>
#include <random>

#include "Dice.h"

static std::mt19937 rnd;

static int maxOf(int a, int b) {
	return a > b ? a : b;
}

static int minOf(int a, int b) {
	return a < b ? a : b;
}

void getDiceSeed(std::stringstream& strm) {
	strm << rnd;
}

void resetDiceSeed(std::stringstream& strm) {
	strm >> rnd;
}

// return a random int between 1 and cap inclusive
int randInt(int cap) {
	return (rnd() % cap) + 1;
}

int d0() {
	return 0;
}

int d4() {
	return randInt(4);
}

int d6() {
	return randInt(6);
}

int twod6() {
	return d6() + d6();
}

int d8() {
	return randInt(8);
}

int d10() {
	return randInt(10);
}

int d12() {
	return randInt(12);
}

int d20() {
	return randInt(20);
}

int d20adv() {
	return maxOf(d20(), d20());
}

int d20dis() {
	return minOf(d20(), d20());
}

int d100() {
	return randInt(100);
}

std::function<int(void)> funcFromStr(const std::string& token, int& dmgBonus, std::function<int(void)>& singleDie) {
	std::stringstream procToken(token);
	int quant;
	int die;
	dmgBonus = 0;
	procToken >> quant;
	procToken.get();
	procToken >> die;
	char c = procToken.get();
	if (procToken) {
		procToken >> dmgBonus;
		if (c == '-') dmgBonus *= -1;
	}
	switch (die) {
	case 4:
		singleDie = d4;
		break;
	case 6:
		singleDie = d6;
		break;
	case 8:
		singleDie = d8;
		break;
	case 10:
		singleDie = d10;
		break;
	case 12:
		singleDie = d12;
		break;
	case 20:
		singleDie = d20;
		break;
	case 100:
		singleDie = d100;
		break;
	default:
		throw std::runtime_error("Error: invalid die requested.");
	}
	return [=]() {
		int res = 0;
		for (int iter = 0; iter < quant; ++iter) {
			res += singleDie();
		}
		return res;
	};
}