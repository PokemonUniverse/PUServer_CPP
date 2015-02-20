#include "pokemontype.h"

typeList_t PokemonType::m_typeList;

const std::string PokemonType::m_types[18] = {	
	"Normal", 
	"Fire",
	"Water",
	"Electric",
	"Grass",
	"Ice",
	"Fighting",
	"Poison",
	"Ground",
	"Flying",
	"Psychic",
	"Bug",
	"Rock",
	"Ghost",
	"Dragon",
	"Dark",
	"Steel",
	"Typeless"
};

const bool PokemonType::m_special[18] = {
	false,
	true,
	true,
	true,
	true,
	true,
	false,
	false,
	false,
	false,
	true,
	false,
	false,
	false,
	true,
	true,
	false,
	false
};

const double PokemonType::m_multiplier[18][18] = {
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.5, 0, 1, 1, 0.5, 1 },
	{ 1, 0.5, 0.5, 1, 2, 2, 1, 1, 1, 1, 1, 2, 0.5, 1, 0.5, 1, 2, 1 },
	{ 1, 2, 0.5, 1, 0.5, 1, 1, 1, 2, 1, 1, 1, 2, 1, 0.5, 1, 1, 1 },
	{ 1, 1, 2, 0.5, 0.5, 1, 1, 1, 0, 2, 1, 1, 1, 1, 0.5, 1, 1, 1 },
	{ 1, 0.5, 2, 1, 0.5, 1, 1, 0.5, 2, 0.5, 1, 0.5, 2, 1, 0.5, 1, 0.5, 1 },
	{ 1, 0.5, 0.5, 1, 2, 0.5, 1, 1, 2, 2, 1, 1, 1, 1, 2, 1, 0.5, 1 },
	{ 2, 1, 1, 1, 1, 2, 1, 0.5, 1, 0.5, 0.5, 0.5, 2, 0, 1, 2, 2, 1 },
	{ 1, 1, 1, 1, 2, 1, 1, 0.5, 0.5, 1, 1, 1, 0.5, 0.5, 1, 1, 0, 1 },
	{ 1, 2, 1, 2, 0.5, 1, 1, 2, 1, 0, 1, 0.5, 2, 1, 1, 1, 2, 1 },
	{ 1, 1, 1, 0.5, 2, 1, 2, 1, 1, 1, 1, 2, 0.5, 1, 1, 1, 0.5, 1 },
	{ 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 0.5, 1, 1, 1, 1, 0, 0.5, 1 },
	{ 1, 0.5, 1, 1, 2, 1, 0.5, 0.5, 1, 0.5, 2, 1, 1, 0.5, 1, 2, 0.5, 1 },
	{ 1, 2, 1, 1, 1, 2, 0.5, 1, 0.5, 2, 1, 2, 1, 1, 1, 1, 0.5, 1 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 0.5, 0.5, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 0.5, 1 },
	{ 1, 1, 1, 1, 1, 1, 0.5, 1, 1, 1, 2, 1, 1, 2, 1, 0.5, 0.5, 1 },
	{ 1, 0.5, 0.5, 0.5, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0.5, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

PokemonType* PokemonType::T_NORMAL = new PokemonType(0);
PokemonType* PokemonType::T_FIRE = new PokemonType(1);
PokemonType* PokemonType::T_WATER = new PokemonType(2);
PokemonType* PokemonType::T_ELECTRIC = new PokemonType(3);
PokemonType* PokemonType::T_GRASS = new PokemonType(4);
PokemonType* PokemonType::T_ICE = new PokemonType(5);
PokemonType* PokemonType::T_FIGHTING = new PokemonType(6);
PokemonType* PokemonType::T_POISON = new PokemonType(7);
PokemonType* PokemonType::T_GROUND = new PokemonType(8);
PokemonType* PokemonType::T_FLYING = new PokemonType(9);
PokemonType* PokemonType::T_PSYCHIC = new PokemonType(10);
PokemonType* PokemonType::T_BUG = new PokemonType(11);
PokemonType* PokemonType::T_ROCK = new PokemonType(12);
PokemonType* PokemonType::T_GHOST = new PokemonType(13);
PokemonType* PokemonType::T_DRAGON = new PokemonType(14);
PokemonType* PokemonType::T_DARK = new PokemonType(15);
PokemonType* PokemonType::T_STEEL = new PokemonType(16);
PokemonType* PokemonType::T_TYPELESS = new PokemonType(17);

PokemonType* PokemonType::getType(std::string type) 
{
	foreach(typeList_t::value_type pt, m_typeList)
	{
		if(PokemonType* pokeType = pt.second) {
			return pokeType;
		}
	}

	return NULL;
};