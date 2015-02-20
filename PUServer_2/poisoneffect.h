#ifndef INC_POISONEFFECT_H_
#define INC_POISONEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class PoisonEffect : public StatusEffect
{
public:
	PoisonEffect() {
		m_lock = SPECIAL_EFFECT_LOCK;
	};
	~PoisonEffect() {};

	std::string getName() { return "Poison"; };
	std::string getDescription() { return "was poisoned!"; };

	// It stays...
	bool switchOut(Pokemon* p) { return false; };
	bool tick(Pokemon* p);

	bool apply(Pokemon* p);
	void unapply(Pokemon* p) {};

	bool immobilises(Pokemon* p) { return false; };

	int getTier() { return 3; };
};
#endif