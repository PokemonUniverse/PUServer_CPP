#ifndef INC_BURNEFFECT_H_
#define INC_BURNEFFECT_H_

#include <math.h>
#include <boost/format.hpp>

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;

class BurnEffect : public StatusEffect
{
public:
	BurnEffect() {
		m_lock = SPECIAL_EFFECT_LOCK;
	};
	~BurnEffect() {};

	std::string getName() { return "Burn"; };
	std::string getDescription() { return " was burned!"; };

	bool switchOut(Pokemon* p) { return false; };
	bool tick(Pokemon* p);

	int getTier() { return 3; };

	bool apply(Pokemon* p);
	void unapply(Pokemon* p);

	bool immobilises(Pokemon* p) { return false; };
};

#endif