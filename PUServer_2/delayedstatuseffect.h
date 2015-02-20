#ifndef INC_DELAYEDSTATUSEFFECT_H_
#define INC_DELAYEDSTATUSEFFECT_H_

#include "definitions.h"
#include "fieldeffect.h"

class BattleField;
class StatusEffect;

class DelayedStatusEffect : public FieldEffect
{
	StatusEffect* m_effect;
	std::string m_message;

protected:
	teamInfo_s m_party;
	uint32_t m_turns;

public:
	DelayedStatusEffect(StatusEffect* effect, teamInfo_s party, uint32_t turns, std::string message)
		: m_effect(effect), m_party(party), m_turns(turns), m_message(message) {};

	virtual bool isSingleton() { return false; };
	virtual std::string getName() { return ""; };
	virtual std::string getDescription() { return ""; };
	virtual int getTier() { return 1; };

	virtual bool applyToField(BattleField* field);
	virtual bool tickField(BattleField* field);
};
#endif