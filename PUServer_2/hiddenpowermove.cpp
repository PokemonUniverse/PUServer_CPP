#include "hiddenpowermove.h"

#include "pokemon.h"
#include "pokemontype.h"

void HiddenPowerMove::switchIn( Pokemon *p )
{
	int power = 0;
	int type = 0;

	for(int i = 0; i < 6; ++i)
	{
		int iv = p->getIv(i);
		int increment = 1 << i;
		if(iv % 2 != 0) {
			type += increment;
		}

		if((iv % 4 == 2) || (iv % 4 == 3)) {
			power += increment;
		}
	}

	power = (int)((((double)power * 40.0) / 63.0) + 30.0);
	setPower(power);
	type = (int)((double)type * (15.0/63.0));

	PokemonType* moveType[] = {
		PokemonType::T_FIGHTING,
		PokemonType::T_FLYING,
		PokemonType::T_POISON,
		PokemonType::T_GROUND,
		PokemonType::T_ROCK,
		PokemonType::T_BUG,
		PokemonType::T_GHOST,
		PokemonType::T_STEEL,
		PokemonType::T_FIRE,
		PokemonType::T_WATER,
		PokemonType::T_GRASS,
		PokemonType::T_ELECTRIC,
		PokemonType::T_PSYCHIC,
		PokemonType::T_ICE,
		PokemonType::T_DRAGON,
		PokemonType::T_DARK
	};

	setType(moveType[type]);
}