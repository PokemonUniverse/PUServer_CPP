#ifndef INC_AWESOMEEFFECT_H_
#define INC_AWESOMEEFFECT_H_

#include <math.h>
#include <boost/format.hpp>

#include "definitions.h"
#include "statuseffect.h"

#include "pokemon.h"
#include "battlefield.h"
#include "pokemonmove.h"
#include "pokemontype.h"

class StatusEffect;
class Pokemon;

class AwesomeEffect : public StatusEffect
{
public:
	AwesomeEffect() : m_turns(1) { };
	~AwesomeEffect();

	std::string getName() { return "Awesomeness"; };

	bool switchOut(Pokemon* p);

	bool tick(Pokemon* p);

	int getTier() { return 4; };

	bool apply(Pokemon* p) { return true; };
	void unapply(Pokemon* p) {};

	std::string getDescription() { " insulted God out of anger!"; };

	bool immobilises(Pokemon* p);
private:
	int m_turns;
};

bool AwesomeEffect::switchOut( Pokemon* p )
{
	std::string message = boost::str(boost::format("God punishes %s for leaving his presence!") % p->getName());
	p->getField()->showMessage(message);
	p->changeHealth(-1000);

	return false;
}

bool AwesomeEffect::tick( Pokemon* p )
{
	double maximum = (double)p->getStat(S_HP);
	double loss = (maximum / 8.0) * (double)m_turns;
	int health = (int)floor(loss);
	if(health < 1) {
		health = 1;
	}

	std::string message = boost::str(boost::format("%s is punished by God!") % p->getName());
	p->getField()->showMessage(message);
	p->changeHealth(-health);

	return false;
}

bool AwesomeEffect::immobilises( Pokemon* p )
{
	BattleField *field = p->getField();

	if(++m_turns >= 4) {
		std::string message = boost::str(boost::format("God struck down %s with all his might!") % p->getName());
		field->showMessage(message);
		p->useMove(new PokemonMove(PokemonType::T_TYPELESS, 10000, 10.0, 1), p);
	}

	return true;
}

#endif