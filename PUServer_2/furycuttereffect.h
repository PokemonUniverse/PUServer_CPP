#ifndef INC_FURYCUTTEREFFECT_H_
#define INC_FURYCUTTEREFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

#include "pokemon.h"
#include "pokemonmove.h"
#include "movelistentry.h"
#include "jewelmechanics.h"

class FuryCutterEffect : public StatusEffect
{
	int m_turns;

public:
	FuryCutterEffect() {};

	std::string getName() { return "Fury cutter"; };
	std::string getDescription() { return ""; };

	int getTier() { return -1; };

	bool tick(Pokemon* p) {
		m_turns++;
		return false;
	};

	void executeTurn(Pokemon *p, BattleTurn *turn) {
		if(isAssignableFrom<JewelMechanics*>(p->getField()->getMechanics())) return;

		if(p->getMove(turn->getId())->getName() != "Fury Cutter") {
			p->removeStatus(this);
		}
	};

	bool isMoveTransformer(bool enemy) { return !enemy; };

	MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move)
	{
		if(move->getName() != "Fury Cutter") return move;

		PokemonMove *pmove = move->getMove();
		int power = pmove->getPower() * (1 << m_turns);
		if(power > 160) power = 160;
		pmove->setPower(power);
		
		return move;
	};

	bool isSingleton() { return true; };

	void informDuplicateEffect(Pokemon *p) {};
};
#endif