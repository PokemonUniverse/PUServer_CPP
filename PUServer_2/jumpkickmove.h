#ifndef INC_JUMPKICKMOVE_H_
#define INC_JUMPKICKMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"
#include "statuseffect.h"
#include "pokemon.h"

class BattleMechanics;
class PokemonType;
class Pokemon;

class JumpKickCalculationEffect : public StatusEffect
{
public:
	bool isEffectivenessTransformer(bool enemy) {
		return !enemy;
	};

	double getTransformedEffectiveness(PokemonType *move, PokemonType *pokemon) {
		double expected  = StatusEffect::getEnemyTransformedEffectiveness(move, pokemon);
		if(expected == 0.0)
			return 1.0;

		return expected;
	};
};

class JumpKickMove : public PokemonMove
{
	int m_jewelPower;
	int m_power;

public:
	JumpKickMove(PokemonType* type, uint32_t power, uint32_t jewelPower, double accuracy, uint32_t pp)
		: PokemonMove(type, power, accuracy, pp), m_jewelPower(jewelPower), m_power(power) {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);
};

#endif