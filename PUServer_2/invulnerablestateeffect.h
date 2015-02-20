#ifndef INC_INVULNERABLESTATEEFFECT_H_
#define INC_INVULNERABLESTATEEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

#include <vector>

class Pokemon;
class BattleMechanics;
class MoveListEntry;

class InvulnerableStateEffect : public StatusEffect
{
	int m_turns;
	std::vector<std::string> m_effectiveMoves;

public:
	InvulnerableStateEffect(std::vector<std::string> effectiveMoves) : m_effectiveMoves(effectiveMoves) {};

	std::string getName() { return "Invulnerable state"; };
	std::string getDescription() { return ""; };

	bool isMoveTransformer(bool enemy) { return enemy; };
	bool tick(Pokemon* p) { return false; };

	int getTier() { return -1; };
	
	bool immobilises(Pokemon* p);

protected:
	MoveListEntry* getEnemyTransformedMove(Pokemon *p, MoveListEntry *move);
};
#endif