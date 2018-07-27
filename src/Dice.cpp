#include "Dice.h"
#include <random>

static std::mt19937 rnd;

static int maxOf(int a, int b) {
	return a > b ? a : b;
}

static int minOf(int a, int b) {
	return a < b ? a : b;
}

// return a random int between 1 and cap inclusive
int randInt(int cap) {
	return (rnd() % cap) + 1;
}

int d4() {
	return randInt(4);
}

int d6() {
	return randInt(6);
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