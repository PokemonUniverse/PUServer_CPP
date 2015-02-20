#ifndef INC_FIXEDATTACKEFFECT_H_
#define INC_FIXEDATTACKEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class StatusEffect;
class Pokemon;
class MoveListEntry;

/**
* A status that forces the user to continue to use the same move.
*/
class FixedAttackEffect : public StatusEffect 
{
protected:
	int m_turns;

private:
	int m_pp;
	std::string m_name;
	std::string m_description;
	std::string m_message;

	int getMoveIndex(Pokemon *p);

public:
	FixedAttackEffect(std::string name, std::string description, std::string message) {
		m_name = name;
		m_description = description;            
		m_message = message;
	};

	std::string getName() { return m_name; };

	std::string getDescription() { return m_description; };

	int getTier() {	return 3; };

	bool apply(Pokemon* p);

	bool tick(Pokemon* p);

	bool canSwitch(Pokemon* p) { return false; };

	bool vetoesMove(Pokemon* p, MoveListEntry* entry);

	bool isSingleton() { return true; };

	void informDuplicateEffect(Pokemon* p) { };

	void executeTurn(Pokemon* p, BattleTurn* turn);
};

#endif