#ifndef INC_RESTRAININGEFFECT_H_
#define INC_RESTRAININGEFFECT_H_

#include <boost/format.hpp>

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;

class RestrainingEffect : public StatusEffect
{
	int m_turns;
	std::string m_description;
	std::string m_name;

public:
	RestrainingEffect(std::string name, std::string description) : m_name(name), m_description(description) {};

	std::string getName() { return m_name; };
	std::string getDescription() { return boost::str(boost::format("was %s") % m_description); };

	int getTier() { return 3; };

	bool tick(Pokemon* p);
	bool apply(Pokemon* p);
	bool canSwitch(Pokemon* p);
};
#endif