#ifndef __player_h_
#define __player_h_

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include <boost/asio.hpp>
#include <boost/unordered_map.hpp>
#include <boost/format.hpp>

#include "definitions.h"
#include "enums.h"
#include "creature.h"
#include "game.h"
#include "items.h"
#include "packet.h"
#include "position.h"
#include "database.h"
#include "logwriter.h"
#include "towns.h"

using boost::asio::ip::tcp;

class Creature;
class Player;
class PUSocket;
class Client;
class Npc;
class Item;
class Town;

extern LogWriter g_log;

typedef boost::shared_ptr<boost::asio::ip::tcp::socket>socket_pointer;
typedef boost::unordered_map<uint16_t, uint16_t> PlayerQuestMap;
typedef boost::unordered_map<int, Pokemon*> DepotPokemonMap;
typedef boost::unordered_map<std::string, int> FriendList;

enum PlayerLoadingStatus_t
{
	PLS_LOADING,
	PLS_RUNNING,
	PLS_EXIT
};

class Player : public Creature
{
public:
	Player(PUSocket *s, const std::string &_name);
	~Player();

	Client* client;
	Database* database;
	PlayerLoadingStatus_t playerState;

	virtual Player* getPlayer() { return this; }
	virtual const Player* getPlayer() const { return this; }

	PlayerTypes getCreatureType() const { return PLAYER; }

	void aprilFoolsHack();
	int generateColour();

	void onCreatureMove(const Creature *creature, Cylinder *fromCylinder, Cylinder *toCylinder);
	void onCreatureSay(const Creature* creature, SpeakClasses type, std::string text) { }

	void removeVisibleCreature(const Creature *creature);
	void addVisibleCreature(const Creature *creature);
	
	bool loadInfo(const std::string &_name);
	void loadFriends();

	void saveFriends();
	void saveInfo();
	void loadGroupData(int group_id);

	void loadPokemon(int _id);
	void addNewPokemonToParty(int slot, int uniquePokemonID);
	void addDepotPokemonToParty(int slot, int uniquePokemonID);

	bool moveToPokeCenter();
	void kickPlayer();

	std::string getIP() const { return clientIP; }

	int getStatus() const { return userStatus; }
	int getAccessLevel() const { return userStatus; }

	void updatePokedex(int _pokemonID, int _cought = -1);

	// Towns & PokeCenters
	void setLastTown(int _townID);
	Town* getLastTown() { return lastTown; }
	Position getLastPokeCenter() { return lastPokeCenter; }

	// Flags
	void addFlag(PlayerFlags value) { groupFlags |= (1 << value); }
	void removeFlag(PlayerFlags value) { groupFlags &= ~(1 << value); }
	void setFlags(uint64_t flags){ groupFlags = flags;}
	bool hasFlag(PlayerFlags value) const { return (0 != (groupFlags & ((uint64_t)1 << value)));}

	// Money
	void addMoney(int _money);
	void removeMoney(int _money); 
	int getMoney() { return money; }

	// NPC Interaction
	uint32_t getInteractingNPC() { return interactingNPC; }
	void setInteractingNPC(int npcID) { interactingNPC = npcID; }

	void setShopOwner(Npc *npc) { shopOwner = npc; }
	Npc* getShopOwner() { return shopOwner; }
	void shopBuyItem(int _itemIndex, int _count);
	void openShopWindow(Npc* npc);
	void closeShopWindow();

	// Battle challenge
	void challengePlayer(std::string playerName);
	void challengeCancel(bool hazChallenged=false);
	void challengeHandle(int handle);
	bool challengeRadius();
	void setChallenged(bool challenge) { isChallenged = challenge; }
	bool getChallenged() { return isChallenged; }
	void setChallengedID(int playerID) { challengerPlayerID = playerID; }
	int getChallengedID() { return challengerPlayerID; }

	// Quests
	uint16_t getQuestProgress(int questid);
	void updateQuest(int questid, int progress);
	void saveQuests();
	void loadQuests();

	// Pokemon follow
	Pokemon *getFollower() { return follower; }
	void setFollower(Pokemon *p);

	// Items
	void loadItems();
	void saveItems();
	bool addItem(int _itemId, int _count = 1, int slot = -1);
	bool updateItem(int _itemIndex, int _count = 0);
	void removeItem(int _itemIndex);
	void useItem(int _itemIndex);
	void useItemPokemon(int _pokemon, int _attack);

	int getLastUsedItemIndex() { return lastUsedItemIndex; }
	void setLastUsedItemIndex(int _index) { lastUsedItemIndex = _index; }

	// Friends
	void addFriend(std::string _name);
	void removeFriend(std::string _name);
	bool hasFriend(std::string _name);
	void setOnline(std::string _name, int online);
	
	//////////////////////////////
	void setBattlePacket(boost::shared_ptr<Packet> packet);
	void savePokemon(int slotID = -1);

	// Protocol Functions
	void sendPacket(Packet packet);
	bool sendPing();
	void sendLogin();
	void sendDisconnect(); 
	void sendCreatureSay(Creature* creature, SpeakClasses type, std::string text, int channelId = 0);
	void sendCreatureTeleport(Creature *creature);
	void sendCreatureTurn(Creature* creature, Direction dir);
	void sendCreatureMove(const Creature* creature, Tile* fromTile, Tile* toTile);
	void sendCancelMessage(ReturnValue message) const;
	void sendCancelWalk(Tile *fromCylinder);
	void sendMap(Direction dir);
	void sendItems();
	void sendMusicUpdate(SoundType_t _type, bool _fromStart, std::vector<int> playList);
	void sendCreatureChangeOutfit(Creature *creature);
	
	void sendToChannel(Creature* creature, SpeakClasses type, std::string text, uint16_t channelId, uint32_t time = 0);
	void sendDialogueQuestions(DIALOGUE_TYPES type, std::string question, std::map<int, std::string> answers, int npc_id = 0);
	void sendDialogueClose();
	void setBattleDialogueAnswer(int option);

	void doAction();

	// Ping/Pong
	void resetPong() { pongFail = 0; }
	int getPong() { return pongFail; }
	void increasePong() { if(playerState == PLS_RUNNING) { pongFail++; } }

protected:
	// For PvP
	uint32_t challengerPlayerID;
	uint32_t interactingNPC;
	bool isChallenged;

	uint32_t userGroup;
	uint32_t userStatus;
	uint32_t money;

	uint64_t groupFlags;

	int lastUsedItemIndex;

	std::string clientIP;
	int pongFail;

	Npc *shopOwner;
	Pokemon *follower;

	Town *lastTown;
	Position lastPokeCenter;

	PlayerQuestMap quests;
	DepotPokemonMap depotPokemon;
	FriendList friends;
};

#endif