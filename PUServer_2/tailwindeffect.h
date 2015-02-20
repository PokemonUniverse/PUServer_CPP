#ifndef INC_TAILWINDEFFECT_H_
#define INC_TAILWINDEFFECT_H_

#include "definitions.h"
#include "fieldeffect.h"

class Pokemon;
class BattleField;

class TailwindEffect : public FieldEffect
{
	teamInfo_s m_party;
	int m_turns;

public:
	TailwindEffect(teamInfo_s party) : m_turns(3), m_party(party) {};

	std::string getName() { return "Tailwind"; };
	std::string getDescription() { return ""; };
	teamInfo_s getParty() { return m_party; };

	int getTier() { return 4; } //TODO: No idea about this tier, just a guess.
	bool isSingleton() { return true; };

	bool equals(FieldEffect* eff);
	bool applyToField(BattleField* field);
	bool tickField(BattleField* field);
	bool apply(Pokemon* p);
	void unapply(Pokemon* p);
};
#endif