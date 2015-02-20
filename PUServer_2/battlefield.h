#ifndef __BATTLEFIELD_H_
#define __BATTLEFIELD_H_

#include <boost/unordered_map.hpp>
#include <vector>

#include "battlemechanics.h"
#include "battleturn.h"
#include "battleteam.h"
#include "creature.h"
#include "movelist.h"
#include "movelistentry.h"
#include "statuseffect.h"

class BattleMechanics;
class BattleMessage;
class Creature;
class Pokemon;
class MoveListEntry;
class StatusEffect;
class FieldEffect;
class PokemonType;
class PokemonParty;

typedef std::vector<BattleTeam*> battleTeamList_t;
typedef std::vector<Pokemon*> pokemonList_t;
typedef std::vector<FieldEffect*> fieldEffects_t;

class BattleField
{
public:
	//************************************
	// Method:    BattleField
	// FullName:  BattleField::BattleField
	// Access:    public 
	// Qualifier: Creates a new instance of BattleField
	// Parameter: BattleMechanics * mech
	//************************************
	BattleField(BattleMechanics* mech, battleTeamList_t teams) {
		m_active[0] = 0;
		m_active[1] = 0;

		m_naration = true;
		m_mechanics = mech;

		setPokemon(teams);
	};

	//************************************
	// Method:    BattleField
	// FullName:  BattleField::BattleField
	// Access:    public 
	// Qualifier: Creates a new instance of BattleField
	// Parameter: BattleMechanics * mech
	//************************************
	BattleField(BattleMechanics* mech) {
		m_active[0] = 0;
		m_active[1] = 0;

		m_naration = true;
		m_mechanics = mech;
	};

	//************************************
	// Method:    getQueuedTurns
	// FullName:  BattleField::getQueuedTurns
	// Access:    public 
	// Returns:   battleTurnList_t
	// Qualifier: Get queued moves
	//************************************
	virtual battleTurnList_t getQueuedTurns() = 0;

	//************************************
	// Method:    queueMove
	// FullName:  BattleField::queueMove
	// Access:    public 
	// Returns:   void
	// Qualifier: Queue a move
	// Parameter: int trainer
	// Parameter: BattleTurn move
	//************************************
	virtual void queueMove(int trainer, BattleTurn move) = 0;

	//************************************
	// Method:    showMessage
	// FullName:  BattleField::showMessage
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: Narrate the battle
	// Parameter: std::string message
	//************************************
	virtual void showMessage(std::string message) = 0;

	//************************************
	// Method:    refreshActivePokemon
	// FullName:  BattleField::refreshActivePokemon
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: Refresh all active pokemon. The exact meaning of this is for the implementation to decide
	//************************************
	virtual void refreshActivePokemon() = 0;

	//************************************
	// Method:    getOpponent
	// FullName:  BattleField::getOpponent
	// Access:    public 
	// Returns:   Creature*
	// Qualifier: Get the opponent of the creature (trainer) passed in.
	// Parameter: Creature * c
	//************************************
	virtual Creature* getOpponent(Creature *p) = 0;

	//************************************
	// Method:    informPokemonHealthChanged
	// FullName:  BattleField::informPokemonHealthChanged
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: Informs that a pokemon's health was changed
	// Parameter: Pokemon * p
	// Parameter: int change
	//************************************
	virtual void informPokemonHealthChanged(Pokemon *p, int change) = 0;

	//************************************
	// Method:    informStatusApplied
	// FullName:  BattleField::informStatusApplied
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: Inform that a status was applied to a pokemon
	// Parameter: Pokemon * p
	// Parameter: StatusEffect * eff
	//************************************
	virtual void informStatusApplied(Pokemon *p, StatusEffect *eff) = 0;

	//************************************
	// Method:    informStatusRemoved
	// FullName:  BattleField::informStatusRemoved
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: Inform that a status effect was removed from a pokemon
	// Parameter: Pokemon * p
	// Parameter: StatusEffect * eff
	//************************************
	virtual void informStatusRemoved(Pokemon *p, StatusEffect *eff) = 0;

	//************************************
	// Method:    informSwitchInPokemon
	// FullName:  BattleField::informSwitchInPokemon
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: Inform that a pokemon was switched in
	// Parameter: int trainer
	// Parameter: Pokemon * p
	//************************************
	virtual void informSwitchInPokemon(Creature* trainer, Pokemon *p) = 0;

	//************************************
	// Method:    informPokemonFainted
	// FullName:  BattleField::informPokemonFainted
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: Inform that a pokemon fainted
	// Parameter: int trainer
	// Parameter: int idx
	//************************************
	virtual void informPokemonFainted(Creature *trainer, Pokemon *p) = 0;

	//************************************
	// Method:    informUseMove
	// FullName:  BattleField::informUseMove
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: Inform taht a pokemon used a move
	// Parameter: Pokemon * p - the pokemon who used the move
	// Parameter: std::string name - name of the move that was used
	//************************************
	virtual void informUseMove(Pokemon *p, std::string name) = 0;

	//************************************
	// Method:    informVictory
	// FullName:  BattleField::informVictory
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: Inform that a player has won
	// Parameter: Creature * winner
	//************************************
	virtual void informVictory(Creature* winner) = 0;

	//************************************
	// Method:    requestAndWaitForSwitch
	// FullName:  BattleField::requestAndWaitForSwitch
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: Wait for a player to switch pokemon
	// Parameter: Creature * trainer
	//************************************
	virtual void requestAndWaitForSwitch(Creature* trainer) = 0;

	//************************************
	// Method:    checkBattleEnd
	// FullName:  BattleField::checkBattleEnd
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: Check if one party has won the battle and inform victory if so.
	// Parameter: Creature * trainer
	//************************************
	virtual void checkBattleEnd(Creature *trainer) = 0;

public:
	static MoveListEntry* getStruggle();

	//************************************
	// Method:    disconnect
	// FullName:  BattleField::disconnect
	// Access:    public 
	// Returns:   void
	// Qualifier: Call when creature disconnects from server
	// Parameter: Creature * creature
	//************************************
	void disconnect(Creature *creature) {};

	BattleMechanics* getMechanics() { return m_mechanics; };

	bool isNarrationEnabled() { return m_naration; };
	void setNarrationEnabled(bool enabled) { m_naration = enabled; };

	double getEffectiveness(PokemonType *move, PokemonType *pokemon, bool enemy);

	pokemonList_t getActivePokemon();
	Pokemon* getActivePokemon(Creature* creature);
	Pokemon* getActivePokemon(uint32_t idx);
	Pokemon* getActivePokemon(uint32_t team, uint32_t idx);

	//************************************
	// Method:    attachField
	// FullName:  BattleField::attachField
	// Access:    private 
	// Returns:   void
	// Qualifier: Attaches battle field to a specific team
	// Parameter: Creature * creature
	//************************************
	void attachField(BattleTeam* team, uint8_t idx) {
		team->attachField(this, idx);
	};

	//************************************
	// Method:    applyEffect
	// FullName:  BattleField::applyEffect
	// Access:    public 
	// Returns:   bool
	// Qualifier: Apply a new FieldEffect to this BattleField.
	//
	// Note that the actual effect passed in is not used -- it is cloned via
	// eff.getFieldCopy(), not eff.clone(), the latter of which should return
	// the same object.
	//
	// Parameter: FieldEffect * eff
	//************************************
	bool applyEffect(FieldEffect* eff);

	//************************************
	// Method:    removeEffect
	// FullName:  BattleField::removeEffect
	// Access:    public 
	// Returns:   void
	// Qualifier: Remove a FieldEffect from this field.
	// Parameter: FieldEffect * eff
	//************************************
	void removeEffect(FieldEffect* eff);

	//************************************
	// Method:    synchroniseFieldEffects
	// FullName:  BattleField::synchroniseFieldEffects
	// Access:    public 
	// Returns:   void
	// Qualifier: Synchronise FieldEffects
	//************************************
	void synchroniseFieldEffects();

	//************************************
	// Method:    getEffectByType
	// FullName:  BattleField::getEffectByType
	// Access:    public 
	// Returns:   FieldEffect*
	// Qualifier: Returns the first instance of an effect of a certain class that is applied to the BattleField.
	// Parameter: T * type
	//************************************
	template< class T >	FieldEffect* getEffectByType(){
		fieldEffects_t list = getEffectsByType<T>();
		if(list.size() == 0) {
			return NULL;
		}

		return (FieldEffect*)list.front();
	};

	//************************************
	// Method:    getEffectsByType
	// FullName:  BattleField::getEffectsByType
	// Access:    public 
	// Returns:   fieldEffects_t
	// Qualifier: Returns a list of the effects of a certain class that are applied to this BattleField.
	// Parameter: T * type
	//************************************
	template< class T >	fieldEffects_t getEffectsByType(){
		fieldEffects_t ret;
		fieldEffects_t::iterator it = m_effects.begin();
		for(; it != m_effects.end(); ++it)
		{
			FieldEffect *effect = (*it);
			if(effect == NULL || !effect->isActive()) {
				continue;
			}

			if(isAssignableFrom< T* >(effect)) {
				ret.push_back(effect);
			}
		}

		return ret;
	};

	//************************************
	// Method:    switchInPokemon
	// FullName:  BattleField::switchInPokemon
	// Access:    public 
	// Returns:   void
	// Qualifier: Switch in a pokemon and apply FieldEffects to it
	// Parameter: Creature * trainer
	// Parameter: Pokemon * p
	//************************************
	void switchInPokemon(Creature *trainer, Pokemon *p);

	//************************************
	// Method:    replaceFaintedPokemon
	// FullName:  BattleField::replaceFaintedPokemon
	// Access:    public 
	// Returns:   void
	// Qualifier: Replace a fainted pokemon
	// Parameter: Creature * trainer
	// Parameter: Pokemon * pokemon
	// Parameter: bool search
	//************************************
	void replaceFaintedPokemon(Creature *trainer, Pokemon *pokemon, bool search);

	//************************************
	// Method:    getAliveCount
	// FullName:  BattleField::getAliveCount
	// Access:    public 
	// Returns:   int
	// Qualifier: Return the number of party members in a given party who are alive.
	// Parameter: Creature * trainer
	//************************************
	int getAliveCount(Creature *trainer);

	//************************************
	// Method:    executeTurn
	// FullName:  BattleField::executeTurn
	// Access:    private 
	// Returns:   void
	// Qualifier: Execute a turn
	// Parameter: battleTurnList_t move
	//************************************
	void executeTurn(battleTurnList_t move);
	
protected:
	virtual ~BattleField() 
	{
		detachField();

		//TODO: Clear effects and such? Memory leak?

		m_creatures.clear();
		m_mechanics = NULL;
	};

	//************************************
	// Method:    attachField
	// FullName:  BattleField::attachField
	// Access:    private 
	// Returns:   void
	// Qualifier: Attaches battle field to all
	//************************************
	void attachField() 
	{
		for(int i = 0; i < m_creatures.size(); ++i) {
			attachField(m_creatures.at(i), i);
		}
	};

	//************************************
	// Method:    requestsMoves
	// FullName:  BattleField::requestsMoves
	// Access:    virtual protected 
	// Returns:   void
	// Qualifier: Request moves for next turn from all players
	//************************************
	virtual void requestsMoves() {};

	//************************************
	// Method:    requestsMove
	// FullName:  BattleField::requestsMove
	// Access:    virtual protected 
	// Returns:   void
	// Qualifier: Request move for next turn from 1 player
	// Parameter: Creature * creature
	//************************************
	virtual void requestsMove(Creature* creature) {};

	//************************************
	// Method:    requestPokemonReplacement
	// FullName:  BattleField::requestPokemonReplacement
	// Access:    virtual protected 
	// Returns:   void
	// Qualifier: Requests for a new active pokemon
	//************************************
	virtual void requestPokemonReplacement(Creature* creature) = 0;

	//************************************
	// Method:    canRun
	// FullName:  BattleField::canRun
	// Access:    virtual protected 
	// Returns:   bool
	// Qualifier: Returns true if the player can run from the battle
	//************************************
	virtual bool canRun() { return false; };

	//************************************
	// Method:    sendMessage
	// FullName:  BattleField::sendMessage
	// Access:    virtual protected 
	// Returns:   void
	// Qualifier: Send message to all human trainers
	// Parameter: BattleMessage * message
	//************************************
	virtual void sendMessage(BattleMessage* message);

	void setPokemon(battleTeamList_t teams);

	battleTeamList_t m_creatures;

	uint32_t m_participants;
	int m_active[2];

	fieldEffects_t m_effects;

private:

	//************************************
	// Method:    detachField
	// FullName:  BattleField::detachField
	// Access:    private 
	// Returns:   void
	// Qualifier: Detaches the battle field from all creatures
	//************************************
	void detachField() {};
	//************************************
	// Method:    detachField
	// FullName:  BattleField::detachField
	// Access:    private 
	// Returns:   void
	// Qualifier: Detaches the battle field from a specifiek team
	// Parameter: uint32_t teamid
	//************************************
	void detachField(uint32_t teamid) {};

	//************************************
	// Method:    sortBySpeed
	// FullName:  BattleField::sortBySpeed
	// Access:    private 
	// Returns:   void
	// Qualifier: Determine the order in which pokemon attack, etc.
	// Parameter: pokemonList_t * pokemon
	//************************************
	void sortBySpeed(pokemonList_t* pokemon);

	//************************************
	// Method:    applyEffects
	// FullName:  BattleField::applyEffects
	// Access:    private 
	// Returns:   void
	// Qualifier: Apply field effects to a pokemon.
	// Parameter: Pokemon * p
	//************************************
	void applyEffects(Pokemon *p);

	//************************************
	// Method:    executeTurn
	// FullName:  BattleField::executeTurn
	// Access:    private 
	// Returns:   void
	// Qualifier: Execute a turn
	// Parameter: BattleTurn * turn
	// Parameter: Pokemon * source
	// Parameter: Pokemon * target
	//************************************
	void executeTurn(BattleTurn *turn, Pokemon *source, Pokemon *target);

	//************************************
	// Method:    tickStatuses
	// FullName:  BattleField::tickStatuses
	// Access:    private 
	// Returns:   void
	// Qualifier: Tick status effects at the end of a turn
	// Parameter: pokemonList_t active
	//************************************
	void tickStatuses(pokemonList_t active);

	BattleMechanics *m_mechanics;
	bool m_naration;
};

#endif