#ifndef KERF_DICE_H
#define KERF_DICE_H

#include <functional>
#include <string>

int randInt(int cap);
int d0(); // stand-in function for when no damage is guaranteed
int d4();
int d6();
int d8();
int d10();
int d12();
int d20();
int d20adv();
int d20dis();
int d100();

std::function<int(void)> funcFromStr(const std::string& token);

#endif//KERF_DICE_H

