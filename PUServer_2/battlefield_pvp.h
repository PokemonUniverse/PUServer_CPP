#ifndef __BATTLEFIELD_PVP_H_
#define __BATTLEFIELD_PVP_H_

#include "battlefield.h"

class BattleMechanics;
class Pokemon;
class StatusEffect;

class BattleField_PvP : BattleField
{
private:
	std::string m_description;

public:
	BattleField_PvP(BattleMechanics* mechanics) : BattleField(mechanics) {};
	BattleField_PvP(BattleMechanics* mechanics, battleTeamList_t teams) : BattleField(mechanics, teams) {};

	~BattleField_PvP();

	//************************************
	// Method:    getDescription
	// FullName:  BattleField_PvP::getDescription
	// Access:    public 
	// Returns:   std::string
	// Qualifier: Get description of this battle field
	//************************************
	std::string getDescription() { return m_description; }

	//************************************
	// Method:    informStatusApplied
	// FullName:  BattleField_PvP::informStatusApplied
	// Access:    private 
	// Returns:   void
	// Qualifier: Inform that a status effect was applied to a pokemon.
	// Parameter: Pokemon * p
	// Parameter: StatusEffect * eff
	//************************************
	void informStatusApplied(Pokemon *p, StatusEffect *eff) {};

	//************************************
	// Method:    informStatusRemoved
	// FullName:  BattleField_PvP::informStatusRemoved
	// Access:    private 
	// Returns:   void
	// Qualifier: Inform that a status effect was removed from a pokemon.
	// Parameter: Pokemon * p
	// Parameter: StatusEffect * eff
	//************************************
	void informStatusRemoved(Pokemon *p, StatusEffect *eff) {};

	//************************************
	// Method:    informUseMove
	// FullName:  BattleField_PvP::informUseMove
	// Access:    public 
	// Returns:   void
	// Qualifier: Inform that a pokemon used a move
	// Parameter: Pokemon * p
	// Parameter: std::string name
	//************************************
	void informUseMove(Pokemon *p, std::string name) {};

	//************************************
	// Method:    informPokemonFainted
	// FullName:  BattleField_PvP::informPokemonFainted
	// Access:    public 
	// Returns:   void
	// Qualifier: Inform that a pokemon fainted
	// Parameter: Creature * trainer
	// Parameter: Pokemon * p
	//************************************
	void informPokemonFainted(Creature *trainer, Pokemon *p) {};

	//************************************
	// Method:    informSwitchInPokemon
	// FullName:  BattleField_PvP::informSwitchInPokemon
	// Access:    public 
	// Returns:   void
	// Qualifier: A pokemon was switched in
	// Parameter: Creature * trainer
	// Parameter: Pokemon * p
	//************************************
	void informSwitchInPokemon(Creature* trainer, Pokemon *p) {
		informSwitchInPokemon(trainer, p, false);
	}

	//************************************
	// Method:    informSwitchInPokemon
	// FullName:  BattleField_PvP::informSwitchInPokemon
	// Access:    public 
	// Returns:   void
	// Qualifier: A pokemon was switched in
	// Parameter: Creature * trainer
	// Parameter: Pokemon * p
	// Parameter: bool silent
	//************************************
	void informSwitchInPokemon(Creature* trainer, Pokemon *p, bool silent) {};

	//************************************
	// Method:    informPokemonHealthChanged
	// FullName:  BattleField_PvP::informPokemonHealthChanged
	// Access:    public 
	// Returns:   void
	// Qualifier: A pokemon's health was changed
	// Parameter: Pokemon * p
	// Parameter: int change
	//************************************
	void informPokemonHealthChanged(Pokemon *p, int change) {};

	//************************************
	// Method:    informVictory
	// FullName:  BattleField_PvP::informVictory
	// Access:    public 
	// Returns:   void
	// Qualifier: A player has won
	// Parameter: Creature * winner
	//************************************
	void informVictory(Creature* winner) {};

	//************************************
	// Method:    showMessage
	// FullName:  BattleField_PvP::showMessage
	// Access:    public 
	// Returns:   void
	// Qualifier: Narrate the battle
	// Parameter: std::string message
	//************************************
	void showMessage(std::string message) { showMessage(message, false); };
	void showMessage(std::string message, bool important) {};

	//************************************
	// Method:    refreshActivePokemon
	// FullName:  BattleField_PvP::refreshActivePokemon
	// Access:    public 
	// Returns:   void
	// Qualifier: Refresh all active pokemon. This function is used in conjunction with
	//			  MoveList.SubstituteEffect to display the proper sprite.
	//************************************
	void refreshActivePokemon() {};

	//************************************
	// Method:    refreshStats
	// FullName:  BattleField_PvP::refreshStats
	// Access:    public 
	// Returns:   void
	// Qualifier: Refresh all players' statistics
	//************************************
	void refreshStats() {};

	//************************************
	// Method:    refreshStats
	// FullName:  BattleField_PvP::refreshStats
	// Access:    public 
	// Returns:   void
	// Qualifier: Refresh a player's statistics
	// Parameter: Creature * trainer
	//************************************
	void refreshStats(Creature* trainer) {};

	//************************************
	// Method:    requestAndWaitForSwitch
	// FullName:  BattleField_PvP::requestAndWaitForSwitch
	// Access:    public 
	// Returns:   void
	// Qualifier: Wait for a player to switch pokemon.
	//			  This method should be called *only* from the dispatch thread.
	// Parameter: Creature * trainer
	//************************************
	void requestAndWaitForSwitch(Creature* trainer) {};

protected:
	//************************************
	// Method:    requestPokemonReplacement
	// FullName:  BattleField_PvP::requestPokemonReplacement
	// Access:    protected 
	// Returns:   void
	// Qualifier: Obtain a replacement pokemon for the trainer identified by the parameter.
	// Parameter: Creature * creature
	//************************************
	void requestPokemonReplacement(Creature* creature) {};

private:
	//************************************
	// Method:    updateStatus
	// FullName:  BattleField_PvP::updateStatus
	// Access:    private 
	// Returns:   void
	// Qualifier: Get a status update message.
	// Parameter: Pokemon * p
	// Parameter: StatusEffect * eff
	//************************************
	void updateStatus(Pokemon* p, StatusEffect* eff);

	//************************************
	// Method:    updateStatus
	// FullName:  BattleField_PvP::updateStatus
	// Access:    private 
	// Returns:   void
	// Qualifier: Send a message to all users conveying a status update.
	// Parameter: bool online
	// Parameter: std::string user
	//************************************
	void updateStatus(bool online, std::string user) {};

	//************************************
	// Method:    initialiseBattle
	// FullName:  BattleField_PvP::initialiseBattle
	// Access:    private 
	// Returns:   void
	// Qualifier: Initialise this battle by attaching this object to each of the pokemon
	//			  in the player's party as well as communicating the initial state of the
	//			  battle to each of the players.
	//************************************
	void initialiseBattle() {};

};
#endif