#ifndef INC_DAMAGELISTENEREFFECT_H_
#define INC_DAMAGELISTENEREFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;
class MoveListEntry;

/**
* Records the amount of damage done to a Pokemon in a turn. Should be applied in
* a beginTurn event.
*/
class DamageListenerEffect : public StatusEffect
{
	int m_damage;
	bool m_special;

public:
	DamageListenerEffect() {
		m_damage = 0;
		m_special = true;
	};

	std::string getName() { return ""; };
	std::string getDescription() { return ""; };

	int getTier() { return 1; };

	bool tick(Pokemon* p);
	bool isListener() { return true; };

	void informDamaged(Pokemon *attacker, Pokemon *target, MoveListEntry *move, uint32_t damage);

	uint32_t getDamage() { return m_damage; };
	bool isSpecial() { return m_special; };
};

#endif