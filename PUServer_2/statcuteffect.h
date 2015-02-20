#ifndef INC_STATCUTEFFECT_H_
#define INC_STATCUTEFFECT_H_

#include "definitions.h"
#include "fieldeffect.h"

class Pokemon;
class BattleField;
class MoveListEntry;
class StatusEffect;

class StatCutEffect : public FieldEffect
{
	uint32_t m_stat;
	uint32_t m_turns;
	std::string m_name;
	
	teamInfo_s m_party;

public:
	StatCutEffect(int stat, teamInfo_s party, std::string name) : m_stat(stat), m_party(party), m_name(name) {};

	std::string getName() { return m_name; };
	std::string getDescription() { return ""; };

	teamInfo_s getParty() { return m_party; };
	int getStat() { return m_stat; };

	int getTier() { return 0; };
	bool isSingleton() { return false; };
	bool isMoveTransformer(bool enemy) { return enemy; };

	bool equals(StatusEffect* eff);
	bool applyToField(BattleField* field);
	bool tickField(BattleField* field);

protected:
	MoveListEntry* getEnemyTransformedMove(Pokemon *p, MoveListEntry *move);
};
#endif