#include "jumpkickmove.h"

#include <boost/format.hpp>

#include "pokemon.h"
#include "jewelmechanics.h"
#include "battlemechanics.h"
#include "battlefield.h"
#include "protecteffect.h"

uint32_t JumpKickMove::use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
{
	int power = (isAssignableFrom<JewelMechanics*>(mech)) ? m_jewelPower : m_power;
	setPower(power);

	bool ineffective = (getEffectiveness(attacker, defender) == 0.0);
	bool protect = defender->hasEffect<ProtectEffect>();

	if(ineffective) {
		attacker->addStatus(attacker, new JumpKickCalculationEffect());
	}

	int damage = mech->calculateDamage(this, attacker, defender, protect);
	if(ineffective) {
		attacker->removeStatus<JumpKickCalculationEffect>();
	}

	BattleField* field = attacker->getField();
	std::string message;

	if(!ineffective && !protect && PokemonMove::attemptHit(mech, attacker, defender)) {
		defender->changeHealth(-damage);
		return damage;
	}
	else if(protect) {
		StatusEffect* eff = defender->getEffect<ProtectEffect>();
		message = boost::str(boost::format("%s %s") % defender->getName() % eff->getDescription());
		field->showMessage(message);
	}

	message = boost::str(boost::format("%s kept going and crashed!") % attacker->getName());
	field->showMessage(message);

	int recoil = damage / 2;
	int max = defender->getStat(S_HP) / 2;
	if(recoil > max) {
		recoil = max;
	}

	attacker->changeHealth(-recoil);
	return damage;
}