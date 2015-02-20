#ifndef INC_MULTISTATCHANGEEFFECT_H_
#define INC_MULTISTATCHANGEEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

#include <vector>

class MultipleStatChangeEffect : public StatusEffect
{
public:
	typedef std::vector<int> intArray_t;

	MultipleStatChangeEffect(intArray_t stats) {
		m_stats = stats;
	};
	~MultipleStatChangeEffect() {};

	std::string getName() { return "Statuses"; };
	std::string getDescription() { return ""; };

	bool immobilises(Pokemon* p) { return false; };
	bool tick(Pokemon* p) { return false; };
	bool switchOut(Pokemon* p) { return false; };

	int getTier() { return -1; };

	bool apply(Pokemon* p);
	void unapply(Pokemon* p) {};
	
private:
	intArray_t m_stats;
};
#endif