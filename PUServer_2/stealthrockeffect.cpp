#include "stealthrockeffect.h"

#include "battlefield.h"
#include "pokemon.h"
#include "pokemonmove.h"
#include "pokemontype.h"

void StealthRockEffect::switchIn( Pokemon *p )
{
	if(getLayers(p) <= 0) return;

	double effectiveness = PokemonMove::getEffectiveness(PokemonType::T_ROCK, NULL, p);
	double baseDamage = p->getStat(S_HP) / 8.0;
	int damage = (int)(baseDamage * effectiveness);
	if(damage < 1) damage = 1;

	std::string message = boost::str(boost::format("Pointed stones dug into %s") % p->getName());
	p->getField()->showMessage(message);
	p->changeHealth(-damage, true);
}