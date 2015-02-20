#ifndef INC_POKEMON_WRAPPER_H_
#define INC_POKEMON_WRAPPER_H_

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;
class BattleTurn;

class PokemonWrapper
{
private:
	Pokemon* m_poke;
	BattleTurn* m_turn;
	int m_idx;

	PokemonWrapper(Pokemon* p, BattleTurn *turn, int idx) : m_poke(p), m_turn(turn), m_idx(idx) {};

public:
	static bool compareSpeed(Pokemon *p1, Pokemon *p2)
	{
		const int s1 = p1->getStat(S_SPEED);
		const int s2 = p2->getStat(S_SPEED);

		int comp = 0;
		if(s1 > s2) { comp = -1; }
		else if(s2 > s1) { comp = 1; }
		
		if(comp != 0) {
			//TODO: Uncomment when SpeedSwapEffect has been created
			//if(p1->getField()->getEffectByType<SpeedSwapEffect>() != NULL) {
			//	comp = (comp == 1) ? -abs(comp) : abs(comp);
			//}
		}
		else {
			comp = ((random_range(1,100) % 2) == 0) ? -1 : 1;
		}
		
		return (comp < 0);
	};

	static bool compareStatuses(StatusEffect *e1, StatusEffect *e2) {
		int comp = e1->getTier() - e2->getTier();

		if(comp == 0) {
			comp = ((random_range(1,100) % 2) == 0) ? -1 : 1;
		}

		return (comp < 0);
	};

	static void sortMoves(pokemonList_t &pokemon, battleTurnList_t &moves)
	{
		battleTurnList_t list;
		std::sort(pokemon.begin(), pokemon.end(), compareSpeed);

		pokemonList_t::iterator it = pokemon.begin();
		for(; it != pokemon.end(); ++it)
		{
			Pokemon* p = (*it);
			if(p == NULL) continue;

			battleTurnList_t::iterator bit = moves.begin();
			foreach(battleTurnList_t::value_type bit, moves)
			{
				BattleTurn *turn = bit.second;
				if(turn->getOwner() == (*it)) {
					list.insert(std::make_pair<uint32_t, BattleTurn*>(turn->getOwner()->getMaster()->getGUID(), turn) );
				}
			}
		}

		moves = list;
	};

};
#endif