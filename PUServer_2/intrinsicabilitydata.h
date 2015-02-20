#ifndef __INTRINSICABILITYDATA_H_
#define __INTRINSICABILITYDATA_H_

#include "intrinsicability.h"
#include "pokemon.h"
#include "movelistentry.h"

class IA_ColorChange : IntrinsicAbility
{
public:
	IA_ColorChange() : IntrinsicAbility("Color Change") {};

	bool isListener() { return true; };

	void informDamaged(Pokemon *attacker, Pokemon *target, MoveListEntry *move, uint32_t damage)
	{
		PokemonType *type = move->getMove()->getType();
		if(!target->isType(type)) {
			pokemonTypeList_t newList;
			newList.push_back(type);
			target->setType(newList);
			
			//TODO: Message "target became the xxx type!"
		}
	};

	void switchIn(Pokemon *p) {
		pokemonTypeList_t newList;
		newList.push_back(PokemonType::T_NORMAL);
		p->setType(newList);
	};
};

class IA_Drought : IntrinsicAbility
{
public:
	IA_Drought() : IntrinsicAbility("Drought") {};

	void switchIn(Pokemon *p) {
		//p.getField().showMessage(p.getName() + "'s Drought intensified the sun's rays!");
		//p.getField().applyEffect(new SunEffect(0));
	}
};

class IA_Drizzle : IntrinsicAbility
{
public:
	IA_Drizzle() : IntrinsicAbility("Drought") {};

	void switchIn(Pokemon *p) {
		//p.getField().showMessage(p.getName() + "'s Drizzle caused a storm!");
		//p.getField().applyEffect(new RainEffect(0));
	}
};
#endif