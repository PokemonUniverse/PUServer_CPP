#ifndef INC_TYPECUTMOVE_H_
#define INC_TYPECUTMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"
#include "fieldeffect.h"

#include "pokemontype.h"

class Pokemon;
class BattleField;
class StatusEffect;
class MoveListEntry;

class TypeCutStatus : public FieldEffect
{
	PokemonType* m_type;
	Pokemon* m_user;
	std::string m_name;

public:
	TypeCutStatus(PokemonType* type, Pokemon* user, std::string name) {
		m_type = type;
		m_user = user;
		m_name = name;
	};

	std::string getName() { return m_name; };
	std::string getDescription() { return ""; };

	bool switchOut(Pokemon* p);
	bool equals(StatusEffect* eff);
	bool applyToField(BattleField* field);

	bool tickField(BattleField* field) { return false; };
	int getTier() { return -1; };
	bool isMoveTransformer(bool enemy) { return !enemy; };

protected:
	MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move);
};

class TypeCutMove : public PokemonMove
{
	PokemonType* m_cut;
	std::string m_name;

public:
	TypeCutMove(PokemonType* type, uint32_t pp, PokemonType* cut, std::string name)
		: PokemonMove(type, 0, 1.0, pp), m_cut(cut), m_name(name) {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		return true;
	};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);
};
#endif