#ifndef __client_h_
#define __client_h_

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/unordered_map.hpp>
#include <iostream>
#include <vector>

#include "global.h"
#include "game.h"
#include "player.h"
#include "pusocket.h"
#include "creature.h"
#include "pokemon.h"
#include "items.h"
#include "tile.h"
#include "database.h"
#include "packet.h"
#include "logwriter.h"
#include "chat.h"

class Creature;
class Player;
class Tile;
class Pokemon;
class Item;

extern LogWriter g_log;
extern Chat g_chat;

using boost::asio::ip::tcp;

typedef boost::unordered_map<int,Item*> ItemMap;

class Client
{
public:
	Client(Player *p, PUSocket *m_socket);
	~Client();

	Player *player;
	PUSocket* socket;

	int lastActivity;
	int lastMessage;

	void MainThread();

	void sendLogin();
	void sendDisconnect();
	void sendPing();
	void sendMoney(int _goldz);
	void sendSpeedChange(const Creature *creature, int speed);
	void sendCreatureSay(Creature* creature, SpeakClasses type, std::string text);
	void sendCreatureSay(Creature* creature, SpeakClasses type, std::string text, uint16_t channelId, uint32_t time = 0);
	void sendCreatureTeleport(Creature *creature);
	void sendCreatureTurn(Creature *creature, Direction dir);
	void sendCreatureMove(const Creature *creature, Tile *fromTile, Tile *toTile);
	void sendMessage(MessageClasses mclass, const std::string message);
	void sendChannel(int channelID, std::string channelName, bool openChannel);
	void sendCancelMessage(const std::string msg);
	void sendCancelWalk(Tile *fromCylinder);
	void sendMap(Direction dir);
	void sendToChannel(Creature* creature, SpeakClasses type, std::string& text, uint16_t channelId, uint32_t time);
	void sendCreatureChangeOutfit(Creature *creature);

	void sendDialogueQuestions(DIALOGUE_TYPES type, std::string question, std::map<int, std::string> answers, int npc_id = 0);
	void sendDialogueClose();

	void sendMovePlayer(const Creature *c, Tile *fromTile, Tile *toTile);
	void sendWarpPlayer(Tile *toTile);
	void sendCancelMove();
	void sendAddCreature(const Creature *p);
	void sendRemoveCreature(const Creature *p);
	
	void sendPokemonData(PokemonList _pokemonParty);
	
	void sendItems(ItemMap items);
	void sendItemSlotUpdate(int _category, int _slot);
	void sendItemAdd(int _index);
	void sendItemRemove(int _index);
	void sendItemsShop(ItemMap items);
	void sendItemsShopCloseWindow();

	void sendMusicUpdate(SoundType_t _type, bool _fromStart, std::vector<int> playList);
	
	void sendFriendAdd(std::string _name, int online);
	void sendFriendRemove(std::string _name);

	void sendDataPacket(Packet packet, int isResend = 0);
private:
	boost::thread clientThread;

	//void sendIdentity();
	void sendTile(std::vector<Tile*> *list);	
	//void TestPacket(Packet packet);
	void sendItemAdd(int _category, int _slot);

	void receivedPacket(boost::shared_ptr<Packet> packet);
	void receiveLogout();
	void receiveRequestLoginpackets();
	void receiveMovePlayer(boost::shared_ptr<Packet> packet);
	void receiveTurnPlayer(boost::shared_ptr<Packet> packet);
	void receiveDefaultMessage(boost::shared_ptr<Packet> packet);
	void receiveRequestPrivateMessage(boost::shared_ptr<Packet> packet);
	void recievePlayerAction(boost::shared_ptr<Packet> packet);
	void recieveRefreshMap();
	void receiveWorldTileClick(boost::shared_ptr<Packet> packet);
	void receiveItemUse(boost::shared_ptr<Packet> packet);
	void receiveItemUsePokemon(boost::shared_ptr<Packet> packet);
	void receiveOutfitChange(boost::shared_ptr<Packet> packet);
	void receiveChangeItemOrder(boost::shared_ptr<Packet> packet);
	void receiveShopItemBuy(boost::shared_ptr<Packet> packet);
	void receiveDialogueAnswer(boost::shared_ptr<Packet> packet);
	void receiveChangePokemonOrder(boost::shared_ptr<Packet> packet);
	void receiveShopWindowClose();
	void receiveFriendAdd( std::string friendName );
	void receiveFriendRemove( std::string friendName );
	void receivePong();
};

#endif