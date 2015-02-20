#ifndef INC_TOXICEFFECT_H_
#define INC_TOXICEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;

class ToxicEffect : public StatusEffect
{
public:
	ToxicEffect() {
		m_lock = SPECIAL_EFFECT_LOCK;
		m_turns = 1;
	};
	~ToxicEffect () {};

	std::string getName() { return "Toxic"; };
	std::string getDescription() { return "was badly poisoned!"; };

	bool switchOut(Pokemon* p) {
		m_turns = 1;
		return false;
	};

	/**
     * Toxic removes 1/16, 2/16, 3/16, etc. max health each round
     * up to a maximum of 8/16.
     */
	bool tick(Pokemon* p);

	int getTier() { return 3; };

	bool apply(Pokemon* p);
	void unapply(Pokemon* p) {};

	bool immobilises(Pokemon* p) { return false; };

private:
	int m_turns;
};
#endif