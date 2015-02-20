#ifndef INC_PERFECTACCURACYMOVE_H_
#define INC_PERFECTACCURACYMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"

#include "invulnerablestateeffect.h"
#include "pokemon.h"

class PokemonType;
class BattleMechanics;

class PerfectAccuracyMove : public PokemonMove
{
public:
	PerfectAccuracyMove(PokemonType* type, uint32_t power, uint32_t pp)
		: PokemonMove(type, power, 0, pp) {};

	static bool isHit(BattleMechanics* mech, Pokemon* user, Pokemon* target) {
		return !target->hasEffect<InvulnerableStateEffect>();
	}

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		return isHit(mech, attacker, defender);
	}
};
#endif