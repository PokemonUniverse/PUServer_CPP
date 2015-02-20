#ifndef INC_SUCKERPUNCHEFFECT_H_
#define INC_SUCKERPUNCHEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

#include "pokemon.h"

class SuckerPunchEffect : public StatusEffect
{
public:
	SuckerPunchEffect() {};

	int getTier() { return 1; };
	bool tick(Pokemon* p) {
		p->removeStatus(this);
		return true;
	}

	std::string getName() { return ""; };
	std::string getDescription() { return ""; };
};
#endif