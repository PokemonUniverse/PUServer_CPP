#include "pokemonmove.h"
#include "movelistentry.h"

MoveListEntry::MoveListEntry(std::string name, PokemonMove *pmove) : m_name(name), m_move(pmove)
{
	if(m_move->getMoveListEntry() != NULL) {
		std::cout << m_name << " is used by two MoveListEntries!" << std::endl;
	}

	m_move->setMoveListEntry(this);
}