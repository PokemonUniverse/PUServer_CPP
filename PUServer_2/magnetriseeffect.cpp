#include "magnetriseeffect.h"

#include "pokemon.h"
#include "pokemontype.h"


bool MagnetRiseEffect::tick( Pokemon* p )
{
	if(--m_turns == 0) {
		p->removeStatus(this);
		std::string message = boost::str(boost::format("%s is no longer immune to ground attacks!") % p->getName());
		p->getField()->showMessage(message);

		return true;
	}

	return false;
}

double MagnetRiseEffect::getEnemyTransformedEffectiveness( PokemonType *move, PokemonType *pokemon )
{
	if(move->equals(PokemonType::T_GROUND)) {
		return 0.0;
	}

	return StatusEffect::getEnemyTransformedEffectiveness(move, pokemon);
}