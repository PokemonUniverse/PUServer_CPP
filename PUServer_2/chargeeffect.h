#ifndef INC_CHARGEEFFECT_H_
#define INC_CHARGEEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;
class MoveListEntry;

class ChargeEffect : public StatusEffect
{
public:
	//************************************
	// Method:    ChargeEffect
	// FullName:  ChargeEffect::ChargeEffect
	// Access:    public 
	// Returns:   
	// Qualifier: Initialise a new charge effect.
	// Parameter: int turns - number of turns to charge
	// Parameter: std::string msg - message to display when effect is apllied
	// Parameter: MoveListEntry * move - move to use after charge is complete
	//************************************
	ChargeEffect(int turns, std::string msg, MoveListEntry* move) : m_turns(turns), m_move(move), m_msg(msg) {};
	~ChargeEffect() {};

	std::string getName() { return "Charge"; };

	void setTurns(int turns) { m_turns = turns; };

	bool isMoveTransformer(bool enemy) { return !enemy; };
	MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move);

	bool apply(Pokemon* p);
	void unapply(Pokemon* p) {};

	bool canSwitch(Pokemon* p) { return false; };
	int getTier() { return 0; };

	bool tick(Pokemon* p) {
		--m_turns;
		return false;
	}

	//************************************
	// Method:    getMove
	// FullName:  ChargeEffect::getMove
	// Access:    public 
	// Returns:   MoveListEntry*
	// Qualifier: Returns the move that will be used after the carge finishes.
	//************************************
	MoveListEntry* getMove() { return m_move; };

private:
	int m_turns;
	MoveListEntry* m_move;
	std::string m_msg;
};


#endif