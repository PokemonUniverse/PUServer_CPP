#ifndef __POKEMONMOVE_H_
#define __POKEMONMOVE_H_

#include <boost/unordered_map.hpp>

#include "definitions.h"

class BattleTurn;
class BattleMechanics;
class MoveListEntry;
class SleepEffect;
class Pokemon;
class PokemonType;

typedef boost::unordered_map<uint32_t, BattleTurn*> battleTurnList_t; // Creature GUID, BattleTurn pointer

/**
* This class represents a move that a pokemon can use on its turn.
*/
class PokemonMove
{
public:
	PokemonMove(PokemonType *type, uint32_t power, double accuracy, uint32_t pp) 
		: m_type(type), m_power(power), m_accuracy(accuracy), m_pp(pp) {};
	~PokemonMove() {};

	virtual PokemonMove* clone() { return new PokemonMove(*this); }

	void setMoveListEntry(MoveListEntry *e) {
		m_entry = e;
	};

	MoveListEntry* getMoveListEntry() { return m_entry; };
	bool isSpecial(BattleMechanics *mech);
	virtual bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);

	virtual bool isAttack() { return (m_power != 0); };
	virtual bool isDamaging() { return isAttack(); };

	//************************************
	// Method:    beginTurn
	// FullName:  PokemonMove::beginTurn
	// Access:    public 
	// Returns:   void
	// Qualifier: This function is called at the beginning on a turn on which this move is about to be used.
	// Parameter: battleTurnList_t turn - the moves about to be used on this turn
	// Parameter: int index - the position of the source pokemon in the turn array
	// Parameter: Pokemon * source - the pokemon who is using the move
	//************************************
	void beginTurn(battleTurnList_t turn, int index, Pokemon *source) { };

	virtual uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);

	virtual bool hasHighCriticalHitRate() { return false; };

	void setAccuracy(double accuracy) { 
		if (accuracy > 1.0) {
			m_accuracy = 1.0;
		} else if (accuracy < 0.0) {
			m_accuracy = 0.0;
		} else {
			m_accuracy = accuracy;
		}
	};
	double getAccuracy() { return m_accuracy; };

	PokemonType* getType() { return m_type; };
	void setType(PokemonType* type) { m_type = type; };

	void setPower(int power) { m_power = power; };
	int getPower() { return m_power; };

	//This method is called when a pokemon who has this move is switched into the field.
	virtual void switchIn(Pokemon *p) {};

	//************************************
	// Method:    getStatusException
	// FullName:  PokemonMove::getStatusException
	// Access:    public 
	// Returns:   StatusEffect*
	// Qualifier: Some moves can be used even if a status effect (e.g. sleep) would normally prevent it.
	// If this move van be used a such, the class of the status effect is returned by this method. Otherwise,
	// the method returns null.
	//************************************
	virtual SleepEffect* getStatusException() { return NULL; };

	/**
     * Get the effectiveness of this move against a denfending pokemon.
     */
	double getEffectiveness(Pokemon* attacker, Pokemon* defender) {
		return getEffectiveness(m_type, attacker, defender);
	};
	/**
     * Get the effectiveness of one type of move against an arbitrary pokemon.
     */
	static double getEffectiveness(PokemonType* type, Pokemon* attacker, Pokemon* defender);

	virtual bool canCriticalHit() { return true; };
	
    /**
     * Get the priority of this move. Priority determines when this move will
     * be used during the turn.
     */
    virtual int getPriority() { return 0; };
protected:
	PokemonMove() {};

	PokemonType* m_type;
	uint32_t m_power;
	double m_accuracy;
	uint32_t m_pp;
	MoveListEntry* m_entry;
};

#endif