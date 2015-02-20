#ifndef __MOVELISTENTRY_H_
#define __MOVELISTENTRY_H_

#include "definitions.h"
#include "pokemonmove.h"

class PokemonMove;

class MoveListEntry
{
public:
	MoveListEntry(std::string name, PokemonMove *pmove);

	MoveListEntry* clone() {
		return new MoveListEntry(*this);
	}

	bool equals(MoveListEntry* entry) {
		return (entry->m_name == m_name);
	};

	std::string getName() { return m_name; };
	PokemonMove* getMove() { return m_move; };

protected:
	std::string m_name;
	PokemonMove* m_move;
private:
};
#endif