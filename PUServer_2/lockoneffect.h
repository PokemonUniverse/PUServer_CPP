#ifndef INC_LOCKONEFFECT_H_
#define INC_LOCKONEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class LockOnEffect : public StatusEffect
{
	int m_turns;

public:
	LockOnEffect() {};

	std::string getName() { return "Locked-on"; };
	std::string getDescription() { return "took aim at the foe!"; };

	int getTier() { return 0; };
	bool isMoveTransformer(bool enemy) { return !enemy; };
	MoveListEntry* getEnemyTransformedMove(Pokemon *p, MoveListEntry *move) {
		if(move->getMove()->isAttack()) {
			p->removeStatus(this);
		}

		return move;
	};
};
#endif