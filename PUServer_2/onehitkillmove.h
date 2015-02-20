#ifndef INC_ONEHITKILLMOVE_H_
#define INC_ONEHITKILLMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"

class Pokemon;
class PokemonType;
class BattleMechanics;

class OneHitKillMove : public PokemonMove
{
public:
	OneHitKillMove(PokemonType* type, uint32_t pp) : PokemonMove(type, 0, 0, pp) {};

	bool isAttack() { return true; };

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);
	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);
};
#endif