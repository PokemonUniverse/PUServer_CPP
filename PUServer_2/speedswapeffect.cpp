#include "speedswapeffect.h"

#include "battlefield.h"

bool SpeedSwapEffect::tickField( BattleField* field )
{
	if(--m_turns == 0)	{
		field->removeEffect(this);
		return true;
	}

	return false;
}

void SpeedSwapEffect::unapplyToField( BattleField* field )
{
	field->showMessage("The twisted dimensions returned to normal!");
}