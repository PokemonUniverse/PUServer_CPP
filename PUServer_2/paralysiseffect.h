#ifndef INC_PARALYSISEFFECT_H_
#define INC_PARALYSISEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;

class ParalysisEffect : public StatusEffect
{
public:
	ParalysisEffect() {
		m_lock = SPECIAL_EFFECT_LOCK; 
	};
	~ParalysisEffect() {};

	std::string getName() { return "Paralysis"; };
	std::string getDescription() { return "is paralysed! It may be unable to move!"; };

	bool switchOut(Pokemon* p) { return false; };
	bool tick(Pokemon* p) { return false; };

	int getTier() { return 2; };

	bool apply(Pokemon* p);
	void unapply(Pokemon* p);

	bool immobilises(Pokemon* p);
	
private:
};

#endif