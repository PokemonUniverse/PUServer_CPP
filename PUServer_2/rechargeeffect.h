#ifndef INC_RECHARGEEFFECT_H_
#define INC_RECHARGEEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

#include "pokemon.h"

class MoveListEntry;

class RechargeEffect : public StatusEffect
{
	int m_turns;

public:
	RechargeEffect(int turns) : m_turns(turns) {};

	std::string getName() { return "Recharge Effect"; };
	std::string getDescription() { return ""; };

	int getTier() { return 0; };
	
	bool tick(Pokemon* p) {
		if(m_turns-- <= 0) {
			p->removeStatus(this);
		}

		return false;
	}

	bool canSwitch(Pokemon* p) { return false; };
	bool isMoveTransformer(bool enemy) { return !enemy; };
	
	MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move) {
		std::string message = boost::str(boost::format("%s must recharge!") % p->getName());
		p->getField()->showMessage(message);

		return NULL;
	}
};

#endif 