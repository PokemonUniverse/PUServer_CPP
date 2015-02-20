#ifndef INC_PERISHSONGEFFECT_H_
#define INC_PERISHSONGEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class PerishSongEffect : public StatusEffect
{
	bool m_soundImmune;
	int m_turns;
public:
	PerishSongEffect(bool soundImmune) : m_soundImmune(soundImmune) {
		m_turns = 3;
	};

	std::string getName() { return "Perish count"; };
	std::string getDescription() { return ""; };

	int getTier() { return 5; };
	bool hitsThroughSubstitude() { return true; };

	bool apply(Pokemon* p);
	bool tick(Pokemon* p);
};
#endif