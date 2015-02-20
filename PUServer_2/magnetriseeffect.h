#ifndef INC_MAGNETRISEEFFECT_H_
#define INC_MAGNETRISEEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;
class PokemonType;

class MagnetRiseEffect : public StatusEffect
{
	int m_turns;

public:
	MagnetRiseEffect() : m_turns(5) {};

	std::string getName() { return "Magnet Rise"; };
	std::string getDescription() { return "became immune to ground attacks!"; };

	bool tick(Pokemon* p);
	double getEnemyTransformedEffectiveness(PokemonType *move, PokemonType *pokemon);

	int getTier() { return 1; };
	bool isEffectivenessTransformer(bool enemy) { return enemy; };
};
#endif