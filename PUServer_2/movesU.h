#ifndef INC_MOVESU_H_
#define INC_MOVESU_H_

#include "definitions.h"
#include "pokemonmove.h"
#include "fixedattackmove.h"

#include "battlefield.h"
#include "battlemechanics.h"
#include "pokemon.h"
#include "pokemontype.h"
#include "fixedattackeffect.h"

struct Uproar : public FixedAttackMove 
{
	Uproar() : FixedAttackMove(PokemonType::T_NORMAL, 50, 1.0, 20, new FixedAttackEffect("Uproar", " caused an uproar", " calmed down.")) {};
};

struct UTurn : public PokemonMove
{
	UTurn() : PokemonMove(PokemonType::T_BUG, 70, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int damage = mech->calculateDamage(this, attacker, defender);

		BattleField* field = attacker->getField();
		if(field->getAliveCount(attacker->getMaster()) > 1)
		{
			field->requestAndWaitForSwitch(attacker->getMaster());
			Pokemon* target = field->getActivePokemon(attacker->getMaster());

			/*TODO: Uncomment when HoldItem and ChoiceBandItem are implemented
			HoldItem* item = target->getItem();
			if(isAssignableFrom<ChoiceBandItem*>(item))
			{
				for(int i = 0; i < 4; ++i)
				{
					MoveListEntry* entry = target->getMove(i);
					if(entry != NULL && entry->getName() == "U-turn") {
						ChoiceBandItem bandItem = dynamic_cast<ChoiceBandItem*>(item);
						bandItem.setChoice(target, mech, entry);
						break;
					}
				}
			}*/

		}

		return damage;
	}
};

#endif