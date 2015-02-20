#ifndef INC_CONFUSEEFFECT_H_
#define INC_CONFUSEEFFECT_H_

#include <boost/format.hpp>

#include "definitions.h"
#include "statuseffect.h"
#include "pokemonmove.h"

class ConfuseEffect : public StatusEffect
{
public:
	ConfuseEffect() {};
	~ConfuseEffect() {};

	std::string getName() { return "Confusion"; };
	std::string getDescription() { return " became confused!"; };

	bool tick(Pokemon* p) { return false; };
	int getTier() { return 1; };
	bool switchOut(Pokemon* p) { return true; };
	bool apply(Pokemon* p);
	void unapply(Pokemon* p);
	bool immobilises(Pokemon* p);

private:
	int m_turns;
};

class ConfuseEffectMove : public PokemonMove
{
public:
	ConfuseEffectMove(PokemonType *type, uint32_t power, double accuracy, uint32_t pp) 
		: PokemonMove(type, power, accuracy, pp) {};
	~ConfuseEffectMove() {};

	uint32_t use(BattleMechanics* mech, Pokemon* source, Pokemon* target);

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		return true;
	};

	bool canCriticalHit() { return false; };
};

#endif
