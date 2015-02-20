#ifndef INC_CONVERSIONEFFECT_H_
#define INC_CONVERSIONEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

#include "pokemontype.h"
#include "pokemonspecies.h"

class Pokemon;

class ConversionEffect : public StatusEffect
{
	pokemonTypeList_t m_types;

public:
	ConversionEffect() {};

	std::string getName() { return "Conversion"; };
	std::string getDescription() { return ""; };

	int getTier() { return -1; };

	bool tick(Pokemon* p) { return false; };

	bool apply(Pokemon* p);
	void unapply(Pokemon* p);
	bool switchOut(Pokemon* p);
};

class Conversion2Effect : public StatusEffect
{
	pokemonTypeList_t m_types;

public:
	Conversion2Effect() {};

	std::string getName() { return "Conversion 2"; };
	std::string getDescription() { return ""; };

	int getTier() { return -1; };

	bool tick(Pokemon* p) { return false; };

	bool apply(Pokemon* p);
	void unapply(Pokemon* p);
	bool switchOut(Pokemon* p);
};
#endif