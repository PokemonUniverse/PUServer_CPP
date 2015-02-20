#include "statuseffect.h"
#include "pokemon.h"
#include "fieldeffect.h"

bool FieldEffect::tick(Pokemon* p)
{
	if(!m_ticked)
	{
		m_ticked = true;
		if(tickField(p->getField())) {
			return true;
		}
	}

	return tickPokemon(p);
}