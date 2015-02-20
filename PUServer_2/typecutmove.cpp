#include "typecutmove.h"

#include "pokemon.h"
#include "battlefield.h"
#include "pokemonmove.h"

uint32_t TypeCutMove::use( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	BattleField* field = attacker->getField();
	if(!field->applyEffect(new TypeCutStatus(m_cut, attacker, m_name)))
	{
		field->showMessage("But it failed!");
	}

	return 0;
}

bool TypeCutStatus::switchOut( Pokemon* p )
{
	if(p == m_user) {
		p->getField()->removeEffect(this);
	}

	return FieldEffect::switchOut(p);
}

bool TypeCutStatus::equals( StatusEffect* eff )
{
	if(!isAssignableFrom<TypeCutStatus*>(eff))
		return false;

	TypeCutStatus* rhs = dynamic_cast<TypeCutStatus*>(eff);
	return m_type->equals(rhs->m_type);
}

bool TypeCutStatus::applyToField( BattleField* field )
{
	std::string message = boost::str(boost::format("%s attacks were weakened!") % m_type->toString());
	field->showMessage(message);

	return true;
}

MoveListEntry* TypeCutStatus::getTransformedMove( Pokemon *p, MoveListEntry *move )
{
	PokemonMove* pmove = move->getMove();
	if(pmove->getType()->equals(m_type)) {
		pmove->setPower(pmove->getPower() / 2);
	}

	return move;
}