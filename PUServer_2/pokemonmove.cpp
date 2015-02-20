#include "pokemonmove.h"

#include "battlemechanics.h"
#include "pokemon.h"
#include "pokemonspecies.h"

bool PokemonMove::isSpecial(BattleMechanics *mech) {
	return mech->isMoveSpecial(this);
}

bool PokemonMove::attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
	return mech->attemptHit(this, attacker, defender); 
}

uint32_t PokemonMove::use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
	return mech->calculateDamage(this, attacker, defender);
};

double PokemonMove::getEffectiveness( PokemonType* type, Pokemon* attacker, Pokemon* defender )
{
	pokemonTypeList_t defTypes = defender->getTypes();
	double multiplier = 1.0;
	for(int i = 0; i < defTypes.size(); ++i)
	{
		PokemonType* def = defTypes.at(i);
		if(def == NULL)
			continue;

		double expected = type->getMultiplier(def);
		double factor;

		if(attacker != NULL) {
			factor = attacker->getEffectiveness(type, def, false);
			if(factor == expected) {
				factor = defender->getEffectiveness(type, def, true);
			}
		}
		else {
			BattleField* field = defender->getField();
			factor = field->getEffectiveness(type, def, false);
			if(factor == expected) {
				factor = field->getEffectiveness(type, def, true);
			}
		}

		multiplier *= factor;
	}

	return multiplier;
}