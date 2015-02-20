#ifndef INC_MULTIPLEHITMOVE_H_
#define INC_MULTIPLEHITMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"

class MultipleHitMove : public PokemonMove
{
public:
	MultipleHitMove(PokemonType* type, uint32_t power, double accuracy, uint32_t pp)
		: PokemonMove(type, power, accuracy, pp) {};
	~MultipleHitMove() {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);
};
#endif