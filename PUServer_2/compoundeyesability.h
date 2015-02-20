#ifndef __COMPOUNDEYESABILITY_H_
#define __COMPOUNDEYESABILITY_H_

#include "definitions.h"
#include "intrinsicability.h"

class Pokemon;
class MoveListEntry;

class CompoundEyesAbility : IntrinsicAbility
{
public:
	CompoundEyesAbility() : IntrinsicAbility("Compoundeyes") {};

	bool isMoveTransformer(bool enemy) { return !enemy; };

	MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move);
};
#endif