#ifndef INC_MOVESZ_H_
#define INC_MOVESZ_H_

#include "definitions.h"
#include "statusmove.h"

#include "paralysiseffect.h"
#include "flincheffect.h"

struct ZapCannon : public StatusMove
{
	ZapCannon() {
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_ELECTRIC, 100, 0.5, 5, a1, a2, a3);
	};
};

struct ZenHeadbutt : public StatusMove
{
	ZenHeadbutt() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.2);

		StatusMove(PokemonType::T_PSYCHIC, 80, 0.9, 15, a1, a2, a3);
	};
};
#endif