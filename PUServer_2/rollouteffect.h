#ifndef INC_ROLLOUTEFFECT_H_
#define INC_ROLLOUTEFFECT_H_

#include "definitions.h"
#include "fixedattackeffect.h"

#include "movelistentry.h"

class RolloutEffect : public FixedAttackEffect
{
public:
	RolloutEffect(std::string name) : FixedAttackEffect(name, "", "") {};

	bool apply(Pokemon* p) {
		FixedAttackEffect::apply(p);
		m_turns = 5;
		
		return true;
	};

	bool isMoveTransformer(bool enemy) { return !enemy; };

	MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move) {
		move->getMove()->setPower(move->getMove()->getPower() * (1 << (5 - m_turns) ) );
		return move;
	};
};

#endif