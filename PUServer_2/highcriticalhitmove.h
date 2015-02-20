#ifndef INC_HIGHCRITICALHITMOVE_H_
#define INC_HIGHCRITICALHITMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"

class HighCriticalHitMove : public PokemonMove
{
public:
	HighCriticalHitMove(PokemonType* type, uint32_t power, double accuracy, uint32_t pp) 
		: PokemonMove(type, power, accuracy, pp) {};

	bool hasHighCriticalHitRate() { return true; };
};
#endif