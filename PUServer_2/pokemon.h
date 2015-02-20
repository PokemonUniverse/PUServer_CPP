#ifndef __POKEMON_H_
#define __POKEMON_H_

#include <vector>

#include "definitions.h"
#include "enums.h"

#include "battlefield.h"
#include "battlemechanics.h"
#include "pokemonspecies.h"
#include "pokemonnature.h"
#include "movelistentry.h"
#include "statmultiplier.h"
#include "statusmove.h"

class BattleField;
class StatusEffect;
class SleepEffect;
class SubstituteEffect;
class IntrinsicAbility;

typedef std::vector<MoveListEntry*> moveList_t;

class Pokemon : public PokemonSpecies
{
	//typedef std::vector<StatusEffect*> statusEffectList_t;

public:
	Pokemon(BattleMechanics* mech, PokemonSpecies* species, PokemonNature* nature, std::string ability, std::string item,
		int gender, int level, intArray_t ivs, moveList_t moves, intArray_t ppUps, bool validate);

	~Pokemon();

	virtual Pokemon* getPokemon() { return this; };
	virtual const Pokemon* getPokemon() const { return this; };

	static std::string getStatName(int stat)
	{
		switch (stat) 
		{
		case S_HP: return "HP";
		case S_ATTACK: return "attack";
		case S_DEFENCE: return "defence";
		case S_SPEED: return "speed";
		case S_SPATTACK: return "special attack";
		case S_SPDEFENCE: return "special defence";
		case S_ACCURACY: return "accuracy";
		case S_EVASION: return "evasion";
		}

		return "";
	}
	static std::string getStatNameShort(int stat)
	{
		switch (stat) 
		{
		case S_HP: return "HP";
		case S_ATTACK: return "Atk";
		case S_DEFENCE: return "Def";
		case S_SPEED: return "Spd";
		case S_SPATTACK: return "SAtk";
		case S_SPDEFENCE: return "SDef";
		case S_ACCURACY: return "Acc";
		case S_EVASION: return "Evas";
		}
		return "";
	}

	bool hasItem(std::string name) { return false; }; //TODO: hasItem(std::string name) implementation

	double getEffectiveness(PokemonType* move, PokemonType* pokemon, bool enemy) {
		return getEffectiveness(m_statuses, move, pokemon, enemy);
	};
	static double getEffectiveness(fieldEffects_t statuses, PokemonType *move, PokemonType *pokemon, bool enemy);
	static double getEffectiveness(statusEffectList_t statuses, PokemonType *move, PokemonType *pokemon, bool enemy);

	void attachToField( BattleField* field );

	// Create a substitute to take hits for this Pokemon
	bool createSubstitude();
	void setSubstitute(int hp) { m_substitute = hp; };
	int getSubstitute() { return m_substitute; };
	bool hasSubstitute() { return (m_substitute != 0); };

	void changeHealth(int hp, bool throughSubstitude = false);
	int getHealth() { return m_hp; };

	void setLevel(int level) { m_level = level; };
	int getLevel() { return m_level; };

	//************************************
	// Method:    faint
	// FullName:  Pokemon::faint
	// Access:    public 
	// Returns:   void
	// Qualifier: Causes this pokemon to faint
	//************************************
	void faint();

	int getCriticalHitFactor() {
		return hasAbility("Super Luck") ? 2 : 1;
	}

	bool isCriticalImmune() {
		return (hasAbility("Battle Armor") || hasAbility("Shell Armor"));
	}

	int getIv(int i) {
		if(i < 0 || i > 5) { return 0; }
		return m_iv[i];
	};

	int getStat(int i, double multiplier) {
		if ((i < 0) || (i > 5)) { return 0; }
		return (int)(((double)m_stat[i]) * multiplier);
	}

	int getStat(int i) {
		if ((i < 0) || (i > 5)) { return 0; }
		// Consider stat modifications.
		return getStat(i, m_multiplier[i]->getMultiplier());
	}

	int getRawStat(int i) {
		if ((i < 0) || (i > 5)) { return 0; }
		return m_stat[i];
	}

	void setRawStat(int i, int newStat) {
		if ((i < 0) || (i > 5)) { return; }
		m_stat[i] = newStat;
	}

	void setPp(uint32_t i, int amount) {
		if(i >= m_pp.size()) return;
		if(amount < 0) amount = 0;
		m_pp[i] = amount;
	}

	int getPp(int i) {
		if((i < 0) || (i >= m_move.size()) || (m_move[i] == NULL)) {
			return -1;
		}

		return m_pp[i];
	}

	int getMaxPp(int i) {
		if((i < 0) || (i >= m_move.size()) || (m_move[i] == NULL)) {
			return -1;
		}

		return m_maxPp[i];
	}

	StatMultiplier* getMultiplier(int i);

	StatMultiplier* getAccuracy() { return m_accuracy; };
	StatMultiplier* getEvasion() { return m_evasion; };

	PokemonNature* getNature() { return m_nature; };
	BattleField* getField() { return m_field; };
	bool isFainted() { return m_fainted; };

	bool isType(PokemonType* type)
	{
		for(pokemonTypeList_t::iterator it = m_type.begin(); it != m_type.end(); ++it)
		{
			PokemonType *t = (*it);
			if(t->equals(type)) {
				return true;
			}
		}
		return false;
	};
	
	// Add a status effect to this pokemon.
	StatusEffect* addStatus(Pokemon* source, StatusEffect* eff);
	// Remove a status effect from this pokemon.
	void removeStatus(StatusEffect *eff);
	void removeStatus(int lock);
	template< class C > void removeStatus(){
		statusEffectList_t list = m_statuses;
		statusEffectList_t::iterator it = list.begin();
		for(; it != list.end(); ++it) {
			StatusEffect *effect = (*it);
			if(!effect->isRemovable() && isAssignableFrom< C* >(effect)) {
				unapplyEffect(effect);
			}
		}
	};

	bool allowStatus(StatusEffect* eff, Pokemon* source);
	// Remove status effects that have ended.
	void synchroniseStatuses();

	void switchIn();
	void switchOut();

	void setAbility(IntrinsicAbility *abl, bool ignoreTransferability);

	// Get one of this pokemon's moves
	MoveListEntry* getMove(int32_t i);

	/**
     * Get the last move used by this pokemon, or null if the pokemon has not
     * used a move since it has been out.
     */
	MoveListEntry* getLastMove() { return m_lastMove; };

	//************************************
	// Method:    executeTurn
	// FullName:  Pokemon::executeTurn
	// Access:    public 
	// Returns:   void
	// Qualifier: This method is called when the pokemon is just about to execute its turn
	// Parameter: BattleTurn * turn - the turn that is about to be executed
	//************************************
	void executeTurn(BattleTurn *turn);

	bool isImmobilised(SleepEffect *exception);

	int useMove(int32_t move, Pokemon *target);
	int useMove(MoveListEntry *move, Pokemon *target);
	int useMove(PokemonMove *move, Pokemon *target);

	//************************************
	// Method:    hasAbility
	// FullName:  Pokemon::hasAbility
	// Access:    public 
	// Returns:   bool
	// Qualifier: Returns whether this pokemon has a particular ability
	// Parameter: std::string name
	//************************************
	bool hasAbility(const std::string name);

	/**
     * Return whether this Pokemon has a particular class of effect.
     */
	bool hasEffect(int lock) {
		return (getEffect(lock) != NULL);
	}

	/**
     * Return the effect applied to this pokemon of a particular lock or
     * null if there is no such effect applied.
     */
	StatusEffect* getEffect(int lock);

	//************************************
	// Method:    hasEffect
	// FullName:  Pokemon::hasEffect<Type>
	// Access:    public 
	// Returns:   bool
	// Qualifier: Return whether this Pokemon has a particular class of effect.
	//************************************
	template< class Type > bool hasEffect()
	{
		return (getEffect<Type>() != NULL);
	};

	template< class Type > StatusEffect* getEffect()
	{
		statusEffectList_t::iterator it = m_statuses.begin();
		for(; it != m_statuses.end(); ++it) {
			StatusEffect *eff = (*it);
			if(!eff || !eff->isActive()) {
				continue;
			}
			if(isAssignableFrom<Type*>(eff)) {
				return eff;
			}
		}

		return NULL;
	};

	//************************************
	// Method:    beginStatusTicks
	// FullName:  Pokemon::beginStatusTicks
	// Access:    public 
	// Returns:   void
	// Qualifier: Begin ticking effects.
	//************************************
	void beginStatusTicks();

	//************************************
	// Method:    getStatusesByTier
	// FullName:  Pokemon::getStatusesByTier
	// Access:    public 
	// Returns:   statusEffectList_t
	// Qualifier: Get all status effects of a certain tier
	// Parameter: int tier
	//************************************
	statusEffectList_t getStatusesByTier(int tier);

	Pokemon* getOpponent();

	//************************************
	// Method:    getOriginalAbility
	// FullName:  Pokemon::getOriginalAbility
	// Access:    public 
	// Returns:   IntrinsicAbility*
	// Qualifier: Return the original ability of this pokemon.
	//************************************
	IntrinsicAbility* getOriginalAbility() { return m_originalAbility; };

	//************************************
	// Method:    getAbility
	// FullName:  Pokemon::getAbility
	// Access:    public 
	// Returns:   IntrinsicAbility*
	// Qualifier: Return this Pokemon's ability
	//************************************
	IntrinsicAbility* getAbility();

	//************************************
	// Method:    getTeammates
	// FullName:  Pokemon::getTeammates
	// Access:    public 
	// Returns:   pokemonList_t
	// Qualifier: Returns a list of in-battle team mates
	//************************************
	pokemonList_t getTeammates();

	//************************************
	// Method:    getNormalStatuses
	// FullName:  Pokemon::getNormalStatuses
	// Access:    public 
	// Returns:   statusEffectList_t
	// Qualifier: Get a list of statuses that are not special, weather, abilities, or items
	// Parameter: int lock
	//************************************
	statusEffectList_t getNormalStatuses(int lock);

	bool isFirstTurn() { return m_firstTurn; };

protected:
	//************************************
	// Method:    getTransformedMove
	// FullName:  Pokemon::getTransformedMove
	// Access:    protected 
	// Returns:   MoveListEntry*
	// Qualifier: Transform a move based on the status effects applied to the pokemon.
	// Parameter: MoveListEntry * move
	// Parameter: bool enemy - whether this Pokemon is an enemy
	//************************************
	MoveListEntry* getTransformedMove(MoveListEntry* move, bool enemy);

private:
	void informStatusListeners(Pokemon* source, StatusEffect* eff, bool applied);

	void informDamaged(Pokemon* source, MoveListEntry* move, int damage);

	//************************************
	// Method:    unapplyEffect
	// FullName:  Pokemon::unapplyEffect
	// Access:    private 
	// Returns:   void
	// Qualifier: Invoke unapply on a status effect, optionally disabling it.
	// Parameter: StatusEffect * eff
	// Parameter: bool disable
	//************************************
	void unapplyEffect(StatusEffect *eff, bool disable = true);
	
	int32_t m_hp;

	uint32_t m_level;

	PokemonNature* m_nature;
	
	intArray_t m_stat;
	intArray_t m_pp, m_maxPp;
	intArray_t m_ppUp;
	intArray_t m_iv;

	int m_party, m_position;
	int m_substitute; // Health of substitute, 0 = no substitute

	bool m_fainted;
	bool m_firstTurn;
	bool m_shiny;

	std::string m_nickname;
	std::string m_abilityName;

	IntrinsicAbility* m_originalAbility;
	IntrinsicAbility* m_ability;

	typedef std::vector<StatMultiplier*> multiplierList_t;
	multiplierList_t m_multiplier;

	StatMultiplier* m_accuracy;
	StatMultiplier* m_evasion;

	statusEffectList_t m_statuses;

	MoveListEntry* m_lastMove;
	moveList_t m_move;
};

#endif