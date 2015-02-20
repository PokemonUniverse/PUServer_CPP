#include "jewelmechanics.h"

#include "pokemonmove.h"
#include "movelistentry.h"
#include "pokemontype.h"

bool JewelMechanics::isMoveSpecial( PokemonMove *move )
{
	MoveListEntry* entry = move->getMoveListEntry();
	if(entry == NULL) {
		return move->getType()->isSpecial();
	}

	jewelMovesList_t::iterator it = m_moves.find(entry->getName());
	if(it == m_moves.end()) {
		std::cout << "[Warning] no move type entry for " << entry->getName() << "!" << std::endl;
		return move->getType()->isSpecial();
	}

	return it->second;
}