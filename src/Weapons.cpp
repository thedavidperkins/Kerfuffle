#include <stdexcept>

#include "Weapons.h"

std::string gWeaponNames[] = {
	"CLUB",
	"DAGGER",
	"GREATCLUB",
	"HANDAXE",
	"JAVELIN",
	"LIGHT_HAMMER",
	"MACE",
	"QUARTERSTAFF",
	"SICKLE",
	"SPEAR",
	"LIGHT_CROSSBOW",
	"DART",
	"SHORTBOW",
	"SLING",
	"BATTLEAXE",
	"FLAIL",
	"GLAIVE",
	"GREATAXE",
	"GREATSWORD",
	"HALBERD",
	"LANCE",
	"LONGSWORD",
	"MAUL",
	"MORNINGSTAR",
	"PIKE",
	"RAPIER",
	"SCIMITAR",
	"SHORTSWORD",
	"TRIDENT",
	"WAR_PICK",
	"WARHAMMMER",
	"WHIP",
	"BLOWGUN",
	"HAND_CROSSBOW",
	"HEAVY_CROSSBOW",
	"LONGBOW",
	"NET",
	"UNARMED"
};

WEAPON_PROPS gWeaponProps[] = {
	MELEE | SIMPLE | LIGHT,											//CLUB
	MELEE | SIMPLE | FINESSE | LIGHT | THROWN,						//DAGGER
	MELEE | SIMPLE | TWO_HANDED,									//GREATCLUB
	MELEE | SIMPLE | LIGHT | THROWN,								//HANDAXE
	MELEE | SIMPLE | THROWN,										//JAVELIN
	MELEE | SIMPLE | LIGHT | THROWN,								//LIGHT_HAMMER
	MELEE | SIMPLE,													//MACE
	MELEE | SIMPLE | VERSATILE,										//QUARTERSTAFF
	MELEE | SIMPLE | LIGHT,											//SICKLE
	MELEE | SIMPLE | THROWN | VERSATILE,							//SPEAR
	RANGED | SIMPLE | AMMUNITION | LOADING | TWO_HANDED,			//LIGHT_CROSSBOW
	RANGED | SIMPLE | FINESSE | THROWN,								//DART
	RANGED | SIMPLE | AMMUNITION | TWO_HANDED,						//SHORTBOW
	RANGED | SIMPLE | AMMUNITION,									//SLING
	MELEE | MARTIAL | VERSATILE,									//BATTLEAXE
	MELEE | MARTIAL,												//FLAIL
	MELEE | MARTIAL | HEAVY | REACH | TWO_HANDED,					//GLAIVE
	MELEE | MARTIAL | HEAVY | TWO_HANDED,							//GREATAXE
	MELEE | MARTIAL | HEAVY | TWO_HANDED,							//GREATSWORD
	MELEE | MARTIAL | HEAVY | REACH | TWO_HANDED,					//HALBERD
	MELEE | MARTIAL | REACH | SPECIAL,								//LANCE
	MELEE | MARTIAL | VERSATILE,									//LONGSWORD
	MELEE | MARTIAL | HEAVY | TWO_HANDED,							//MAUL
	MELEE | MARTIAL,												//MORNINGSTAR
	MELEE | MARTIAL | HEAVY | REACH | TWO_HANDED,					//PIKE
	MELEE | MARTIAL | FINESSE,										//RAPIER
	MELEE | MARTIAL | FINESSE | LIGHT,								//SCIMITAR
	MELEE | MARTIAL | FINESSE | LIGHT,								//SHORTSWORD
	MELEE | MARTIAL | THROWN | VERSATILE,							//TRIDENT
	MELEE | MARTIAL,												//WAR_PICK
	MELEE | MARTIAL | VERSATILE,									//WARHAMMMER
	MELEE | MARTIAL | FINESSE | REACH,								//WHIP
	RANGED | MARTIAL | AMMUNITION | LOADING,						//BLOWGUN
	RANGED | MARTIAL | AMMUNITION | LIGHT | LOADING,				//HAND_CROSSBOW
	RANGED | MARTIAL | AMMUNITION | HEAVY | LOADING | TWO_HANDED,	//HEAVY_CROSSBOW
	RANGED | MARTIAL | AMMUNITION | HEAVY | TWO_HANDED,				//LONGBOW
	RANGED | MARTIAL | SPECIAL | THROWN,							//NET
	0																//UNARMED
};

std::function<int(void)> gWeaponDmg[] = {
	d4,								//CLUB
	d4,								//DAGGER
	d8,								//GREATCLUB
	d6,								//HANDAXE
	d6,								//JAVELIN
	d4,								//LIGHT_HAMMER
	d6,								//MACE
	d6,								//QUARTERSTAFF
	d4,								//SICKLE
	d6,								//SPEAR
	d8,								//LIGHT_CROSSBOW
	d4,								//DART
	d6,								//SHORTBOW
	d4,								//SLING
	d8,								//BATTLEAXE
	d8,								//FLAIL
	d10,							//GLAIVE
	d12,							//GREATAXE
	[]() { return d6() + d6(); },	//GREATSWORD
	d10,							//HALBERD
	d12,							//LANCE
	d8,								//LONGSWORD
	[]() { return d6() + d6(); },	//MAUL
	d8,								//MORNINGSTAR
	d10,							//PIKE
	d8,								//RAPIER
	d6,								//SCIMITAR
	d6,								//SHORTSWORD
	d6,								//TRIDENT
	d8,								//WAR_PICK
	d8,								//WARHAMMMER
	d4,								//WHIP
	[]() { return 1; } ,			//BLOWGUN
	d6,								//HAND_CROSSBOW
	d10,							//HEAVY_CROSSBOW
	d8,								//LONGBOW
	d0,								//NET
	d4								//UNARMED
};

std::function<int(void)> gVersWeaponDmg[] = {
	d4,								//CLUB
	d4,								//DAGGER
	d8,								//GREATCLUB
	d6,								//HANDAXE
	d6,								//JAVELIN
	d4,								//LIGHT_HAMMER
	d6,								//MACE
	d8,								//QUARTERSTAFF
	d4,								//SICKLE
	d8,								//SPEAR
	d8,								//LIGHT_CROSSBOW
	d4,								//DART
	d6,								//SHORTBOW
	d4,								//SLING
	d10,							//BATTLEAXE
	d8,								//FLAIL
	d10,							//GLAIVE
	d12,							//GREATAXE
	[]() { return d6() + d6(); },	//GREATSWORD
	d10,							//HALBERD
	d12,							//LANCE
	d10,							//LONGSWORD
	[]() { return d6() + d6(); },	//MAUL
	d8,								//MORNINGSTAR
	d10,							//PIKE
	d8,								//RAPIER
	d6,								//SCIMITAR
	d6,								//SHORTSWORD
	d8,								//TRIDENT
	d8,								//WAR_PICK
	d10,							//WARHAMMMER
	d4,								//WHIP
	[]() { return 1; } ,			//BLOWGUN
	d6,								//HAND_CROSSBOW
	d10,							//HEAVY_CROSSBOW
	d8,								//LONGBOW
	d0,								//NET
	d4								//UNARMED
};

DMG_TYPE gDamageTypes[] = {
	BLUDGEONING,	//CLUB
	PIERCING,		//DAGGER
	BLUDGEONING,	//GREATCLUB
	SLASHING,		//HANDAXE
	PIERCING,		//JAVELIN
	BLUDGEONING,	//LIGHT_HAMMER
	BLUDGEONING,	//MACE
	BLUDGEONING,	//QUARTERSTAFF
	SLASHING,		//SICKLE
	PIERCING,		//SPEAR
	PIERCING,		//LIGHT_CROSSBOW
	PIERCING,		//DART
	PIERCING,		//SHORTBOW
	BLUDGEONING,	//SLING
	SLASHING,		//BATTLEAXE
	BLUDGEONING,	//FLAIL
	SLASHING,		//GLAIVE
	SLASHING,		//GREATAXE
	SLASHING,		//GREATSWORD
	SLASHING,		//HALBERD
	PIERCING,		//LANCE
	SLASHING,		//LONGSWORD
	BLUDGEONING,	//MAUL
	PIERCING,		//MORNINGSTAR
	PIERCING,		//PIKE
	PIERCING,		//RAPIER
	SLASHING,		//SCIMITAR
	PIERCING,		//SHORTSWORD
	PIERCING,		//TRIDENT
	PIERCING,		//WAR_PICK
	BLUDGEONING,	//WARHAMMMER
	SLASHING,		//WHIP
	PIERCING,		//BLOWGUN
	PIERCING,		//HAND_CROSSBOW
	PIERCING,		//HEAVY_CROSSBOW
	PIERCING,		//LONGBOW
	BLUDGEONING,	//NET
	BLUDGEONING		//UNARMED
};

static int maxDamage(WEAPON_TYPE wep) {
	switch (wep)
	{
	case NET:
		return 0;
		break;
	case BLOWGUN:
		return 1;
		break;
	case CLUB:
	case DAGGER:
	case LIGHT_HAMMER:
	case SICKLE:
	case DART:
	case SLING:
	case WHIP:
	case UNARMED:
		return 4;
		break;
	case HANDAXE:
	case JAVELIN:
	case MACE:
	case QUARTERSTAFF:
	case SPEAR:
	case SHORTBOW:
	case SCIMITAR:
	case SHORTSWORD:
	case TRIDENT:
	case HAND_CROSSBOW:
		return 6;
		break;
	case GREATCLUB:
	case LIGHT_CROSSBOW:
	case BATTLEAXE:
	case FLAIL:
	case LONGSWORD:
	case MORNINGSTAR:
	case RAPIER:
	case WAR_PICK:
	case WARHAMMMER:
	case LONGBOW:
		return 8;
		break;
	case GLAIVE:
	case HALBERD:
	case PIKE:
	case HEAVY_CROSSBOW:
		return 10;
		break;
	case GREATAXE:
	case GREATSWORD:
	case LANCE:
	case MAUL:
		return 12;
		break;
	default:
		throw std::runtime_error("Weapon type not recognized.");
	}
}

static int minDamage(WEAPON_TYPE wep) {
	if (wep == NET) {
		return 0;
	}
	if (wep == GREATSWORD || wep == MAUL) {
		return 2;
	}
	return 1;
}

WEAPON_TYPE mostDamage(WEAPON_TYPE first, WEAPON_TYPE second) {
	int maxDmg1 = maxDamage(first);
	int maxDmg2 = maxDamage(second);
	if (maxDmg1 == maxDmg2) {
		return minDamage(first) > minDamage(second) ? first : second;
	}
	return maxDmg1 > maxDmg2 ? first : second;
}

WEAPON_TYPE mostDamage(std::vector<WEAPON_TYPE>& typeList) {
	if (typeList.empty()) return UNARMED;
	WEAPON_TYPE ret = typeList[0];
	for (size_t iter = 1; iter < typeList.size(); ++iter) {
		ret = mostDamage(ret, typeList[iter]);
	}
	return ret;
}

bool weaponFromString(const std::string& wepName, WEAPON_TYPE& wep) {
	for (wep = CLUB; wep != N_WEAPON_TYPE; wep = (WEAPON_TYPE)(wep + 1)) {
		if (wepName == gWeaponNames[wep]) {
			return true;
		}
	}
	return false;
}

bool propFromString(const std::string& propName, WEAPON_PROPS_BITS& bit) {
	if (propName == "MELEE") {
		bit = MELEE;
	}
	else if (propName == "RANGED") {
		bit = RANGED;
	}
	else if (propName == "SIMPLE") {
		bit = SIMPLE;
	}
	else if (propName == "MARTIAL") {
		bit = MARTIAL;
	}
	else if (propName == "AMMUNITION") {
		bit = AMMUNITION;
	}
	else if (propName == "FINESSE") {
		bit = FINESSE;
	}
	else if (propName == "HEAVY") {
		bit = HEAVY;
	}
	else if (propName == "LIGHT") {
		bit = LIGHT;
	}
	else if (propName == "LOADING") {
		bit = LOADING;
	}
	else if (propName == "REACH") {
		bit = REACH;
	}
	else if (propName == "SPECIAL") {
		bit = SPECIAL;
	}
	else if (propName == "THROWN") {
		bit = THROWN;
	}
	else if (propName == "TWO_HANDED") {
		bit = TWO_HANDED;
	}
	else if (propName == "VERSATILE") {
		bit = VERSATILE;
	}
	else if (propName == "SILVERED") {
		bit = SILVERED;
	}
	else {
		return false;
	}
	return true;
}

bool dmgTypeFromString(const std::string& typeName, DMG_TYPE& type) {
	if (typeName == "PIERCING") {
		type = PIERCING;
	}
	else if (typeName == "SLASHING") {
		type = SLASHING;
	}
	else if (typeName == "BLUDGEONING") {
		type = BLUDGEONING;
	}
	else if (typeName == "FORCE") {
		type = FORCE;
	}
	else if (typeName == "NECROTIC") {
		type = NECROTIC;
	}
	else if (typeName == "POISON") {
		type = POISON;
	}
	else if (typeName == "FIRE") {
		type = FIRE;
	}
	else if (typeName == "FROST") {
		type = FROST;
	}
	else if (typeName == "RADIANT") {
		type = RADIANT;
	}
	else {
		return false;
	}
	return true;
}