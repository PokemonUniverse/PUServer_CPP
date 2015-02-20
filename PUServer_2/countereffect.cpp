#include "countereffect.h"

#include "movelistentry.h"
#include "pokemon.h"
#include "pokemonmove.h"
#include "protectmove.h"

MoveListEntry* CounterEffect::getTransformedMove( Pokemon *p, MoveListEntry *move )
{
	PokemonMove* pmove = move->getMove();

	if(isAssignableFrom<ProtectMove*>(pmove) && p->getLastMove() != NULL)
	{
		++m_count;
		ProtectMove *protectMove = dynamic_cast<ProtectMove*>(pmove);
		protectMove->setFailure(1.0 - (1.0 / (double)(1 << m_count) ) );
	}
	else {
		p->removeStatus(this);
	}

	return move;
}