#include "weathereffect.h"

#include <boost/format.hpp>
#include "battlefield.h"
#include "pokemon.h"
#include "intrinsicability.h"

bool WeatherEffect::tickField( BattleField* field )
{
	if(m_turns != -1 && --m_turns == 0) {
		// Time to fade
		field->removeEffect(this);
		return true;
	}

	tickWeather(field);
	return false;
}

bool WeatherEffect::hasEffects( BattleField* field )
{
	pokemonList_t pokemon = field->getActivePokemon();
	pokemonList_t::iterator it = pokemon.begin();
	for(; it != pokemon.end(); ++it) {
		Pokemon *p = (*it);
		if(p->hasAbility("Air Lock") || p->hasAbility("Cloud Nine"))
			return false;
	}

	return true;
}

void WeatherEffect::setTypes( Pokemon* p, pokemonTypeList_t types, bool apply )
{
	if(!p->hasAbility("Forecast") || p->getOriginalAbility()->getName() != "Forecast")
		return;

	teamInfo_s party(p->getMaster()->getBattleTeamIdx(), p->getMaster()->getBattleTeamPosition());
	if(apply) {
		m_types[party] = p->getTypes();
		p->setType(types);

		// Assume only changing to a single type
		std::string message = boost::str(boost::format("%s became the %s type!") % p->getName() % types[0]->toString());
		p->getField()->showMessage(message);
	}
	else if (m_types.find(party) != m_types.end()) {
		p->setType(m_types[party]);
		m_types.erase(party);
	}
}