#ifndef INC_MOVESO_H_
#define INC_MOVESO_H_

#include "definitions.h"
#include "statusmove.h"
#include "pokemontype.h"
#include "rampagemove.h"

#include "statchangeeffect.h"
#include "multiplestatchangeeffect.h"
#include "jewelmechanics.h"

struct Octazooka : public StatusMove
{
	Octazooka() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ACCURACY, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.5);

		StatusMove(PokemonType::T_WATER, 65, 0.85, 10, a1, a2, a3);
	};
};

struct OminousWind : public StatusMove
{
	OminousWind() {
		intArray_t stats;
		stats.push_back(S_ATTACK);
		stats.push_back(S_DEFENCE);
		stats.push_back(S_SPEED);
		stats.push_back(S_SPATTACK);
		stats.push_back(S_SPDEFENCE);

		statusEffectList_t a1;
		a1.push_back(new MultipleStatChangeEffect(stats));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_GHOST, 60, 1.0, 5, a1, a2, a3);
	};
};

struct Outrage : public RampageMove
{
	Outrage() : RampageMove(PokemonType::T_DRAGON, 120, 1.0, 15) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		setPower((int)(((double)getPower()) / 120.0 * (isAssignableFrom<JewelMechanics*>(mech) ? 120.0 : 90.0)));
		return RampageMove::use(mech, attacker, defender);
	};
};

struct Overheat : public StatusMove
{
	Overheat() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPATTACK, false, 2));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_FIRE, 140, 0.9, 5, a1, a2, a3);
	};
};
#endif