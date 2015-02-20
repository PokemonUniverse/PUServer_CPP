#ifndef __BATTLETURN_H_
#define __BATTLETURN_H_

#include "definitions.h"

class Pokemon;
class PokemonMove;

class BattleTurn
{
public:
	~BattleTurn();

	//************************************
	// Method:    getSwitchTurn
	// FullName:  BattleTurn::getSwitchTurn
	// Access:    public 
	// Returns:   BattleTurn*
	// Qualifier: Get a BattleTurn object that represents switching in the identified pokemon
	// Parameter: int i
	//************************************
	static BattleTurn* getSwitchTurn(Pokemon *owner, int32_t i);

	//************************************
	// Method:    getMoveTurn
	// FullName:  BattleTurn::getMoveTurn
	// Access:    public static 
	// Returns:   BattleTurn*
	// Qualifier: Get a BattleTurn object that represents using the identified move
	// Parameter: int32_t i
	//************************************
	static BattleTurn* getMoveTurn(Pokemon *owner, int32_t i);

	BattleTurn* clone() {
		return new BattleTurn(*this);
	};

	int32_t getId() { return m_id; };
	bool isMoveTurn() { return m_useMove; };

	PokemonMove* getMove(Pokemon* p);

	void setTarget(Pokemon* target) { m_target = target; };
	Pokemon* getTarget() { return m_target; };
	Pokemon* getOwner() { return m_owner; };

protected:
	bool m_useMove;
	int32_t m_id;
	
	Pokemon* m_owner;
	Pokemon* m_target;

private:
	BattleTurn() : m_useMove(false), m_id(-1), m_target(NULL), m_owner(NULL) {
		// Prevent this class from being instanced directly
	};
};
#endif