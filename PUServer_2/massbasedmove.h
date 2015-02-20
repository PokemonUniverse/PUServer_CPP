#ifndef INC_MASEBASEDMOVE_H_
#define INC_MASEBASEDMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"

class MassBasedMove : public PokemonMove
{
public:
	MassBasedMove(PokemonType* type, double accuracy, uint32_t pp)
		: PokemonMove(type, 0, accuracy, pp) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		double mass = defender->getMass();
		if(mass <= 10.0) {
			setPower(20);
		} else if(mass <= 25.0) {
			setPower(40);
		} else if(mass <= 100.0) {
			setPower(80);
		} else if(mass <= 200.0) {
			setPower(100);
		} else {
			setPower(120);
		}

		int damage = PokemonMove::use(mech, attacker, defender);
		setPower(0);

		return damage;
	}

	bool isAttack() { return true; };
};
#endif