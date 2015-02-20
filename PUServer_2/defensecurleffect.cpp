#include "defensecurleffect.h"

#include "pokemon.h"
#include "movelistentry.h"

MoveListEntry* DefenseCurlEffect::getTransformedMove( Pokemon *p, MoveListEntry *move )
{
	if(move->getName() == "Rollout") {
		PokemonMove* pmove = move->getMove();
		pmove->setPower(pmove->getPower() * 2);
	}

	return move;
}