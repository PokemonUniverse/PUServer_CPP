#include "gravityeffect.h"

#include "battlefield.h"
#include "pokemon.h"
#include "intrinsicability.h"
#include "pokemontype.h"

bool GravityEffect::applyToField( BattleField* field )
{
	field->showMessage("Gravity intensified!");
	return true;
}

void GravityEffect::unapplyToField( BattleField* field )
{
	field->showMessage("Gravity returned to normal!");
}

bool GravityEffect::apply( Pokemon* p )
{
	if(p->hasAbility("Levitate")) {
		p->getAbility()->deactivate();
	}

	BattleField* field = p->getField();
	field->setNarrationEnabled(false);
	p->removeStatus<MagnetRiseEffect>();
	field->setNarrationEnabled(true);

	p->getMultiplier(S_ACCURACY)->multiplyBy(1.7);
	return true;
}

void GravityEffect::unapply( Pokemon* p )
{
	//TODO: Need fix! Interferes with Mold Breaker and Gastro Acid
	IntrinsicAbility* ability = p->getAbility();
	if(ability != NULL && ability->getName() == "Levitate") {
		ability->activate();
	}
	p->getMultiplier(S_ACCURACY)->divideBy(1.7);
}

double GravityEffect::getTransformedEffectiveness( PokemonType *move, PokemonType *pokemon )
{
	if(PokemonType::T_GROUND->equals(move) && PokemonType::T_FLYING->equals(pokemon)) {
		// Ground is neutral aganist flying under the effects of gravity.
		return 1.0;
	}

	return FieldEffect::getTransformedEffectiveness(move, pokemon);
}

bool GravityEffect::canUseMove( std::string move )
{
	std::string forbidden[] = { "Fly", "Bounce", "Hi Jump Kick", "Jump Kick", "Magnet Rise" };
	foreach(std::string str, forbidden) {
		if(str == move) return false;
	}

	return true;
}

MoveListEntry* GravityEffect::getTransformedMove( Pokemon *p, MoveListEntry *move )
{
	std::string name = move->getName();
	if(!canUseMove(name)) {
		std::string message = boost::str(boost::format("%s can't use %s because of Gravity.") % p->getName() % name);
		p->getField()->showMessage(message);
		return NULL;
	}

	return move;
}

bool GravityEffect::tickField( BattleField* field )
{
	if(--m_turns == 0) {
		field->removeEffect(this);
		return true;
	}

	return false;
}