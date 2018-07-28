#ifndef KERF_WEAPONS_H
#define KERF_WEAPONS_H

#include <functional>
#include <vector>
#include "Dice.h"

enum DMG_TYPE {
	PIERCING,
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
	UNARMED
};

enum WEAPON_PROPS_BITS {
	RANGED = 0x1,		// assume melee if not ranged
	MARTIAL = 0x2,		// assume simple if not martial
	AMMUNITION = 0x4,
	FINESSE = 0x8,
	HEAVY = 0x10,
	LIGHT = 0x20,
	LOADING = 0X40,
	REACH = 0X80,
	SPECIAL = 0x100,
	THROWN = 0x200,
	TWO_HANDED = 0x400,
	VERSATILE = 0x800,
	SILVERED = 0x1000
};
typedef unsigned int WEAPON_PROPS;

extern WEAPON_PROPS gWeaponProps[];
extern std::function<int(void)> gWeaponDmg[];
extern std::function<int(void)> gVersWeaponDmg[];
extern DMG_TYPE gDamageTypes[];

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

WEAPON_TYPE mostDamage(WEAPON_TYPE first, WEAPON_TYPE second);
WEAPON_TYPE mostDamage(std::vector<WEAPON_TYPE>& typeList);

#endif//KERF_WEAPONS_H

