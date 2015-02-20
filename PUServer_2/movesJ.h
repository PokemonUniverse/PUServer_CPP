#ifndef INC_MOVESJ_H_
#define INC_MOVESJ_H_

#include "definitions.h"
#include "pokemontype.h"
#include "jumpkickmove.h"

struct Judgment : public PokemonMove
{
	Judgment() : PokemonMove(PokemonType::T_NORMAL, 100, 1.0, 10) {};
};

struct JumpKick : public JumpKickMove
{
	JumpKick() : JumpKickMove(PokemonType::T_FIGHTING, 70, 85, 0.95, 20) {};
};

#endif