#ifndef INC_STATCHANGEEFFECT_H_
#define INC_STATCHANGEEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class Pokemon;
class StatMultiplier;

class StatChangeEffect : public StatusEffect
{
public:
	StatChangeEffect(int stat, bool raise, int stages = 1) {
		m_stat = stat;
		m_raise = raise;
		m_stages = m_undo = stages;
		m_desc = getDefaultDescription();
	};
	~StatChangeEffect() {};

	bool isSingleton() { return false; };

	std::string getName() { return "Status"; };

	void setDescription(std::string desc) { m_desc = desc; };
	std::string getDescription() { return m_desc; };

	bool immobilises(Pokemon* p) { return false; };
	bool tick(Pokemon* p) { return false; };
	bool switchOut(Pokemon* p) { return true; };

	int getTier() { return -1; };

	bool apply(Pokemon* p);
	void unapply(Pokemon* p);

	bool isRaise() { return m_raise; };
	int getStat() { return m_stat; };

private:
	int m_stat;
	int m_stages;
	int m_undo;
	
	bool m_raise;
	
	std::string m_desc;

	std::string getDefaultDescription();
	StatMultiplier* getMultiplier(Pokemon* p);
};

#endif