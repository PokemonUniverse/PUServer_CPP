#ifndef INC_MOVESY_H_
#define INC_MOVESY_H_

#include "definitions.h"
#include "statusmove.h"

#include "yawneffect.h"

class Pokemon;
class BattleMechanics;

struct Yawn : public StatusMove
{
	Yawn() {
		statusEffectList_t a1;
		a1.push_back(new YawnEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GROUND, 0, 1.0, 15, a1, a2, a3);
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		return true;
	};
};
#endif