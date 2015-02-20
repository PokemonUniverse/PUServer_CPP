#ifndef INC_MOVESQ_H_
#define INC_MOVESQ_H_

#include "definitions.h"
#include "prioritymove.h"

#include "pokemontype.h"

struct QuickAttack : public PriorityMove
{
	QuickAttack() : PriorityMove(PokemonType::T_NORMAL, 40, 1.0, 30, 1) {};
};

#endif