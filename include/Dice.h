#ifndef KERF_DICE_H
#define KERF_DICE_H

#include <functional>
#include <string>

void getDiceSeed(std::stringstream& strm);
void resetDiceSeed(std::stringstream& strm);

enum ROLL_TYPE {
	R_ATTACK_ROLL,
	R_ABILITY_CHECK,
	R_SAVING_THROW,
	R_OTHER
};

int randInt(size_t cap);
int d0(); // stand-in function for when no damage is guaranteed
int d4();
int d6();
int d8();
int d10();
int d12();
int d20();
int d100();

std::function<int(void)> funcFromStr(const std::string& token, int& dmgBonus, std::function<int(void)>& singleDie);

#endif//KERF_DICE_H

