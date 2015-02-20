#include "pokemonspeciesdata.h"

speciesDatabase_t PokemonSpeciesData::m_database;

PokemonSpecies* PokemonSpeciesData::getPokemonByName(std::string name)	
{
	PokemonSpecies *tmp = NULL;
	foreach(speciesDatabase_t::value_type it, m_database) 
	{
		if((tmp = it.second) && tmp->getName() == name) {
			return tmp;
		}
	}

	return NULL;
}