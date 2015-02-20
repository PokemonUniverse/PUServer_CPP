#ifndef INC_STOCKPILEMOVE_H_
#define INC_STOCKPILEMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"

#include "stockpileeffect.h"

class StockpileMove : public PokemonMove
{
public:
	StockpileMove(PokemonType* type, uint32_t power, double accuracy, uint32_t pp)
		: PokemonMove(type, power, accuracy, pp) {};

	StockpileEffect* getStockpileEffect(Pokemon* p);
	int getLevels(Pokemon* p);
};

#endif