#ifndef INC_FLINCHEFFECT_H_
#define INC_FLINCHEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class FlinchEffect : public StatusEffect
{
public:
	/**
     * Normally 'rounds' = 1, but hyper beam can be considered a flinch effect
     * for two rounds, since the first round it will not have any effect.
     */
	FlinchEffect() : m_rounds(1) {};
	FlinchEffect(int rounds) : m_rounds(rounds) {};
	~FlinchEffect() {};

	std::string getName() { return "Flinch"; };
	std::string getDescription() { return ""; };

	bool tick(Pokemon* p);
	bool switchOut(Pokemon* p) { return true; };
	
	bool apply(Pokemon* p);
	void unapply(Pokemon* p) {};

	int getTier() { return 0; };

	bool immobilises(Pokemon* p);
	
private:
	int m_rounds;
};
#endif