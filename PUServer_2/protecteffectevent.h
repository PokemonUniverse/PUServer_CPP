#ifndef INC_PROTECTEFFECTEVENT_H_
#define INC_PROTECTEFFECTEVENT_H_

#include "definitions.h"

class PokemonMove;
class BattleMechanics;
class Pokemon;

class ProtectEffectEvent
{
public:
	virtual uint32_t use(PokemonMove* move, BattleMechanics* mech, Pokemon* source, Pokemon* target) { return 0; };
};
#endif