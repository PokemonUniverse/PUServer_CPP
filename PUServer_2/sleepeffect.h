#ifndef INC_SLEEPEFFECT_H_
#define INC_SLEEPEFFECT_H_

#include <boost/format.hpp>

#include "definitions.h"
#include "statuseffect.h"

#include "fixedattackeffect.h"

class StatusEffect;

class SleepEffect : public StatusEffect
{
public:
	SleepEffect(int32_t turns = 0) {
		m_lock = SPECIAL_EFFECT_LOCK;
		m_turns = turns;
	};

	~SleepEffect() {};

	std::string getName() { return "Sleep"; };

	static SleepEffect* getDummy() { return m_dummy; };

	bool tick(Pokemon* p) { return false; };
	int getTier() { return 0; };
	bool switchOut(Pokemon* p) { return false; };
	
	bool apply(Pokemon* p);
	void unapply(Pokemon* p) {};

	std::string getDescription() { return " fell asleep!"; };
	bool immobilises(Pokemon* p);

private:
	int32_t m_turns;

	static SleepEffect* m_dummy;
};

#endif