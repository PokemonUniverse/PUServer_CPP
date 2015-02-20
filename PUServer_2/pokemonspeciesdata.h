#ifndef __POKEMONSPECIESDATA_H_
#define __POKEMONSPECIESDATA_H_

class PokemonSpecies;

#include <set>
#include <boost/unordered_map.hpp>

#include "pokemonspecies.h"

typedef boost::unordered_map<uint32_t, PokemonSpecies*> speciesDatabase_t;

class PokemonSpeciesData
{
public:
	static void loadSpeciesDatabase();
	//static void cacheMoveSets();

	static speciesDatabase_t getSpecies() {
		return m_database;
	};

	PokemonSpeciesData();
	~PokemonSpeciesData();

	// Add a Pokemons abilities to the map
	void setAbilities(std::string name, std::string abilities[], bool impl);

	// Returns whether an ability is implemented
	bool isAbilityImplemented(std::string ability) {
		return (m_unimplemented.find(ability) != m_unimplemented.end());
	};

	// Returns whether a pokemon can have a particular ability
	bool canUseAbility(std::string name, std::string ability) {
		return false;
	};

	std::vector<std::string> getAbilityNames(std::string name) {
		ablNamesList_t::iterator it = m_ablNames.find(name);
		if(it == m_ablNames.end()) {
			std::vector<std::string> dummy;
			return dummy;
		}

		return it->second;
	};

	std::set<std::string> getPossibleAbilities(std::string name) {
		abilitiesList_t::iterator it = m_abilities.find(name);
		if(it == m_abilities.end()) {
			std::set<std::string> dummy;
			return dummy;
		}
		return it->second;
	};

	PokemonSpecies* getSpecies(int i) {
		speciesDatabase_t::iterator it = m_database.find(i);
		return (it == m_database.end()) ? NULL : it->second;
	};

	int getSpeciesCount() { return m_database.size(); };

	PokemonSpecies* getPokemonByName(std::string name);

	std::set<std::string> getLearnableMoves(int i) {
		moveSets_t::iterator it = m_movesets.find(i);
		if(it == m_movesets.end()) {
			std::set<std::string> dummy;
			return dummy;
		}

		return it->second;
	};

	bool canLearn(int i, std::string move) {
		moveSets_t::iterator it = m_movesets.find(i);
		if(it == m_movesets.end()) { 
			return false;
		}
		std::set<std::string> set = it->second;
		std::set<std::string>::iterator setIt = set.find(move);
		if(setIt == set.end()) {
			return false;
		}
		return true;
	};
	
private:
	typedef boost::unordered_map<std::string, std::set<std::string> > abilitiesList_t;
	abilitiesList_t m_abilities;

	typedef boost::unordered_map<std::string, std::vector<std::string> > ablNamesList_t;
	ablNamesList_t m_ablNames;

	typedef std::set<std::string> unimplementedList_t;
	unimplementedList_t m_unimplemented;

	typedef boost::unordered_map<uint32_t, std::set<std::string> > moveSets_t;
	moveSets_t m_movesets;

	// Database of all species
	static speciesDatabase_t m_database;
};

#endif