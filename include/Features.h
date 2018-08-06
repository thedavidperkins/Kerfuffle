#ifndef KERF_FEATURES_H
#define KERF_FEATURES_H

#include <vector>
#include <string>

class Creature;

typedef __int64 FEATURE_BIT;

#define FEATURE_DEF(className, bitName, bitVal) \
const FEATURE_BIT F_##bitName = bitVal;

#include "FeatureDefs.inl"

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
#define EMPTY_TRKR(className, bitName)							\
class className : public EmptyTrkr {							\
public:															\
    className(Creature* owner) : EmptyTrkr(bitName, owner) {}	\
};
EMPTY_TRKR(LuckyTrkr, F_LUCKY);
EMPTY_TRKR(BraveTrkr, F_BRAVE);

//=================================================================================

class SneakAttackTrkr : public FeatureTrkr {
public:
	SneakAttackTrkr(Creature* owner) : FeatureTrkr(F_SNEAK_ATTACK, owner) {}
	virtual void reset() { m_used = false; }

	bool isUsable(Creature* target, const std::vector<Creature*>& friends);
	int use();
protected:
	bool _targetDistracted(Creature* target, const std::vector<Creature*>& friends);
	bool m_used;
};

//=================================================================================
//=================================================================================
//=================================================================================

template <class T>
inline FEATURE_BIT classBit() { return 0; }

#define FEATURE_DEF(className, bitName, bitVal) \
template <> inline FEATURE_BIT classBit<className>() { return F_##bitName; }

#include "FeatureDefs.inl"

#endif//KERF_FEATURES_H
