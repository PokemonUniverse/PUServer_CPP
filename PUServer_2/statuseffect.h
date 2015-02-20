#ifndef __STATUSEFFECT_H_
#define __STATUSEFFECT_H_

#include "definitions.h"

#include "pokemontype.h"

class Pokemon;
class BattleTurn;
class MoveListEntry;

enum StatusEffectLocks_t
{
	NO_EFFECT_LOCK = 0,
	/**
	* A pokemon can have only one of freeze, burn, sleep, paralysis, and
	* poison, so we protect against this by giving this class of effects
	* a designated lock called SPECIAL_EFFECT_LOCK.
	*/		
	SPECIAL_EFFECT_LOCK = 1,
	/**
	* There can be only be only weather effect in play.
	*/
	WEATHER_EFFECT_LOCK = 2
};

enum StatusEffectState_t
{
	STATE_ACTIVE = 0,
	STATE_DEACTIVATED = 1,
	STATE_REMOVABLE = 2
};

enum StatusEffects_t
{
	EFFECT_DEFAULT = 0,
	EFFECT_BURN = 1
};

class StatusEffect
{
public:
	StatusEffect() {};
	virtual ~StatusEffect() {};

	void setInducer(Pokemon* p) {
		m_inducer = p;
	};

	Pokemon* getInducer() {
		return m_inducer;
	};

	StatusEffectLocks_t getLock() {
		return m_lock;
	};

	static int const getTierCount() { return 6; };

	virtual StatusEffects_t getEffect() { return EFFECT_DEFAULT; };

	virtual std::string getName() { return ""; };

	// Can this statuc effect by baton passed (Almost all can)
	virtual bool isPassable() { return true; };

	// Does this effect allow the application of the given status effect to
	// a particular pokemon? This is called on the target pokemon
	virtual bool allowsStatus(StatusEffect* eff, Pokemon* source, Pokemon* target) {
		return true;
	};

	// Return whether this effect can coexist with another effect.
	virtual bool isExclusiveWith(StatusEffect* eff) {
		return (m_lock == NO_EFFECT_LOCK) ? false : (m_lock == eff->getLock());
	};

	// Return whether this effect immobilises the pokemon.
	virtual bool immobilises(Pokemon* p) { return false; };

	// Description
	virtual std::string getDescription() { return ""; };

	/**
	* Called each turn that this status effect is applied to a pokemon.
	* Returns whether the status was removed.
	*/
	virtual bool tick(Pokemon* p) { return true; };

	/**
	* Remove the tier of the after-turn effect.
	*/
	virtual int getTier() {	return -1; }


	// Called when a pokemon with this status effect switches in.
	virtual void switchIn(Pokemon *p) {};

	/**
	* Called when a pokemon with this status effect switches out.
	* Returns true if the status effect should be removed.
	*/
	virtual bool switchOut(Pokemon* p) { return true; }

	// Unapply this status effect
	virtual void unapply(Pokemon* p) {};

	/**
	* Applies the initial effects of the status to a pokemon but does not add
	* the status to the list of statuses the pokemon has.
	*/
	virtual bool apply(Pokemon* p) { return true; };

	/**
	* Return whether this status effect can apply statuses through a
	* substitute.
	*/
	virtual bool hitsThroughSubstitude() { return false; };

	/**
	* Does this status effect transform effectivenesses?
	*/
	virtual bool isEffectivenessTransformer(bool enemy) { return false; }

	/**
	* This method is called when the pokemon to whom the status effect is
	* applied is just about to execute his turn.
	*/
	virtual void executeTurn(Pokemon *p, BattleTurn *turn) {};

	/**
	* Returns true if this status effect is capable of transforming moves.
	* @param enemy whether this is an enemy move
	*/
	virtual bool isMoveTransformer(bool enemy) { return false; };

	// Return whether this effect listens for damage.
	virtual bool isListener() { return false; };

	// React to damage
	virtual void informDamaged(Pokemon *attacker, Pokemon *target, MoveListEntry *move, uint32_t damage) {};

	// Determine whether this effect deactivates a pokemon
	virtual bool deactivates(Pokemon *p) { return false; };

	virtual bool equals(StatusEffect* eff) {
		if(eff->getEffect() != getEffect()) {
			return false;
		}

		return isSingleton();
	};

	template<class Type> bool equals() {
		return (typeid(this) == typeid(Type*));
	};

	/**
	* Determine whether this effect is a singleton -- i.e., whether only
	* a single copy of it can be present on a pokemon.
	*/
	virtual bool isSingleton() { return true; };

	virtual void informDuplicateEffect(Pokemon *p) {
		//TODO: Send message to field "But it failed!"
		// Also do this to inherited functions
	};

	// Returns whether this 
	virtual bool canSwitch(Pokemon *p) { return true; };

	virtual bool canSwitch() { return true; };

	/**
	* Returns whether this status effect vetoes the choice of a particular
	* move.
	*/
	virtual bool vetoesMove(Pokemon *p, MoveListEntry *entry) { return false; };

	/**
	* Begin ticking this effect.
	*/
	virtual void beginTick() {};

	virtual bool isStatusListener() { return false; };
	virtual void informStatusApplied(Pokemon* source, Pokemon* poke, StatusEffect* eff) {};
	virtual void informStatusRemoved(Pokemon* poke, StatusEffect* eff) {};

	/************************************************************************/
	/* Non virtual functions                                                */
	/************************************************************************/
	
	StatusEffect* clone() {	return new StatusEffect(*this);	};

	// Disable this status effect (i.e. mark it as removable). This cannot be undone.
	void disable() { m_state = STATE_REMOVABLE; };

	void deactivate() {
		if(m_state != STATE_REMOVABLE) {
			m_state = STATE_DEACTIVATED;
		}
	};

	void activate() {
		if(m_state != STATE_REMOVABLE) {
			m_state = STATE_ACTIVE;
		}
	};

	bool isActive() {
		return (m_state == STATE_ACTIVE);
	};

	bool isRemovable() {
		return (m_state == STATE_REMOVABLE);
	}

	/**
	* Get transformed effectiveness based on this status effect.
	* @param move type of the move
	* @param pokemon type of the pokemon
	* @param enemy whether the Pokemon using the move is an enemy
	*/
	double getEffectiveness(PokemonType *move, PokemonType *pokemon, bool enemy) {
		if (enemy) {
			return getEnemyTransformedEffectiveness(move, pokemon);
		}
		return getTransformedEffectiveness(move, pokemon);
	}

	/**
	* Transform a move based on this status effect.
	* @param move  the move to transform; the method is free to modify it
	*              although it may also return a new MoveListEntry
	* @param enemy whether the Pokemon p is an enemy
	* @return      the transformed move
	*/
	MoveListEntry* getMove(Pokemon *p, MoveListEntry *move, bool enemy) {
		if (enemy) {
			return getEnemyTransformedMove(p, move);
		}
		return getTransformedMove(p, move);
	}

protected:
	StatusEffectLocks_t m_lock;

	virtual double getTransformedEffectiveness(PokemonType *move, PokemonType *pokemon) {
		return move->getMultiplier(pokemon);
	}

	virtual double getEnemyTransformedEffectiveness(PokemonType *move, PokemonType *pokemon) {
		return move->getMultiplier(pokemon);
	}

	virtual MoveListEntry* getEnemyTransformedMove(Pokemon *p, MoveListEntry *move) {
		return move;
	}

	virtual MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move) {
		return move;
	}

private:
	StatusEffectState_t m_state;
	Pokemon* m_inducer;
};

#endif