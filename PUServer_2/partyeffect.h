#ifndef INC_PARTYEFFECT_H_
#define INC_PARTYEFFECT_H_

#include "definitions.h"
#include "fieldeffect.h"

class BattleField;
class Pokemon;

class PartyEffect : public FieldEffect
{
	int m_turns[2];
	bool m_active[2];
	std::string m_startMessage;
	std::string m_endMessage;

public:
	PartyEffect(std::string start, std::string end) : m_startMessage(start), m_endMessage(end) {
		m_turns[0] = 5;
		m_active[0] = false;

		m_turns[1] = 5;
		m_active[1] = false;
	};

	bool applyToField(BattleField* field) { return true; };
	bool isSingleton() { return true; };
	
	int getTier() { return 0; };

	bool tickField(BattleField* field);
	void activateParty(Pokemon* p);

	bool isActive(uint8_t party) {
		if(party < 0 || party > 1) return false;
		return m_active[party];
	};
};
#endif