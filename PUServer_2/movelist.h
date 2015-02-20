#ifndef __MOVELIST_H_
#define __MOVELIST_H_

#include <boost/unordered_map.hpp>

#include "definitions.h"
#include "movelistentry.h"
#include "tools.h"

class MoveList
{
public:
	MoveList();
	~MoveList();

	static MoveList* getDefaultData() {
		return m_instance;
	};

	MoveListEntry* getRandomMove() {
		int rnd = random_range(0, m_movesNames.size()-1);
		std::string name = m_movesNames.at(rnd);

		return m_moves.at(name);
	};

	MoveListEntry* getMove(std::string moveName);
	MoveListEntry* getMove(uint32_t idx) {
		return getMove(m_movesNames.at(idx));
	};

private:
    /**
     * Initialise all moves that do not have a special effect.
     */
	void initNonStatusMoves();

	/**
	 * Initialise all moves that have a special effect or are an exeption on the standard formula
	 */
	void initStatusMoves();

	/**
	 * Initialise all moves from database mapped move id as key and name as value
	 */ 
	void initNamesMoves();

	typedef boost::unordered_map<std::string, MoveListEntry*> moveList_t;
	moveList_t m_moves;

	typedef boost::unordered_map<uint32_t, std::string> moveNameList_t;
	moveNameList_t m_movesNames;

	static MoveList *m_instance;
};

#endif