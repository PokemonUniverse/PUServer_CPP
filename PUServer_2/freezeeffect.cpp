#include "freezeeffect.h"

#include <boost/format.hpp>

#include "pokemon.h"
#include "battleturn.h"
#include "pokemonmove.h"

bool FreezeEffect::apply( Pokemon* p )
{
	if(p->hasAbility("Magma Armor")) {
		return false;
	}

	if(p->isType(PokemonType::T_ICE)) {
		return false;
	}

	/* TODO: Uncomment when SunEffect is implemented
	if(p->getField()->getEffectByType<SunEffect>() != NULL) {
			return false;
		}*/
	
	return true;
}

void FreezeEffect::unapply( Pokemon* p )
{
	std::string message = boost::str(boost::format("%s was defrosted!") % p->getName());
	p->getField()->showMessage(message);
}

void FreezeEffect::executeTurn( Pokemon *p, BattleTurn *turn )
{
	std::string name = turn->getMove(p)->getMoveListEntry()->getName();
	if(name == "Flame Wheel" || name == "Sacred Fire") {
		p->removeStatus(this);
	}
}

void FreezeEffect::informDamaged( Pokemon *attacker, Pokemon *target, MoveListEntry *move, uint32_t damage )
{
	PokemonMove *pmove = move->getMove();
	if(pmove != NULL) {
		if(pmove->getType()->equals(PokemonType::T_FIRE)) {
			target->removeStatus(this);
		}
	}
}

bool FreezeEffect::immobilises( Pokemon* p )
{
	int rnd = random_range(0,100);
	if(rnd <= 25) {
		p->removeStatus(this);
		return false;
	}

	std::string message = boost::str(boost::format("%s is frozen solid!") % p->getName());
	p->getField()->showMessage(message);

	return true;
}