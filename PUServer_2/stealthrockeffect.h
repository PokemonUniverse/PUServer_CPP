#ifndef INC_STEALTHROCKEFFECT_H_
#define INC_STEALTHROCKEFFECT_H_

#include "definitions.h"
#include "spikeseffect.h"

class BattleField;
class Pokemon;

class StealthRockEffect : public SpikesEffect
{
public:
	StealthRockEffect() { 
		m_maxLayers = 1;
		m_message = "";
	};

	std::string getName() { return "Stealth rock"; };

	void switchIn(Pokemon *p);

	bool applyToField(BattleField* field) {
		field->showMessage("Pointed stones float in the air around foes's team!");
		return true;
	}
};
#endif