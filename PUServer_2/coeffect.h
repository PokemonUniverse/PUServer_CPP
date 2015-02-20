#ifndef INC_COEFFECT_H_
#define INC_COEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

/*
 * Removes an effect from the opponent if the user switches out.  
 */
class CoEffect : public StatusEffect
{
	StatusEffect* m_type;

public:
	CoEffect(StatusEffect* type) : m_type(type) {};
	~CoEffect() {};

	StatusEffect* getType() { return m_type; };

	std::string getName() { return ""; };
	std::string getDescription() { return ""; };

	int getTier() { return -1; };

	bool tick(Pokemon* p) { return false; };
	bool switchOut(Pokemon* p) { 
		p->getOpponent()->removeStatus(m_type); 
		return true; 
	};
};
#endif