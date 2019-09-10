#ifndef FEATURE_DEF
#define FEATURE_DEF(className, bitName, bitVal)
#endif

FEATURE_DEF(Lucky, LUCKY, 0x1, false)
FEATURE_DEF(Brave, BRAVE, 0x2, false)
FEATURE_DEF(SneakAttack, SNEAK_ATTACK, 0x4, false)
FEATURE_DEF(RelentlessEndurance, RELENTLESS_ENDURANCE, 0x8, false)
FEATURE_DEF(SavageAttacks, SAVAGE_ATTACKS, 0x10, false)
FEATURE_DEF(LayOnHands, LAY_ON_HANDS, 0x20, true)
FEATURE_DEF(FeyAncestry, FEY_ANCESTRY, 0x40, false)
FEATURE_DEF(DwarvenResilience, DWARVEN_RESILIENCE, 0x80, false)

#undef FEATURE_DEF