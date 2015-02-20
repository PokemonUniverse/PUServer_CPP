#ifndef __CRITICALTYPEABILITY_H_
#define __CRITICALTYPEABILITY_H_

#include "definitions.h"
#include "intrinsicability.h"

class Pokemon;
class PokemonType;
class MoveListEntry;

class CriticalTypeAbility : public IntrinsicAbility
{
public:
	CriticalTypeAbility(std::string name, PokemonType* type) : IntrinsicAbility(name), m_type(type) {};

    bool isMoveTransformer(bool enemy) {
        return !enemy;
    }
    
    /**
     * If a pokemon has 1/3 or less health then CriticalType transforms moves
     * of a certain type by giving them one and a half times the power.
     */
    MoveListEntry* getTransformedMove(Pokemon* p, MoveListEntry* entry);

private:
	PokemonType* m_type;
};

#endif