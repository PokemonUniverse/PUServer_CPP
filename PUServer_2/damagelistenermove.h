#ifndef INC_DAMAGELISTENERMOVE_H_
#define INC_DAMAGELISTENERMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"

class Pokemon;
class PokemonType;
class DamageListenerEffect; 

class DamageListenerMove : public PokemonMove
{
	int m_priority;

public:
	DamageListenerMove(PokemonType* type, uint32_t power, double accuracy, uint32_t pp)
		: PokemonMove(type, power, accuracy, pp), m_priority(0) {};

	void beginTurn(battleTurnList_t turn, int index, Pokemon *source);

	DamageListenerEffect* getListener(Pokemon* p);

	int getPriority() { return m_priority; };
	void setPriority(int priority) { m_priority = priority; };

};

#endif