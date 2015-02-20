#ifndef INC_SNATCHEFFECT_H_
#define INC_SNATCHEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class SnatchEffect : public StatusEffect
{
	statusEffectList_t m_effects;

public:
	SnatchEffect(statusEffectList_t effects) : m_effects(effects) { };

	std::string getName() { return "Snatch"; };
	std::string getDescription() { return ""; };

	bool tick(Pokemon* p) { 
		p->removeStatus(this);
		return true;
	};

	bool isMoveTransformer(bool enemy) { return enemy; };
	MoveListEntry* getEnemyTransformedMove(Pokemon *p, MoveListEntry *move) { return NULL; };
	statusEffectList_t getEffects() { return m_effects; };
};
#endif