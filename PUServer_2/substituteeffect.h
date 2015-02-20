#ifndef INC_SUBSTITUTEEFFECT_H_
#define INC_SUBSTITUTEEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class StatusEffect;
class Pokemon;

class SubstituteEffect : public StatusEffect
{
public:
	std::string getName() { return "Substitute"; };
	std::string getDescription() { return " made a substitute!"; };
	
	int getTier() { return -1; };

	bool tick(Pokemon* p) { return false; };
	bool apply(Pokemon* p) { 
		p->getField()->refreshActivePokemon();
		return true; 
	};

	void unapply(Pokemon* p) {
		p->getField()->refreshActivePokemon();
	};

	bool switchOut(Pokemon* p) {
		p->setSubstitute(0);
		return StatusEffect::switchOut(p);
	}

};
#endif