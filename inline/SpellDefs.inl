#ifndef SPELL_DEF
#define SPELL_DEF(className, spellName, dmgString, dmgType, saveType, actionType, levelBits, effects)
#endif

SPELL_DEF(	AcidSplash,		ACID_SPLASH,	"L4:1d6;L10:2d6;L16:3d6;L20:4d6;",		ACID,	DEX,	ACTION_TIMING_ACTION,		CANTRIP,	60.f,	{}	)
SPELL_DEF(	FireBolt,		FIRE_BOLT,		"L4:1d10;L10:2d10;L16:3d10;L20:4d10;",	FIRE,	DEX,	ACTION_TIMING_ACTION,		CANTRIP,	120.f,	{}	)
SPELL_DEF(	RayOfFrost,		RAY_OF_FROST,	"",										FORCE,	DEX,	ACTION_TIMING_ACTION,		CANTRIP,	60.f,	{}	)
SPELL_DEF(	ShockingGrasp,	SHOCKING_GRASP,	"",										FORCE,	DEX,	ACTION_TIMING_ACTION,		CANTRIP,	5.f,	{}	)
SPELL_DEF(	Thornwhip,		THORNWHIP,		"",										FORCE,	DEX,	ACTION_TIMING_ACTION,		CANTRIP,	30.f,	{}	)
SPELL_DEF(	Grease,			GREASE,			"",										FORCE,	DEX,	ACTION_TIMING_ACTION,		L1,			60.f,	{}	)
SPELL_DEF(	MageArmor,		MAGE_ARMOR,		"",										FORCE,	DEX,	ACTION_TIMING_ACTION,		L1,			5.f,	{}	)
SPELL_DEF(	Shield,			SHIELD,			"",										FORCE,	DEX,	ACTION_TIMING_REACTION,		L1,			0.f,	{}	)
SPELL_DEF(	BurningHands,	BURNING_HANDS,	"",										FORCE,	DEX,	ACTION_TIMING_ACTION,		L1_AND_UP,	15.f,	{}	)
SPELL_DEF(	HealingWord,	HEALING_WORD,	"",										FORCE,	DEX,	ACTION_TIMING_BONUS_ACTION,	L1_AND_UP,	60.f,	{}	)
SPELL_DEF(	MagicMissile,	MAGIC_MISSILE,	"",										FORCE,	DEX,	ACTION_TIMING_ACTION,		L1_AND_UP,	120.f,	{}	)
SPELL_DEF(	Sleep,			SLEEP,			"",										FORCE,	DEX,	ACTION_TIMING_ACTION,		L1_AND_UP,	90.f,	{}	)
SPELL_DEF(	Thunderwave,	THUNDERWAVE,	"",										FORCE,	DEX,	ACTION_TIMING_ACTION,		L1_AND_UP,	15.f,	{}	)

#undef SPELL_DEF
