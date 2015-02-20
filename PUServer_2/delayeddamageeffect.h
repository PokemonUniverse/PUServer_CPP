#ifndef INC_DELAYEDDAMGEEFFECT_H_
#define INC_DELAYEDDAMGEEFFECT_H_

#include "definitions.h"
#include "delayedstatuseffect.h"

class BattleField;

class DelayedDamageEffect : public DelayedStatusEffect
{
	int m_damage;

public:
	DelayedDamageEffect(int damage, teamInfo_s party, uint32_t turns)
		: DelayedStatusEffect(NULL, party, turns, "foresaw an attack!"), m_damage(damage) {};

	int getTier() { return 4; };
	bool tickField(BattleField* field);
};

#endif