#ifndef __FIELDEFFECT_H_
#define __FIELDEFFECT_H_

#include "definitions.h"
#include "statuseffect.h"

class BattleField;
class Pokemon;

class FieldEffect : public StatusEffect
{
public:
	virtual ~FieldEffect() {};
    /**
     * Returns this object, rather than making a clone. This is important so
     * that all pokemon getting a clone of a FieldEffect are actually getting
     * the same effect.
     */
	FieldEffect* clone() { return this; };
	FieldEffect* getFieldCopy() { return new FieldEffect(*this); };

	virtual void beginTick() { m_ticked = false; };

	/**
     * Tick the effect on its associated field.
     * Subclasses should override tickPokemon() or tickField() rather than
     * this method.
     */
	virtual bool tick(Pokemon* p);

    /**
     * Generally a FieldEffect is said to be healed when a pokemon is switched
     * out because it will be reapplied when the pokemon returns. This is the
     * case with all current pokemon field effects (i.e. weather) in the sense
     * that a pokemon who switches out during a weather effect and returns when
     * the weather has cleared up is not still affected by that weather. If
     * this function returned false, a pokemon switching in in clear times who
     * had left when weather was present would still be affected by the weather.
     */	
	virtual bool switchOut(Pokemon* p) { return true; };

	virtual bool applyToField(BattleField* field) { return true; };
	virtual void unapplyToField(BattleField* field) {};

protected:
	FieldEffect() : m_ticked(true) { };

	virtual bool tickField(BattleField* field) { return true; }

	virtual bool tickPokemon(Pokemon* p) { return false; }

private:
	bool m_ticked;

};
#endif