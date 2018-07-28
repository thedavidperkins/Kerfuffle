#include <stdexcept>

#include "Weapons.h"

WEAPON_PROPS gWeaponProps[] = {
	LIGHT,															//CLUB
	FINESSE | LIGHT | THROWN,										//DAGGER
	TWO_HANDED,														//GREATCLUB
	LIGHT | THROWN,													//HANDAXE
	THROWN,															//JAVELIN
	LIGHT | THROWN,													//LIGHT_HAMMER
	0,																//MACE
	VERSATILE,														//QUARTERSTAFF
	LIGHT,															//SICKLE
	VERSATILE | THROWN,												//SPEAR
	AMMUNITION | RANGED | LOADING | TWO_HANDED,						//LIGHT_CROSSBOW
	RANGED | FINESSE | THROWN,										//DART
	AMMUNITION | RANGED | TWO_HANDED,								//SHORTBOW
	AMMUNITION | RANGED,											//SLING
	VERSATILE | MARTIAL,											//BATTLEAXE
	MARTIAL,														//FLAIL
	TWO_HANDED | MARTIAL | HEAVY | REACH,							//GLAIVE
	TWO_HANDED | MARTIAL | HEAVY,									//GREATAXE
	TWO_HANDED | MARTIAL | HEAVY,									//GREATSWORD
	TWO_HANDED | MARTIAL | HEAVY | REACH,							//HALBERD
	MARTIAL | REACH | SPECIAL,										//LANCE
	VERSATILE | MARTIAL,											//LONGSWORD
	TWO_HANDED | MARTIAL | HEAVY,									//MAUL
	MARTIAL,														//MORNINGSTAR
	TWO_HANDED | MARTIAL | HEAVY | REACH,							//PIKE
	MARTIAL | FINESSE,												//RAPIER
	MARTIAL | FINESSE | LIGHT,										//SCIMITAR
	MARTIAL | FINESSE | LIGHT,										//SHORTSWORD
	VERSATILE | MARTIAL | THROWN,									//TRIDENT
	MARTIAL,														//WAR_PICK
	VERSATILE | MARTIAL,											//WARHAMMMER
	MARTIAL | FINESSE | REACH,										//WHIP
	AMMUNITION | RANGED | MARTIAL | LOADING,						//BLOWGUN
	AMMUNITION | RANGED | MARTIAL | LIGHT | LOADING,				//HAND_CROSSBOW
	AMMUNITION | RANGED | MARTIAL | HEAVY | LOADING | TWO_HANDED,	//HEAVY_CROSSBOW
	AMMUNITION | RANGED | MARTIAL | HEAVY | TWO_HANDED,				//LONGBOW
	MARTIAL | RANGED | SPECIAL | THROWN,							//NET
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