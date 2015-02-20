#include "pokemonspecies.h"

PokemonSpeciesData* PokemonSpecies::m_default;

bool PokemonSpecies::canUseAbility(PokemonSpeciesData* data, std::string ability) {
	return data->canUseAbility(m_name, ability);
}

bool PokemonSpecies::canLearn(PokemonSpeciesData* data, std::string move) {
	return data->canLearn(m_species, move);
}

std::set<std::string> PokemonSpecies::getPossibleAbilities(PokemonSpeciesData* data) {
	return data->getPossibleAbilities(m_name);
}

std::set<std::string> PokemonSpecies::getLearnableMoves(PokemonSpeciesData* data) {
	return data->getLearnableMoves(m_species);
}

PokemonSpeciesData* PokemonSpecies::getDefaultData() { 
	return m_default; 
}