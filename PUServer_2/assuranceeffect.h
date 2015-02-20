#ifndef INC_ASSURANCEEFFECT_H_
#define INC_ASSURANCEEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

#include "pokemon.h"

class AssuranceEffect : public StatusEffect
{
	int m_health;

public:
	AssuranceEffect() {};

	bool apply(Pokemon* p) {
		m_health = p->getOpponent()->getHealth();

		return true;
	};

	int getHealth() { return m_health; };
	int getTier() { return 5; };

	bool tick(Pokemon* p) {
		p->removeStatus(this);
		return true;
	}
};

#endif