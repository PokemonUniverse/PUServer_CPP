#ifndef INC_UPDATEPOKEMONSTATUSMESSAGE_H_
#define INC_UPDATEPOKEMONSTATUSMESSAGE_H_

#include "battlemessage.h"

class UpdatePokemonStatusMessage : public BattleMessage
{
	std::string m_pokemon;
	std::string m_status;
	
	int m_state;

public:
	enum MessageStatus
	{
		STATUS_ACTIVE = 0,
		STATUS_AFFLICTED = 1,
		STATUS_FAINTED = 2
	};

	UpdatePokemonStatusMessage(std::string pokemon, MessageStatus state, std::string status) {
		m_pokemon = pokemon;
		m_state = state;
		m_status = status;
	};

	std::string getPokemon() { return m_pokemon; }
	std::string getStatus() { return m_status; };
	int getState() { return m_state; };

	std::string getMessage() { return ""; };

};
#endif