#ifndef __POKEMONTYPE_H_
#define __POKEMONTYPE_H_

#include <boost/unordered_map.hpp>

#include "definitions.h"

class PokemonType;
typedef boost::unordered_map<uint32_t, PokemonType*> typeList_t;

class PokemonType
{
public:
	PokemonType();

	//Constants representing each of the types.
	static PokemonType* T_NORMAL;
	static PokemonType* T_FIRE;
	static PokemonType* T_WATER;
	static PokemonType* T_ELECTRIC;
	static PokemonType* T_GRASS;
	static PokemonType* T_ICE;
	static PokemonType* T_FIGHTING;
	static PokemonType* T_POISON;
	static PokemonType* T_GROUND;
	static PokemonType* T_FLYING;
	static PokemonType* T_PSYCHIC;
	static PokemonType* T_BUG;
	static PokemonType* T_ROCK;
	static PokemonType* T_GHOST;
	static PokemonType* T_DRAGON;
	static PokemonType* T_DARK;
	static PokemonType* T_STEEL;
	static PokemonType* T_TYPELESS;

	static typeList_t getTypes() {
		return m_typeList;
	};

	static PokemonType* getType(int i) {
		typeList_t::iterator it = m_typeList.find(i);
		return (it != m_typeList.end()) ? it->second : NULL;
	};

	static PokemonType* getType(std::string type);

	uint32_t getTypeId() { return m_type; };
	bool isSpecial() { return m_special[m_type]; };
	double getMultiplier(PokemonType* defenderType) { return m_multiplier[m_type][defenderType->getTypeId()]; };

	bool equals(PokemonType* type) {
		if(!type || type == NULL) { return false; }
		return (type->getTypeId() == m_type);
	};

	const std::string toString() { return m_types[m_type]; };

private:
	PokemonType(int i) {
		m_type = i;
		m_typeList.insert(std::make_pair<int, PokemonType*>(i, this));
	};

	uint32_t m_type;
	static typeList_t m_typeList;

	static const std::string m_types[18];
	static const bool m_special[18];
	static const double m_multiplier[18][18];
};

const double m_multiplier[18][18] = {
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

#endif