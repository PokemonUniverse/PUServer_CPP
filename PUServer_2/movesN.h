#ifndef INC_MOVESN_H_
#define INC_MOVESN_H_

#include "definitions.h"
#include "statusmove.h"
#include "pokemontype.h"
#include "pokemonmove.h"
#include "pokemon.h"

#include "flincheffect.h"
#include "nightmareeffect.h"
#include "jewelmechanics.h"

struct NastyPlot : public StatusMove
{
	NastyPlot() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPATTACK, true, 2));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_DARK, 0, 1.0, 20, a1, a2, a3);
	};
};

struct NaturePower : public PokemonMove
{
	NaturePower() : PokemonMove(PokemonType::T_NORMAL, 0, 0.95, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		//TODO: This should be based on tarrain
		if(isAssignableFrom<JewelMechanics*>(mech)) {
			return attacker->useMove(MoveList::getDefaultData()->getMove("Swift"), defender);
		} else {
			return attacker->useMove(MoveList::getDefaultData()->getMove("Tri Attack"), defender);
		}
	}

	bool isAttack() { return false; };
};

struct NeedleArm : public StatusMove
{
	NeedleArm() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_GRASS, 60, 1.0, 15, a1, a2, a3);
	};
};

struct Nightmare : public StatusMove
{
	Nightmare() {
		statusEffectList_t a1;
		a1.push_back(new NightmareEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GRASS, 60, 1.0, 15, a1, a2, a3);
	};
};

struct NightShade : public PokemonMove
{
	NightShade() : PokemonMove(PokemonType::T_GHOST, 0, 1.0, 15) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		if(getEffectiveness(attacker, defender) == 0.0) {
			std::string message = boost::str(boost::format("It doesn't affect %s...") % defender->getName());
			return 0;
		}

		int damage = attacker->getLevel();
		defender->changeHealth(-damage);
		return damage;
	};
};
#endif