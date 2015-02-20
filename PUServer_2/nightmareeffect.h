#ifndef INC_NIGHTMAREEFFECT_H_
#define INC_NIGHTMAREEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class NightmareEffect : public StatusEffect
{
public:
	NightmareEffect() {};

	std::string getName() { return "Nightmare"; };
	std::string getDescription() { return "fell into a nightmare!"; };

	bool apply(Pokemon* p);
	bool tick(Pokemon* p);

	int getTier() { return 4; };
};
#endif