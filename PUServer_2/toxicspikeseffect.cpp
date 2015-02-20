#include "toxicspikeseffect.h"

#include "battlefield.h"
#include "pokemon.h"
#include "pokemonmove.h"
#include "pokemontype.h"
#include "poisoneffect.h"
#include "toxiceffect.h"

void ToxicSpikesEffect::switchIn( Pokemon *p )
{
	BattleField* field = p->getField();

	if((PokemonMove::getEffectiveness(PokemonType::T_GROUND, NULL, p) == 0.0)
		|| p->isType(PokemonType::T_STEEL)
		|| p->hasAbility("Levitate")
		|| (getLayers(p) <= 0)) {
			return;
	}

	if(p->isType(PokemonType::T_POISON)) {
		field->showMessage(boost::str(boost::format("%s absorbed the Toxic Spikes") % p->getName() ) );
		removeSpikes(p);

		return;
	}

	int layers = getLayers(p);
	if(layers == 1) {
		if(p->addStatus(NULL, new PoisonEffect()) != NULL) {
			field->showMessage(boost::str(boost::format("%s was poisoned by the Toxic Spikes!") % p->getName()));
		}
	}
	else if(layers == 2) {
		if(p->addStatus(NULL, new ToxicEffect()) != NULL) {
			field->showMessage(boost::str(boost::format("%s was badly poisoned by the Toxic Spikes!") % p->getName()));
		}
	}
}