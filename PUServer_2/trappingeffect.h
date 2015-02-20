#ifndef INC_TRAPPINGEFFECT_H_
#define INC_TRAPPINGEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;

class TrappingEffect : public StatusEffect
{
	std::string m_name;

public:
	TrappingEffect(std::string name) : m_name(name) {};

	std::string getName() { return m_name; };
	virtual std::string getDescription() { return "was trapped!"; };

	int getTier() { return -1; };

	bool apply(Pokemon* p);

	bool tick(Pokemon* p) { return false; };
	bool isSingleton() { return false; };

	virtual bool canSwitch(Pokemon* p);
};
#endif