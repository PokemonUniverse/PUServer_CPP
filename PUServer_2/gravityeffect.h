#ifndef INC_GRAVITYEFFECT_H_
#define INC_GRAVITYEFFECT_H_

#include "definitions.h"
#include "fieldeffect.h"

#include "magnetriseeffect.h"
#include "movelistentry.h"

class Pokemon;
class BattleField;

class GravityEffect : public FieldEffect
{
	int m_turns;

public:
	GravityEffect() : m_turns(5) {}

	std::string getName() { return "Gravity"; };

	int getTier() { return 1; };
	bool tickPokemon(Pokemon* p) { return false; };
	bool isEffectivenessTransformer(bool enemy) { return !enemy; };
	bool isMoveTransformer(bool enemy) { return !enemy; };
	bool vetoesMove(Pokemon *p, MoveListEntry *entry) { return !canUseMove(entry->getName()); };
	
	bool applyToField(BattleField* field);
	void unapplyToField(BattleField* field);
	bool apply(Pokemon* p);
	void unapply(Pokemon* p);
	double getTransformedEffectiveness(PokemonType *move, PokemonType *pokemon);
	bool canUseMove(std::string move);
	MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move);
	bool tickField(BattleField* field);
};
#endif