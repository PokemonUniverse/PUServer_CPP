#ifndef __POKEMONNATURE_H_
#define __POKEMONNATURE_H_

#include <vector>

#include "definitions.h"
#include "enums.h"

class PokemonNature;

typedef std::vector<PokemonNature*> naturesList_t;

class PokemonNature
{
public:
	static const PokemonNature* N_LONELY;
	static const PokemonNature* N_BRAVE;
	static const PokemonNature* N_ADAMANT;
	static const PokemonNature* N_NAUGHTY;
	static const PokemonNature* N_BOLD;
	static const PokemonNature* N_RELAXED;
	static const PokemonNature* N_IMPISH;
	static const PokemonNature* N_LAX;
	static const PokemonNature* N_TIMID;
	static const PokemonNature* N_HASTY;
	static const PokemonNature* N_JOLLY;
	static const PokemonNature* N_NAIVE;
	static const PokemonNature* N_MODEST;
	static const PokemonNature* N_MILD;
	static const PokemonNature* N_QUIET;
	static const PokemonNature* N_RASH;
	static const PokemonNature* N_CALM;
	static const PokemonNature* N_GENTLE;
	static const PokemonNature* N_SASSY;
	static const PokemonNature* N_CAREFUL;
	static const PokemonNature* N_QUIRKY;
	static const PokemonNature* N_HARDY;
	static const PokemonNature* N_SERIOUS;
	static const PokemonNature* N_BASHFUL;
	static const PokemonNature* N_DOCILE;

	static const PokemonNature* getNature(uint32_t i) {
		if(i < 0 || i >= m_natures.size()) { return NULL; }

		return m_natures[i];
	};

	static std::vector<std::string> getNatureNames() {
		PokemonNature *tmp = NULL;
		std::vector<std::string> names;
		for(naturesList_t::iterator it = m_natures.begin(); it != m_natures.end(); ++it)
		{
			if(tmp = (*it)) {
				names.push_back(tmp->getName());
			}
		}

		return names;
	};

	/**
	* Get the effect a nature has on a particular stat.
	* This will be 0.9, 1, or 1.1.
	*
	* @param i the index of the statistic
	*/
	double getEffect(int i) {
		return (i == m_benefits) ? 1.1 : ((i == m_harms) ? 0.9 : 1.0);
	};

	std::string getName() {	return m_name; };
	int getBenefits() { return m_benefits; };
	int getHarms() { return m_harms; };

private:
	int m_nature;
	int m_harms;
	int m_benefits;

	std::string m_name;

	static naturesList_t m_natures;

	PokemonNature(std::string name, int beneifts, int harms) {
		m_name = name;
		m_benefits = beneifts;
		m_harms = harms;
		m_nature = m_natures.size();

		m_natures.push_back(this);
	};

	PokemonNature(std::string name) {
		PokemonNature *tmp = NULL;
		for(naturesList_t::iterator it = m_natures.begin(); it != m_natures.end(); ++it)
		{
			if((tmp = (*it))) {
				m_name = tmp->getName();
				m_benefits = tmp->getBenefits();
				m_harms = tmp->getHarms();
				break;
			}
		}
	};
};

#endif