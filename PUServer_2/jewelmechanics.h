#ifndef INC_JEWELMECHANICS_H_
#define INC_JEWELMECHANICS_H_

#include "definitions.h"
#include "advancedmechanics.h"

#include <boost/unordered_map.hpp>

class PokemonMove;

class JewelMechanics : public AdvancedMechanics
{
public:
	JewelMechanics() {};
	virtual ~JewelMechanics() {};
	
	//TODO: Load jewel moves types from database and put them in the map
	static void loadMovesTypes() {};

	/**
     * Return whether a given move deals special damage.
     * In Diamond/Pearl, this is based on the move, not its type.
     */
	virtual bool isMoveSpecial(PokemonMove *move);

private:
	typedef boost::unordered_map<std::string, bool> jewelMovesList_t;
	jewelMovesList_t m_moves;
};
#endif