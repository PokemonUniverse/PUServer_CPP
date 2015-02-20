#ifndef INC_INGRAINEFFECT_H_
#define INC_INGRAINEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;

class IngrainEffect : public StatusEffect
{
public:
	IngrainEffect() {};

	std::string getName() { return "Ingrain"; };
	std::string getDescription() { return "planted its roots"; };

	int getTier() { return 3; };

	bool tick(Pokemon* p);
	bool canSwitch(Pokemon* p);
};

#endif