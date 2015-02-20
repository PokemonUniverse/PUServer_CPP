#ifndef INC_PROTECTEFFECT_H_
#define INC_PROTECTEFFECT_H_

#include "definitions.h"
#include "delegationeffect.h"
#include "protecteffectevent.h"

#include "pokemon.h"

class BattleMechanics;
class PokemonMove;

class ProtectEffect : public DelegationEffect
{
	class MyEvent : public ProtectEffectEvent
	{
		std::string m_message;
	public:
		MyEvent(std::string message) : ProtectEffectEvent(), m_message(message) {};
		uint32_t use(PokemonMove* move, BattleMechanics* mech, Pokemon* source, Pokemon* target) {
			source->getField()->showMessage(m_message);
			return 0;
		}
	};
public:
	ProtectEffect() {
		m_event = new MyEvent(getDescription());
	};
};
#endif