#ifndef INC_ATTRACTEFFECT_H_
#define INC_ATTRACTEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class AttractEffect : public StatusEffect
{
public:
	AttractEffect() {};

	std::string getName() { return "Attract"; };
	bool isPassable() { return false; };
	bool hitsThroughSubstitude() { return true; };
	std::string getDescription() { return "fell in love!"; };
	int getTier() { return -1; };
	bool tick(Pokemon* p) { return false; };

	bool apply(Pokemon* p);
	bool immobilises(Pokemon* p);
};
#endif