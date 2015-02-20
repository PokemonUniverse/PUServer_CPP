#ifndef INC_PERCENTEFFECT_H_
#define INC_PERCENTEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;

class PercentEffect : public StatusEffect
{
public:
	PercentEffect(double percent, bool stayOnSwitch, int tier, std::string description) {
		m_percent = percent;
		m_stayOnSwitch = stayOnSwitch;
		m_tier = tier;
		m_description = description;
	};
	~PercentEffect() {};

	std::string getName() { return "Percent Effect"; };
	std::string getDescription() { return ""; };

	bool switchOut(Pokemon* p) { return !m_stayOnSwitch; };
	bool tick(Pokemon* p);

	bool apply(Pokemon* p);
	void unapply(Pokemon* p) {};

	bool isSingleton() { return false; };
	bool immobilises(Pokemon* p) { return false; };

	int getTier() { return m_tier; };

	// Change the percent this move heals/hurts
	void setPercent(double percent) { m_percent = percent; };
	double getPercent() { return m_percent; };

protected:
	int calculateChange(Pokemon* p, double percent);
	
private:
	double m_percent;
	bool m_stayOnSwitch;
	int m_tier;
	std::string m_description;
};

#endif