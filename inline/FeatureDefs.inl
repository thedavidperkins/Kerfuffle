#ifndef FEATURE_DEF
#define FEATURE_DEF(className, bitName, bitVal)
#endif

FEATURE_DEF(Lucky, LUCKY, 0x1)
FEATURE_DEF(Brave, BRAVE, 0x2)
FEATURE_DEF(SneakAttack, SNEAK_ATTACK, 0x4)
FEATURE_DEF(RelentlessEndurance, RELENTLESS_ENDURANCE, 0x8)
FEATURE_DEF(SavageAttacks, SAVAGE_ATTACKS, 0x10)
FEATURE_DEF(LayOnHands, LAY_ON_HANDS, 0x20)
FEATURE_DEF(FeyAncestry, FEY_ANCESTRY, 0x40)

#undef FEATURE_DEF