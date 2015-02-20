#include "minimizeeffect.h"

#include "movelistentry.h"
#include "pokemonmove.h"

MoveListEntry* MinimizeEffect::getEnemyTransformedMove( Pokemon *p, MoveListEntry *move )
{
	if(move->getName() == "Stomp" || move->getName() == "Extrasensory") {
		PokemonMove *pmove = move->getMove();
		pmove->setPower(pmove->getPower() * 2);
	}

	return move;
}