#include "confuseeffect.h"

#include "sleepeffect.h"
#include "pokemon.h"
#include "battlemechanics.h"

bool ConfuseEffect::apply( Pokemon* p )
{
	if(p->hasSubstitute() || p->hasAbility("Own Tempo")) {
		return false;
	}
	if(p->hasAbility("Tangled Feet")) {
		p->getMultiplier(S_EVASION)->increaseMultiplier();
	}

	m_turns = random_range(2,6);
	return true;
}

void ConfuseEffect::unapply( Pokemon* p )
{
	if(p->hasAbility("Tangled Feet")) {
		p->getMultiplier(S_EVASION)->decreaseMultiplier();
	}
}

bool ConfuseEffect::immobilises( Pokemon* p )
{
	if(p->hasEffect<SleepEffect>()) {
		return false;
	}

	if(--m_turns <= 0) {
		p->removeStatus(this);
		std::string message = boost::str(boost::format("%s snapped out of confusion!") % p->getName());
		p->getField()->showMessage(message);
		return false;
	}

	BattleField *field = p->getField();
	field->showMessage(boost::str(boost::format("%s is confused!") % p->getName()));

	if(random_range(0,10) <= 5) {
		return false;
	}

	PokemonMove* pmove = new ConfuseEffectMove(PokemonType::T_TYPELESS, 40, 1.0, 1);

	field->showMessage("It hurt itself in its confusion!");
	p->useMove(pmove, p);

	delete pmove;
	return true;
}

uint32_t ConfuseEffectMove::use( BattleMechanics* mech, Pokemon* source, Pokemon* target )
{
	int damage = mech->calculateDamage(this, source, target);
	target->changeHealth(-damage, true);
	return damage;
}