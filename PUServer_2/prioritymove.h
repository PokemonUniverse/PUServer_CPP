#ifndef INC_PRIORITYMOVE_H_
#define INC_PRIORITYMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"

class PriorityMove : public PokemonMove
{
	int m_priority; 

public:
	PriorityMove(PokemonType* type, uint32_t power, double accuracy, uint32_t pp, int priority)
		: PokemonMove(type, power, accuracy, pp), m_priority(priority) {};

	int getPriority() { return m_priority; };
};
#endif