#ifndef INC_DEFENSECURLEFFECT_H_
#define INC_DEFENSECURLEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;
class MoveListEntry;

class DefenseCurlEffect : public StatusEffect
{
public:
	DefenseCurlEffect() {};

	std::string getName() { return "Defense Curl"; };
	std::string getDescription() { return ""; };
	int getTier() { return -1; };
	bool tick(Pokemon* p) { return false; };
	bool isMoveTransformer(bool enemy) { return !enemy; };
	bool isPassable() { return false; };
	void informDuplicateEffect(Pokemon *p) {};

	MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move);
};
#endif