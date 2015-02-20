#include "compoundeyesability.h"

#include "movelistentry.h"
#include "pokemonmove.h"

MoveListEntry* CompoundEyesAbility::getTransformedMove( Pokemon *p, MoveListEntry *move )
{
	PokemonMove *pmove = move->getMove();
	pmove->setAccuracy(pmove->getAccuracy() * 1.3);

	return move;
}