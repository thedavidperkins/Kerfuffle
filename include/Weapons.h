#ifndef KERF_WEAPONS_H
#define KERF_WEAPONS_H

#include <functional>
#include <vector>
#include "Dice.h"

enum DMG_TYPE {
	PIERCING = 0,
	SLASHING,
	BLUDGEONING,
	FORCE,
	NECROTIC,
	POISON,
	FIRE,
	FROST,
	RADIANT
};

enum WEAPON_TYPE {
	CLUB = 0,
	DAGGER,
	GREATCLUB,
	HANDAXE,
	JAVELIN,
	LIGHT_HAMMER,
	MACE,
	QUARTERSTAFF,
	SICKLE,
	SPEAR,
	LIGHT_CROSSBOW,
	DART,
	SHORTBOW,
	SLING,
	BATTLEAXE,
	FLAIL,
	GLAIVE,
	GREATAXE,
	GREATSWORD,
	HALBERD,
	LANCE,
	LONGSWORD,
	MAUL,
	MORNINGSTAR,
	PIKE,
	RAPIER,
	SCIMITAR,
	SHORTSWORD,
	TRIDENT,
	WAR_PICK,
	WARHAMMMER,
	WHIP,
	BLOWGUN,
	HAND_CROSSBOW,
	HEAVY_CROSSBOW,
	LONGBOW,
	NET,
	UNARMED,
	N_WEAPON_TYPE
};

enum WEAPON_PROPS_BITS {
	MELEE = 0x1,
	RANGED = 0x2,
	SIMPLE = 0x4,
	MARTIAL = 0x8,
	AMMUNITION = 0x10,
	FINESSE = 0x20,
	HEAVY = 0x40,
	LIGHT = 0x80,
	LOADING = 0X100,
	REACH = 0X200,
	SPECIAL = 0x400,
	THROWN = 0x800,
	TWO_HANDED = 0x1000,
	VERSATILE = 0x2000,
	SILVERED = 0x4000	
};
typedef unsigned int WEAPON_PROPS;

extern std::string gWeaponNames[];
extern WEAPON_PROPS gWeaponProps[];
extern std::function<int(void)> gWeaponDmg[];
extern std::function<int(void)> gVersWeaponDmg[];
extern DMG_TYPE gDamageTypes[];

extern std::string gDamageTypeNames[];

inline bool isLight(WEAPON_TYPE wep) {
	return gWeaponProps[wep] & LIGHT;
}

inline bool isThrown(WEAPON_TYPE wep) {
	return gWeaponProps[wep] & THROWN;
}

inline bool isVersatile(WEAPON_TYPE wep) {
	return gWeaponProps[wep] & VERSATILE;
}

inline bool isRanged(WEAPON_TYPE wep) {
	return gWeaponProps[wep] & RANGED;
}

inline bool isFinesse(WEAPON_TYPE wep) {
	return gWeaponProps[wep] & FINESSE;
}

inline bool isLoading(WEAPON_TYPE wep) {
	return gWeaponProps[wep] & LOADING;
}

inline bool isTwoHanded(WEAPON_TYPE wep) {
	return gWeaponProps[wep] & TWO_HANDED;
}

int maxDamage(WEAPON_TYPE wep);
int minDamage(WEAPON_TYPE wep);
WEAPON_TYPE mostDamage(WEAPON_TYPE first, WEAPON_TYPE second);
WEAPON_TYPE mostDamage(std::vector<WEAPON_TYPE>& typeList);
bool weaponFromString(const std::string& wepName, WEAPON_TYPE& wep);
bool propFromString(const std::string& propName, WEAPON_PROPS_BITS& bit);
bool dmgTypeFromString(const std::string& typeName, DMG_TYPE& type);
std::string dmgTypeToString(DMG_TYPE type);
std::string wepDmgString(WEAPON_TYPE wep);
std::string wepDmgStringVersatile(WEAPON_TYPE wep);
void wepMinMaxDisRange(WEAPON_TYPE wep, int& min, int& max, int& dis);
std::function<int(void)> wepSingleDie(WEAPON_TYPE wep);

#endif//KERF_WEAPONS_H

