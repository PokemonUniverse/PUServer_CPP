#ifndef INC_SPEEDSWAPEFFECT_H_
#define INC_SPEEDSWAPEFFECT_H_

#include "definitions.h"
#include "fieldeffect.h"

class BattleField;

class SpeedSwapEffect : public FieldEffect
{
	int m_turns;

public:
	SpeedSwapEffect() : m_turns(5) {};

	bool applyToField(BattleField* field) { return true; };

	bool tickField(BattleField* field);
	void unapplyToField(BattleField* field);

	int getTier() { return 1; };
	std::string getDescription() { return ""; };
	std::string getName() { return "Trick room"; };

};
#endif