#ifndef INC_STOCKPILEEFFECT_H_
#define INC_STOCKPILEEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"
#include "statusmove.h"

class StockpileEffect : public StatusEffect
{
	int m_levels;
	statusEffectList_t m_effects;

public:
	StockpileEffect() : m_levels(0) {};

	std::string getName() { return "Stockpile"; };
	std::string getDescription() { return ""; };
	int getTier() { return -1; };
	bool tick(Pokemon* p) { return false; };

	bool incrementLevel(Pokemon* p);
	void unapply(Pokemon* p);

	int getLevels() { return m_levels; };

	bool isSingleton() { return true; };
	bool isPassable() { return false; };
	void informDuplicateEffect(Pokemon *p) {};
};

#endif