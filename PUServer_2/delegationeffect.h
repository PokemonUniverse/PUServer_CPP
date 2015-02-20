#ifndef INC_DELEGATIONEFFECT_H_
#define INC_DELEGATIONEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"
#include "pokemonmove.h"
#include "pokemontype.h"

#include "protecteffectevent.h"

class RainEffect;

class DelegationEffect : public StatusEffect
{
protected:
	ProtectEffectEvent* m_event;

private:
	class ReplaceMove : public PokemonMove
	{
		PokemonMove* m_move;
		ProtectEffectEvent* m_event;
	public:
		ReplaceMove(PokemonMove* move, ProtectEffectEvent* effevent) : PokemonMove(PokemonType::T_TYPELESS, 0, move->getAccuracy(), 1), m_move(move), m_event(effevent) {};

		uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
			return m_event->use(m_move, mech, attacker, defender);
		};
	};

public:
	std::string getName() { return ""; };
	
	int getTier() { return -1; };

	bool isMoveTransformer(bool enemy) { return enemy; };
	bool tick(Pokemon* p);

	MoveListEntry* getEnemyTransformedMove(Pokemon *p, MoveListEntry *move);
};
#endif