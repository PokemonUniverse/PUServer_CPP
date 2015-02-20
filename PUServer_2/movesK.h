#ifndef INC_MOVESK_K_
#define INC_MOVESK_K_

#include "definitions.h"
#include "statusmove.h"
#include "pokemontype.h"
#include "highcriticalhitmove.h"
#include "pokemonmove.h"

#include <boost/format.hpp>

struct KarateChop : public HighCriticalHitMove
{
	KarateChop() : HighCriticalHitMove(PokemonType::T_FIGHTING, 50, 1.0, 25) {};
};

struct Kinesis : public StatusMove
{
	Kinesis() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ACCURACY, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_PSYCHIC, 0, 0.8, 15, a1, a2, a3);
	};
};

struct KnockOff : public PokemonMove
{
	KnockOff() : PokemonMove(PokemonType::T_DARK, 20, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		if(defender->hasSubstitute()) {
			return PokemonMove::use(mech, attacker, defender);
		}

		/* TODO: Create when HoldItem is implemented
		HoldItem item = target.getItem();
				if ((item != null) && !(target.hasAbility("Sticky Hold"))) {
					target.setItem(null);
					user.getField().showMessage(user.getName() + " knocked off foe "
						+ target.getName() + "'s " + item.getName() + "!");
				} else*/
		if(defender->hasAbility("Sticky Hold")) {
			std::string message = boost::str(boost::format("%s held on with its Sticky Hold!") % defender->getName());
			attacker->getField()->showMessage(message);
		}

		return PokemonMove::use(mech, attacker, defender);
	};
};
#endif