#ifndef INC_BATTLETEAM_H_
#define INC_BATTLETEAM_H_

#include <vector>

#include "definitions.h"

class Creature;
class Pokemon;
class BattleField;

typedef std::vector<Pokemon*> pokemonList_t;

class BattleTeam
{
	typedef std::vector<Creature*> teamList_t;
	teamList_t m_team;

	bool m_locked;
	int m_active;
	uint8_t m_id;

public:
	BattleTeam() {};
	~BattleTeam() { m_team.clear();	};
	
	//************************************
	// Method:    isMember
	// FullName:  BattleTeam::isMember
	// Access:    public 
	// Returns:   int
	// Qualifier: Checks if c is a member of this team. If so it will return the slot otherwise -1
	// Parameter: Creature * c
	//************************************
	int isMember(Creature* c) {
		for(int i = 0; i < m_team.size(); ++i) {
			if(m_team.at(i) == c) return i;
		}
		return -1;
	};

	//************************************
	// Method:    addMember
	// FullName:  BattleTeam::addMember
	// Access:    public 
	// Returns:   void
	// Qualifier: Adds a new member to the team. It checks for duplicates
	// Parameter: Creature * c
	//************************************
	void addMember(Creature* c) {
		if(m_locked) return;
		if(isMember(c) == -1) m_team.push_back(c);
	};

	//************************************
	// Method:    attachField
	// FullName:  BattleTeam::attachField
	// Access:    public 
	// Returns:   void
	// Qualifier: Attach battlefield to all team members
	// Parameter: BattleField * field
	// Parameter: uint8_t team
	// Parameter: uint8_t position
	//************************************
	void attachField(BattleField* field, uint8_t team);

	//************************************
	// Method:    lockTeam
	// FullName:  BattleTeam::lockTeam
	// Access:    public 
	// Returns:   void
	// Qualifier: Once locked it's impossible to add new members. Should be called when a battle starts.
	//************************************
	void lockTeam() { 
		m_locked = true; 
	};

	//************************************
	// Method:    getActivePokemonList
	// FullName:  BattleTeam::getActivePokemonList
	// Access:    public 
	// Returns:   pokemonList_t
	// Qualifier: Returns active pokemon of all team members
	//************************************
	pokemonList_t getActivePokemonList();

	//************************************
	// Method:    getActivePokemon
	// FullName:  BattleTeam::getActivePokemon
	// Access:    public 
	// Returns:   Pokemon*
	// Qualifier: Returns the active pokemon of the active member
	//************************************
	Pokemon* getActivePokemon();

	//************************************
	// Method:    getActivePokemon
	// FullName:  BattleTeam::getActivePokemon
	// Access:    public 
	// Returns:   Pokemon*
	// Qualifier: Returns active pokemon of team member at idx, NULL if there is none
	// Parameter: int idx
	//************************************
	Pokemon* getActivePokemon(int idx);
};

#endif