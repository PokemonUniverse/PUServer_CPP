#ifndef INC_MINIMIZEEFFECT_H_
#define INC_MINIMIZEEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;
class MoveListEntry;

class MinimizeEffect : public StatusEffect
{
public:
	MinimizeEffect() {};
	
	std::string getName() { return "Minimize"; };
	std::string getDescription() { return ""; };

	int getTier() { return -1; };

	bool tick(Pokemon* p) { return false; };
	bool isMoveTransformer(bool enemy) { return enemy; };

	MoveListEntry* getEnemyTransformedMove(Pokemon *p, MoveListEntry *move);

	bool isPassable() { return false; };

	void informDuplicateEffect(Pokemon *p) {};
};
#endif