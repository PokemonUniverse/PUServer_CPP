#ifndef INC_RAMPAGEMOVE_H_
#define INC_RAMPAGEMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"
#include "statuseffect.h"

#include "movelistentry.h"

class Pokemon;
class PokemonType;

class RampageMove : public PokemonMove
{
	class RampageStatusEffect : public StatusEffect
	{
		int m_turns;
		PokemonMove* m_move;

	public:
		RampageStatusEffect(PokemonMove* move) : m_move(move) {};

		bool apply(Pokemon* p);

		std::string getName() { return m_move->getMoveListEntry()->getName(); };
		std::string getDescription() { return "went on a rampage!"; };

		int getTier() { return 1; };

		bool canSwitch(Pokemon* p) { return false; };
		bool vetoesMove(Pokemon *p, MoveListEntry *entry) { return !m_move->getMoveListEntry()->equals(entry); };

		bool tick(Pokemon* p);
	};
public:
	RampageMove(PokemonType *type, uint32_t power, double accuracy, uint32_t pp) 
		: PokemonMove(type, power, accuracy, pp) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);
};
#endif