#ifndef INC_MEFIRSTEFFECT_H_
#define INC_MEFIRSTEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

#include "pokemon.h"

class MoveListEntry;

class MeFirstEffect : public StatusEffect
{
	MoveListEntry* m_move;

public:
	MeFirstEffect(MoveListEntry* move) { m_move = move; };

	bool isMoveTransformer(bool enemy) { return !enemy; };

	MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move) {
		p->getField()->informUseMove(p, "Me Frist");
		return m_move;
	};

	bool tick(Pokemon* p) {
		p->removeStatus(this);
		return true;
	};
	
	int getTier() { return 1; ;}

	std::string getName() { return ""; };
	std::string getDescription() { return ""; };
};

#endif