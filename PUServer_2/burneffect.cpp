#include "burneffect.h"

#include "pokemon.h"
#include "pokemontype.h"

bool BurnEffect::tick( Pokemon* p )
{
	if(p->hasAbility("Water Veil")) {
		return false;
	}

	double maximum = (double)p->getStat(S_HP);
	double loss = maximum / 8.0;
	int damage = (int)floor(loss);
	if (damage < 1) {
		damage = 1;
	}

	std::string message = boost::str(boost::format("%s was hurt by its burn!") % p->getName());
	p->getField()->showMessage(message);
	p->changeHealth(-damage, true);

	return false;
}

bool BurnEffect::apply( Pokemon* p )
{
	pokemonTypeList_t type = p->getTypes();

	for(int i = 0; i < type.size(); ++i) {
		if(type.at(i)->equals(PokemonType::T_FIRE)) {
			return true;
		}
	}

	if(!p->hasAbility("Guts")) {
		p->getMultiplier(S_ATTACK)->divideBy(2.0);
	}

	return true;
}

void BurnEffect::unapply( Pokemon* p )
{
	if(!p->hasAbility("Guts")) {
		p->getMultiplier(S_ATTACK)->multiplyBy(2.0);
	}
}