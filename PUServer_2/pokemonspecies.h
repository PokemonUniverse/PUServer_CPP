#ifndef __POKEMONSPECIES_H_
#define __POKEMONSPECIES_H_

#include <set>

#include "definitions.h"
#include "creature.h"
#include "pokemontype.h"
#include "pokemonspeciesdata.h"

class PokemonSpeciesData;

typedef std::vector<PokemonType*> pokemonTypeList_t;
typedef std::vector<int> baseList_t;

class PokemonSpecies : public Creature
{
public:
	PokemonSpecies(int species, std::string name, baseList_t base, pokemonTypeList_t type, int gender) {
		m_species	= species;
		m_name		= name;
		m_base		= base;
		m_type		= type;
		m_genders	= gender;
		m_mass		= 0.0;
	};
	~PokemonSpecies() {};

	virtual PokemonSpecies* getPokemonSpecies() { return this; };
	virtual const PokemonSpecies* getPokemonSpecies() const { return this; };

	static PokemonSpeciesData* getDefaultData();

	std::string getSpeciesName() { return m_name; };

	int getPossibleGenders() { return m_genders; };
	void setPossibleGenders(int genders) { m_genders = genders; };
	void setDefaultData(PokemonSpeciesData* data) { m_default = data; };

	//************************************
	// Method:    getBalancedLevel
	// FullName:  PokemonSpecies::getBalancedLevel
	// Access:    public 
	// Returns:   int
	// Qualifier: Get a "balanced" level for this species using this formula: level = 113 - 0.074 * [base stat total]
	//************************************
	int getBalancedLevel();

	bool canUseAbility(PokemonSpeciesData* data, std::string ability);
	bool canLearn(PokemonSpeciesData* data, std::string move);

	std::set<std::string> getPossibleAbilities(PokemonSpeciesData* data);
	std::set<std::string> getLearnableMoves(PokemonSpeciesData* data);
	
	//TODO: MoveSet getMoveSet(MoveSetData data);

	baseList_t getBaseStats() { return m_base; };
	int getBase(int i) {
		if(i < 0 || i > 5) { return 0; }
		return m_base[i];
	};

	pokemonTypeList_t getTypes() { return m_type; };
	void setType(pokemonTypeList_t type) { m_type = type; };

	double getMass() { return m_mass; };
	void setMass(double mass) { m_mass = mass; };

protected:
	PokemonSpecies() {};

	uint32_t m_species;
	uint32_t m_genders;
	
	double m_mass;

	pokemonTypeList_t m_type;
	baseList_t m_base;

	static PokemonSpeciesData* m_default;
};

#endif