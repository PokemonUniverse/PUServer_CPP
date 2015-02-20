#ifndef INC_TOXICSPIKESEFFECT_H_
#define INC_TOXICSPIKESEFFECT_H_

#include "definitions.h"
#include "spikeseffect.h"

class Pokemon;

class ToxicSpikesEffect : public SpikesEffect
{
public:
	ToxicSpikesEffect() { 
		m_maxLayers = 2;
		m_message = "Toxic Spikes were scattered around the foe's team!";
	};

	std::string getName() { return "Toxic spikes"; };

	void switchIn(Pokemon *p);
};
#endif