#include "onehitkillmove.h"

#include "pokemon.h"
#include "invulnerablestateeffect.h"

bool OneHitKillMove::attemptHit( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	if(defender->hasEffect<InvulnerableStateEffect>()) {
		return false;
	}

	double ratio = (double)(attacker->getLevel() - defender->getLevel()) / 128.0;
	if(ratio < 0) { return false; }

	setAccuracy(0.234 + ratio);

	return PokemonMove::attemptHit(mech, attacker, defender);
}

uint32_t OneHitKillMove::use( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	if(defender->hasAbility("Sturdy")) {
		std::string message = boost::str(boost::format("%s held sturdy!") % defender->getName());
		attacker->getField()->showMessage(message);
	}
	else
	{
		attacker->getField()->showMessage("It's a OHKO!");
		if(defender->hasSubstitute()) {
			defender->setSubstitute(1);
			defender->changeHealth(-1);
		} else {
			defender->faint();
		}
	}

	return 0;
}