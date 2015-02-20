#ifndef INC_LEECHSEEDEFFECT_H_
#define INC_LEECHSEEDEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class LeechSeedEffect : public StatusEffect
{
	bool m_grassImmune;

public:
	LeechSeedEffect() : m_grassImmune(false) {};
	LeechSeedEffect(bool grassImmune) : m_grassImmune(grassImmune) {};

	std::string getName() { return "Leech Seed"; };
	std::string getDescription() { return "was seeded!"; };

	int getTier() { return 3; };

	bool tick(Pokemon* p);
	bool apply(Pokemon* p);
};
#endif