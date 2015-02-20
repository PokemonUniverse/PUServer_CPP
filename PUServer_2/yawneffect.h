#ifndef INC_YAWNEFFECT_H_
#define INC_YAWNEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"
#include "sleepeffect.h"

#include "pokemon.h"

class YawnEffect : public StatusEffect
{
	int m_turns;

	class YawnSleep : public SleepEffect
	{
	public:
		YawnSleep() {};
		bool hitsThroughSubstitude() { return true; };
	};

public:
	YawnEffect() {
		m_turns = 1;
	}

	std::string getName() { return "Yawn"; };
	std::string getDescription() { return "became drowsy!"; };

	int getTier() { return 5; };
	bool hitsThroughSubstitude() { return false; };
	bool isPassable() { return false; };

	bool tick(Pokemon* p)
	{
		if(m_turns-- <= 0) {
			p->addStatus(p->getOpponent(), new YawnSleep());
			p->removeStatus(this);

			return false;
		}

		return true;
	}
};
#endif