#ifndef INC_COUNTEREFFECT_H_
#define INC_COUNTEREFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;
class MoveListEntry;

class CounterEffect : public StatusEffect
{
	int m_count;

public:
	CounterEffect() : m_count(1) {};

	bool tick(Pokemon* p) { return false; };
	
	std::string getName() { return ""; };
	std::string getDescription() { return ""; };

	int getTier() { return -1; };

	bool isMoveTransformer(bool enemy) { return !enemy; };

	MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move);

	void informDuplicateEffect(Pokemon *p) {}; // Do nothing
};
#endif