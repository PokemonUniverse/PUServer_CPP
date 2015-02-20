#include "conversioneffect.h"

#include <boost/format.hpp>

#include "pokemon.h"
#include "pokemontype.h"

bool ConversionEffect::apply( Pokemon* p )
{
	m_types = p->getTypes();
	PokemonType* moveType = NULL;
	MoveListEntry* move;
	bool fail = true;

	for(int i = 0; i < 4; ++i) 
	{
		move = p->getMove(i);
		if(move != NULL
			&& !move->getMove()->getType()->equals(PokemonType::T_TYPELESS)
			&& move->getName() != "Conversion") {
				fail = false;
				break;
		}
	}

	if(!fail)
		return false;

	do 
	{
		move = p->getMove(random_range(0,3));
		if(move == NULL || move->getName() == "Conversion")
			continue;

		moveType = move->getMove()->getType();
	} while (moveType == NULL || moveType->equals(PokemonType::T_TYPELESS));

	pokemonTypeList_t list;
	list.push_back(moveType);
	p->setType(list);

	std::string message = boost::str(boost::format("%s became the %s type!") % p->getName() % moveType->toString());
	p->getField()->showMessage(message);

	return true;
}

void ConversionEffect::unapply( Pokemon* p )
{
	p->setType(m_types);
}

bool ConversionEffect::switchOut( Pokemon* p )
{
	p->setType(m_types);
	return true;
}

/************************************************************************/
/* Conversion 2 Effect                                                  */
/************************************************************************/
bool Conversion2Effect::apply( Pokemon* p )
{
	m_types = p->getTypes();
	MoveListEntry* entry = p->getOpponent()->getLastMove();
	if(entry == NULL) {
		p->getField()->showMessage("But it failed!");
		return false;
	}

	PokemonType* moveType = entry->getMove()->getType();
	typeList_t types = PokemonType::getTypes();
	typeList_t::iterator it = types.begin();
	while(it != types.end()) {
		PokemonType* type = (PokemonType*)it->second;
		if(moveType->getMultiplier(type) >= 1) {
			it = types.erase(it);
			continue;
		}

		++it;
	}

	int random = random_range(0, (types.size() - 1));
	pokemonTypeList_t newType;
	newType.push_back(types.at(random));
	p->setType(newType);

	return true;
}

void Conversion2Effect::unapply( Pokemon* p )
{
	p->setType(m_types);
}

bool Conversion2Effect::switchOut( Pokemon* p )
{
	p->setType(m_types);
	return true;
}