#include "advancedmechanics.h"

#include "pokemon.h"
#include "pokemonmove.h"
#include "pokemontype.h"
#include "battlefield.h"
#include "statchangeeffect.h"
#include "jewelmechanics.h"

uint32_t AdvancedMechanics::calculateStat( Pokemon *p, int i )
{
	if(i < 0 || i > 5) { return 0; }

	int common = (int)((int)(((2.0 * p->getBase(i)) + p->getIv(i) )) * (p->getLevel() / 100.0));

	if(i == S_HP) {
		if(p->getName() == "Shedinja") {
			return 1;
		}
		else {
			return common + 10 + p->getLevel();
		}
	}

	return (int)((common + 5) * p->getNature()->getEffect(i));
}

uint32_t AdvancedMechanics::calculateDamage( PokemonMove *move, Pokemon *attacker, Pokemon *defender, bool silent /*= false*/ )
{
	BattleField* field = attacker->getField();
	PokemonType* moveType = move->getType();
	const bool special = isMoveSpecial(move);

	bool isCritical = ( move->canCriticalHit() && isCriticalHit(move, attacker, defender) );

	double attack = attacker->getStat( (special ? S_SPATTACK : S_ATTACK) );
	int defStat = special ? S_SPDEFENCE : S_DEFENCE;

	StatMultiplier* mul = defender->getMultiplier(defStat);
	double defMultiplier = mul->getMultiplier();
	if(isCritical && (defMultiplier > 1.0)) { 
		defMultiplier = mul->getSecondMultiplier();
	}
	double defence = defender->getStat(defStat, defMultiplier);

	const int rnd = random_range(15,100);
	double multiplier = move->getEffectiveness(attacker, defender);

	if(multiplier > 1.0) {
		if(!silent) {
			field->showMessage("It's super effective!");
		}
	}
	else if(multiplier == 0.0) {
		if(!silent) {
			std::string message = boost::str(boost::format("It doesn't affect %s...") % defender->getName());
			field->showMessage(message);
		}

		return 0; // Return 0 in order to prevent a critical hit from occurring
	}
	else if(multiplier < 1.0) {
		if(!silent) {
			field->showMessage("It's not very effective...");
		}
	}

	const bool stab = attacker->isType(moveType);
	double stabFactor = attacker->hasAbility("Adaptability") ? 2.0 : 1.5;

	int damage = (int)(((int)((int)(((int)((2 * attacker->getLevel()) / 5.0 + 2.0) 
		* attack
		* move->getPower())
		/ defence)
		/ 50.0)
		+ 2)
		* rnd
		* (stab ? stabFactor : 1.0)
		* multiplier);

	if(isCritical) 
	{
		damage *= attacker->hasAbility("Sniper") ? 3 : 2;
		if(defender->hasAbility("Anger Point")) 
		{
			if(!silent) {
				std::string message = boost::str(boost::format("%s's Anger Point raised its attack!") % defender->getName());
				field->showMessage(message);
			}

			StatChangeEffect* eff = new StatChangeEffect(S_ATTACK, true, 12);
			eff->setDescription("");
			defender->addStatus(defender, eff);
		}

		if(!silent) {
			field->showMessage("A critical hit!");
		}
	}
	
	return ((damage < 1) ? 1 : damage);
}

bool AdvancedMechanics::attemptHit( PokemonMove *move, Pokemon *attacker, Pokemon *defender )
{
	BattleField* field = attacker->getField();
	double accuracy = move->getAccuracy();
	bool hit = false;

	if((accuracy != 0.0) 
		&& (attacker->hasAbility("No Guard") || defender->hasAbility("No Guard")) 
		)//|| attacker->hasEffect<LockOnEffect>()) 
	{
		hit = true;
	}
	else {
		double effective = (accuracy * attacker->getAccuracy()->getMultiplier()) / defender->getEvasion()->getMultiplier();
		if(effective > 1.0) effective = 1.0;

		hit = random_range(0,100) <= (effective * 100);
	}

	if(!hit) {
		std::string message = boost::str(boost::format("%s's attack missed!") % attacker->getName());
		field->showMessage(message);
	}

	return hit;
}

bool AdvancedMechanics::isMoveSpecial( PokemonMove *move )
{
	return move->getType()->isSpecial();
}

bool AdvancedMechanics::isCriticalHit( PokemonMove* move, Pokemon* user, Pokemon* target )
{
	if(target->isCriticalImmune()) {
		return false;
	}

	/* TODO: Uncomment when LuckyChantEffect is implemented
	FieldEffect* effect = user->getField()->getEffectByType<LuckyChantEffect>();
	if(effect != NULL) {
		LuckyChantEffect* eff = (LuckyChantEffect*)effect;
		if(eff->isActive()) {
			return false;
		}
	}*/

	int moveFactor = 0;
	if(move->hasHighCriticalHitRate()) {
		moveFactor = isAssignableFrom<JewelMechanics*>(this) ? 1 : 3;
	}

	int factor = user->getCriticalHitFactor()
		//+ (user->hasItem("Scope Lens") ? 1 : 0) // TODO: + (FE/L * 1)
		+ moveFactor;

	double chance = 0.0;
	switch(factor) 
	{
	case 1:
		chance = 0.0625;
		break;
	case 2:
		chance = 0.125;
		break;
	case 3:
		chance = 0.25;
		break;
	case 4:
		chance = 0.332;
		break;
	default:
		chance = 0.5;
		break;
	}

	return (random_range(0,100) <= (chance * 100));

}