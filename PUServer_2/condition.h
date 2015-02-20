#ifndef __CONDITION_H_
#define __CONDITION_H_

#include "enums.h"

#include <list>
#include <vector>

class Creature;
class Player;
class PropStream;

enum ConditionType_t {
	CONDITION_NONE           = 0,
	CONDITION_POISON         = 1 << 0,
	CONDITION_FIRE           = 1 << 1,
	CONDITION_ENERGY         = 1 << 2,
	CONDITION_LIFEDRAIN      = 1 << 3,
	CONDITION_HASTE          = 1 << 4,
	CONDITION_PARALYZE	     = 1 << 5,
	CONDITION_OUTFIT         = 1 << 6,
	CONDITION_INVISIBLE      = 1 << 7,
	CONDITION_LIGHT          = 1 << 8,
	CONDITION_MANASHIELD     = 1 << 9,
	CONDITION_INFIGHT        = 1 << 10,
	CONDITION_DRUNK          = 1 << 11,
	CONDITION_EXHAUSTED      = 1 << 12,
	CONDITION_REGENERATION   = 1 << 13,
	CONDITION_SOUL           = 1 << 14,
	CONDITION_DROWN          = 1 << 15,
	CONDITION_MUTED          = 1 << 16,
	CONDITION_ATTRIBUTES     = 1 << 17,
	CONDITION_FREEZING       = 1 << 18,
	CONDITION_DAZZLED        = 1 << 19,
	CONDITION_CURSED         = 1 << 20,
	CONDITION_EXHAUST_COMBAT = 1 << 21,
	CONDITION_EXHAUST_HEAL   = 1 << 22,
	CONDITION_PACIFIED       = 1 << 23,
	CONDITION_HUNTING        = 1 << 24, // Killing monsters
};

enum ConditionEnd_t{
	CONDITIONEND_CLEANUP,
	CONDITIONEND_DIE,
	CONDITIONEND_TICKS,
	CONDITIONEND_ABORT
};

enum ConditionAttr_t{
	CONDITIONATTR_TYPE = 1,
	CONDITIONATTR_ID = 2,
	CONDITIONATTR_TICKS = 3,
	CONDITIONATTR_HEALTHTICKS = 4,
	CONDITIONATTR_HEALTHGAIN = 5,
	CONDITIONATTR_MANATICKS = 6,
	CONDITIONATTR_MANAGAIN = 7,
	CONDITIONATTR_DELAYED = 8,
	CONDITIONATTR_OWNER = 9,
	CONDITIONATTR_INTERVALDATA = 10,
	CONDITIONATTR_SPEEDDELTA = 11,
	CONDITIONATTR_FORMULA_MINA = 12,
	CONDITIONATTR_FORMULA_MINB = 13,
	CONDITIONATTR_FORMULA_MAXA = 14,
	CONDITIONATTR_FORMULA_MAXB = 15,
	CONDITIONATTR_LIGHTCOLOR = 16,
	CONDITIONATTR_LIGHTLEVEL = 17,
	CONDITIONATTR_LIGHTTICKS = 18,
	CONDITIONATTR_LIGHTINTERVAL = 19,
	CONDITIONATTR_SOULTICKS = 20,
	CONDITIONATTR_SOULGAIN = 21,
	CONDITIONATTR_SKILLS = 22,
	CONDITIONATTR_STATS = 23,
	CONDITIONATTR_OUTFIT = 24,
	CONDITIONATTR_PERIODDAMAGE = 25,
	CONDITIONATTR_SKILLSPERCENT = 26,
	CONDITIONATTR_ISBUFF = 27,
	CONDITIONATTR_SUBID = 28,

	//reserved for serialization
	CONDITIONATTR_END      = 254
};

enum ConditionId_t{
	CONDITIONID_DEFAULT = -1,
	CONDITIONID_COMBAT = 0,
	CONDITIONID_HEAD = 1,
	CONDITIONID_NECKLACE = 2,
	CONDITIONID_BACKPACK = 3,
	CONDITIONID_ARMOR = 4,
	CONDITIONID_RIGHT = 5,
	CONDITIONID_LEFT = 6,
	CONDITIONID_LEGS = 7,
	CONDITIONID_FEET = 8,
	CONDITIONID_RING = 9,
	CONDITIONID_AMMO = 10
};

struct IntervalInfo{
	int32_t timeLeft;
	int32_t value;
	int32_t interval;
};

class Condition
{
public:
	Condition(ConditionId_t _id, ConditionType_t _type, int32_t _ticks);
	virtual ~Condition(){};
};

class ConditionGeneric : public Condition
{
public:
	ConditionGeneric(ConditionId_t _id, ConditionType_t _type, int32_t _ticks);
	virtual ~ConditionGeneric(){};
};

class ConditionInvisible : public ConditionGeneric
{
public:
	ConditionInvisible(ConditionId_t _id, ConditionType_t _type, int32_t _ticks);
	virtual ~ConditionInvisible(){};
};

#endif