#ifndef KERF_FEATURES_H
#define KERF_FEATURES_H

#include <string>

class Creature;

typedef __int64 FEATURE_BIT;
const FEATURE_BIT F_LUCKY			= 0x1;
const FEATURE_BIT F_BRAVE			= 0x2;
const FEATURE_BIT F_SNEAK_ATTACK	= 0x4;
//const FEATURE_BIT F_HALFLING_NIMBLENESS = 0x??? -- Awaiting move implementation
//const FEATURE_BIT F_NATURALLY_STEALTHY = 0x??? -- Awaiting environment description sufficiently clear for this to make a difference

typedef __int64 FEATURES;

inline int snkAtkDiceCount(int lvl) {
	return (lvl + 1) / 2;
}

bool ftrFrmStr(const std::string& str, FEATURE_BIT& bit);

class FeatureTrkr {
public:
	static FeatureTrkr* makeTracker(FEATURE_BIT type, Creature* owner);
	virtual void reset() = 0;
	virtual ~FeatureTrkr() {}
protected:
	FeatureTrkr(FEATURE_BIT ftre, Creature* owner) : m_ftre(ftre), m_owner(owner) {};
	FeatureTrkr(const FeatureTrkr& rhs) {}
	FEATURE_BIT m_ftre;
	Creature* m_owner;
};

//=================================================================================
//=================================================================================
//=================================================================================

class EmptyTrkr : public FeatureTrkr {
public:
	EmptyTrkr(FEATURE_BIT ftre, Creature* owner) : FeatureTrkr(ftre, owner) {}
	virtual void reset() {}
};

//=================================================================================

class SneakAttackTrkr : public FeatureTrkr {
public:
	SneakAttackTrkr(Creature* owner) : FeatureTrkr(F_SNEAK_ATTACK, owner) {}
	virtual void reset() { m_used = false; }

	bool isUsable(Creature* target);
	int use();
protected:
	bool _targetDistracted(Creature* target);
	bool m_used;
};

//=================================================================================
//=================================================================================
//=================================================================================

template <class T>
inline FEATURE_BIT classBit() { return 0; }

#define BIT_FROM_CLASS(className, bitName) \
template <> inline FEATURE_BIT classBit<className>() { return bitName; }

BIT_FROM_CLASS(SneakAttackTrkr, F_SNEAK_ATTACK)

#endif//KERF_FEATURES_H
