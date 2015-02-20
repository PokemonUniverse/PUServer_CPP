#ifndef INC_IOPLAYER_H_
#define INC_IOPLAYER_H_

#include "definitions.h"

class Player;

class PlayerGroup
{
public:
	PlayerGroup(){};
	~PlayerGroup(){};

	std::string m_name;
	uint64_t m_flags;
	uint32_t m_access;
	uint32_t m_maxDepotItems;
	uint32_t m_maxVip;
};

/** Class responsible for loading players from database. */
class IOPlayer 
{
public:
	IOPlayer() {};
	~IOPlayer() {};

	static IOPlayer* instance()
	{
		static IOPlayer instance;
		return &instance;
	}

	//************************************
	// Method:    loadPlayer
	// FullName:  IOPlayer::loadPlayer
	// Access:    public 
	// Returns:   bool
	// Qualifier: Load a player, returns true if the player was successfully loaded
	// Parameter: Player * player - Player object to load to
	// Parameter: const std::string & name - Name of the player
	// Parameter: bool preload - if set to true only group, guid and account id will be loaded, default: false
	//************************************
	bool loadPlayer(Player* player, const std::string& name, bool preload = false);

	//************************************
	// Method:    savePlayer
	// FullName:  IOPlayer::savePlayer
	// Access:    public 
	// Returns:   bool
	// Qualifier: Save a player, returns true if the player was successfully saved
	// Parameter: Player * player - Player to be saved
	//************************************
	bool savePlayer(Player* player);
};
#endif