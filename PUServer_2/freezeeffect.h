#ifndef INC_FREEZEEFFECT_H_
#define INC_FREEZEEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;
class BattleTurn;
class MoveListEntry;

class FreezeEffect : public StatusEffect
{
public:
	FreezeEffect() {
		m_lock = SPECIAL_EFFECT_LOCK;
	};
	~FreezeEffect() {};

	std::string getName() { return "Freeze"; };
	std::string getDescription() { return "was frozen solid!"; };

	bool tick(Pokemon* p) { return false; };

	int getTier() { return -1; };

	bool switchOut(Pokemon* p) { return false; };
	bool apply(Pokemon* p);
	void unapply(Pokemon* p);

	bool isListener() { return true; };

	void executeTurn(Pokemon *p, BattleTurn *turn);
	void informDamaged(Pokemon *attacker, Pokemon *target, MoveListEntry *move, uint32_t damage);

	bool immobilises(Pokemon* p);
};
#endif