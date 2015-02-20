#include "pokemon.h" 
#include "pokemonmove.h"
#include "movelistentry.h"

#include "battleturn.h"

BattleTurn* BattleTurn::getSwitchTurn( Pokemon* owner, int32_t i )
{
	BattleTurn* turn = new BattleTurn();
	turn->m_owner = owner;
	turn->m_id = i;
	turn->m_useMove = false;

	return turn;
}

BattleTurn* BattleTurn::getMoveTurn( Pokemon* owner, int32_t i )
{
	BattleTurn* turn = new BattleTurn();
	turn->m_owner = owner;
	turn->m_id = i;
	turn->m_useMove = true;

	return turn;
}

PokemonMove* BattleTurn::getMove( Pokemon* p )
{
	if(!m_useMove) {
		return NULL;
	}

	MoveListEntry *entry = p->getMove(m_id);
	if(!entry) {
		return NULL;
	}

	return entry->getMove();
}

