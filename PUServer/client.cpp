#include "client.h"
#include "game.h"

#include <boost/foreach.hpp>

#ifdef __BUGTRAP__
#include <eh.h>
#include "BugTrap.h"
#endif

extern Game g_game;

Client::Client(Player *p, PUSocket *m_socket)
{
	socket = m_socket;
	player = p;
	lastActivity = (int)time(0);
	lastMessage = PUSYS_TIME();

	clientThread = boost::thread(boost::bind(&Client::MainThread, this));
}

Client::~Client()
{
	Global::updatePlayers(g_game.getCountPlayers());

	clientThread.interrupt();

	boost::system::error_code error;
	socket->socket.close(error);

	delete socket;
}

void Client::MainThread()
{
#ifdef __BUGTRAP__
	BT_SetTerminate();
#endif // __BUGTRAP__

	srand(time(0));
	for(;;)
	{	
		boost::this_thread::interruption_point();

		try 
		{
			boost::shared_ptr<Packet> packet(new Packet());

			boost::system::error_code error;
			boost::asio::read(socket->socket, 
				boost::asio::buffer(packet->getBuffer(), 2), 
				boost::asio::transfer_at_least(2), 
				error);

			if(error && boost::asio::error::would_block != error)
			{
				std::stringstream logStr;
				if(player) {
					logStr << "[LOGOUT] ID: " << player->getGUID() << " | IP: " << player->getIP() <<"\r\n";
				}
				else {
					logStr << "[LOGOUT] ID: UNKNOWN | IP: UNKNOWN\r\n";
				}

				g_log.WriteLine(logStr.str());

				break;
			}
			else if (error) {
				std::cout << "[ERROR] Client - " << error.message() << std::endl;
				continue;
			}

			boost::asio::read(socket->socket, 
				boost::asio::buffer(packet->getDataBuffer(), packet->getHeader()), 
				boost::asio::transfer_at_least(packet->getHeader()), 
				error);

			if(error) { // 0-byte header hax0r
				std::cout << "[ERROR] Client - " << error.message() << std::endl;
				break;
			}

			receivedPacket(packet);
			lastActivity = time(0);
		}
		catch(...)
		{
			break;
		}
	}

	if(player && player->playerState == PLS_RUNNING) {
		g_game.removeCreature(player);
	}

	return;
}

void Client::receivedPacket(boost::shared_ptr<Packet> packet)
{
	if (!player || packet->getMessageSize() <= 0){
		return;
	}
	unsigned char header = packet->ReadByte();

	switch(header)
	{
	case 0x00:
		receivePong();
		break;
	case 0x01:
		receiveRequestLoginpackets();
		break;
	case 0x04:
		//receiveWorldTileClick(packet);
		break;
	case 0x07:
		//receiveFriendAdd(packet->ReadString());
		break;
	case 0x08:
		//receiveFriendRemove(packet->ReadString());
		break;

	case 0x10:
		receiveDefaultMessage(packet);
		break;
	case 0x12:
		receiveRequestPrivateMessage(packet);
		break;

	case 0x20:
		receiveDialogueAnswer(packet);
		break;
	case 0x22:
		//receiveShopItemBuy(packet);
		break;
	case 0x23:
		//receiveShopItemSell(packet);
		break;
	case 0x24:
		//receiveShopWindowClose();
		break;

	case 0xA0:
		receiveOutfitChange(packet);
		break;
	case 0xAB:
		receiveLogout();
		break;

	case 0xB1:
		receiveMovePlayer(packet);
		break;
	case 0xB2:
		receiveTurnPlayer(packet);
		break;

	case 0xC4:
		recieveRefreshMap();
		break;
	case 0xC5:
		//player->sendItems();
		break;
	case 0xC6:
		//receiveChangeItemOrder(packet);
		break;
	case 0xC8:
		//receiveItemUse(packet);
		break;
	case 0xC9:
		//receiveItemUsePokemon(packet);
		break;

	case 0xD1:
		sendPokemonData(player->getPokemonParty());
		break;
	case 0xD2:
		//receiveChangePokemonOrder(packet);
		break;
	case 0xD3:
		player->setBattlePacket(packet);
		break;

	case 0xE0:
		//pokecenter->packetReader(packet);
		break;
	case 0xE1:
		//player->challengePlayer(packet->ReadString());
		break;
	case 0xE2:
		//player->challengeCancel(true);
		break;
	case 0xE3:
		//player->challengeHandle(packet->ReadByte());
		break;

	case 0x50:
		player->doAction();
		break;
	case 0x66://6
		//TestPacket(packet);
		break;
	default:
		break;
	}
}

void Client::receiveLogout()
{
	g_game.removeCreature(player);
}

void Client::receiveRequestLoginpackets()
{
	sendLogin();

	boost::this_thread::sleep(boost::posix_time::milliseconds(110)); // Otherwise the client will time-out, for some weird reason 

	Packet packet;
	packet.AddByte(0x01);
	packet.AddByte(5); // STATUS_READY

	sendDataPacket(packet);
}

void Client::receiveMovePlayer(boost::shared_ptr<Packet> packet)
{
	Direction dir = (Direction) packet->ReadInt();
	bool sendMap = (packet->ReadInt() == 1) ? true : false;

	g_game.playerMove(player, dir, sendMap);
}

void Client::receiveDefaultMessage(boost::shared_ptr<Packet> packet)
{
	// flood protection
	int timeStamp = PUSYS_TIME();
	if((timeStamp - lastMessage) < PUSERV_CHAT_SPAM)
		return;

	lastMessage = timeStamp;
	SpeakClasses type = (SpeakClasses)packet->ReadByte();
	uint16_t channelId = packet->ReadInt();
	std::string message = packet->ReadString();
	std::string receiver;

	if(g_game.playerSayCommand(player, type, message)){
		return;
	}

	g_game.playerSay(player, channelId, type, receiver, message);
}

void Client::receiveRequestPrivateMessage(boost::shared_ptr<Packet> packet)
{
	std::string name = packet->ReadString();
	if(Player *privPlayer = g_game.getPlayerByName(name))
	{
		if(privPlayer == player) {
			return;
		}

		sendChannel(privPlayer->getID()+10,privPlayer->getName(), true);
	}
	else
	{
		sendCancelMessage("A player with this name is not online.");
	}
}

void Client::receiveDialogueAnswer(boost::shared_ptr<Packet> packet)
{
	int option = packet->ReadInt();

	if(player->getBattle()) 
	{
		player->setBattleDialogueAnswer(option);
	}
	else
	{
		Creature *c = g_game.getCreatureByID(player->getInteractingNPC());
		Npc *npc = NULL;
		if(npc = c->getNpc())
		{
			npc->setAnswer(player, option);
		}
	}
}

void Client::receiveOutfitChange(boost::shared_ptr<Packet> packet)
{
	// Outfit
	CreatureOutfit outfit;
	outfit.upper_style = packet->ReadByte();
	outfit.upper_colour = packet->ReadLong();
	outfit.nek_style = packet->ReadByte();
	outfit.nek_colour = packet->ReadLong();
	outfit.head_style = packet->ReadByte();
	outfit.head_colour = packet->ReadLong();
	outfit.feet_style = packet->ReadByte();
	outfit.feet_colour = packet->ReadLong();
	outfit.lower_style = packet->ReadByte();
	outfit.lower_colour = packet->ReadLong();

	g_game.playerChangeOutfit(player, outfit);
}

void Client::recieveRefreshMap()
{
	sendMap(DIR_NULL);

	Packet packet;
	packet.AddByte(0x03);
	packet.AddByte(1);

	sendDataPacket(packet);
}

void Client::receiveWorldTileClick(boost::shared_ptr<Packet> packet)
{
	short tileX = packet->ReadInt();
	short tileY = packet->ReadInt();

	Tile *checkTile = g_game.getTile(tileX, tileY, player->getZ());

	if(!checkTile)
		return;

	Creature *firstCreature = checkTile->getFirstCreature();
	if(firstCreature)
	{
		// Send creature info
		Packet packet;
		packet.AddByte(0x04);
		packet.AddLong(firstCreature->getID());
		packet.AddString(firstCreature->getName());
		packet.AddInt(firstCreature->getLevel());

		if(firstCreature->getPlayer()) {
			packet.AddByte(1); // PM
			packet.AddByte(1); // Battle
		}
		else {
			packet.AddByte(0); // PM
			packet.AddByte(0); // Battle
		}

		sendDataPacket(packet);
	}
}

void Client::receiveTurnPlayer( boost::shared_ptr<Packet> packet )
{
	Direction dir = (Direction)packet->ReadInt();
	g_game.playerTurn(player, dir);
}

void Client::receiveChangePokemonOrder( boost::shared_ptr<Packet> packet )
{
	int oldSlot = packet->ReadInt();
	int newSlot = packet->ReadInt();

	player->changePokemonSlot(oldSlot, newSlot);
}


void Client::receiveChangeItemOrder( boost::shared_ptr<Packet> packet )
{
	int category = packet->ReadInt();
	int oldSlot = packet->ReadInt();
	int newSlot = packet->ReadInt();

	player->getBackPack()->switchItem(category, oldSlot, newSlot);
}

void Client::receiveItemUse( boost::shared_ptr<Packet> packet )
{
	int category = packet->ReadInt();
	int slot = packet->ReadInt();
	int index = Items::createIndex(slot, category);

	player->useItem(index);
}

void Client::receiveItemUsePokemon( boost::shared_ptr<Packet> packet )
{
	int atk_index = packet->ReadInt();
	if(atk_index == -1) {
		player->setLastUsedItemIndex(-1);
	}
	else
	{
		int attack_id = (char)(atk_index >> 8);
		int pokemon_id = (char)atk_index;

		player->useItemPokemon(pokemon_id, attack_id);
	}
}

void Client::receiveShopItemBuy( boost::shared_ptr<Packet> packet )
{
	int category = packet->ReadInt();
	int slot = packet->ReadInt();
	int index = Items::createIndex(slot, category);
	int count = packet->ReadInt();

	player->shopBuyItem(index, count);
}

void Client::receiveShopWindowClose()
{
	player->closeShopWindow();
}

void Client::receiveFriendAdd( std::string friendName )
{
	player->addFriend(friendName);
}

void Client::receiveFriendRemove( std::string friendName )
{
	player->removeFriend(friendName);
}
//#########################
// Send commands
//#########################
void Client::sendDataPacket(Packet packet, int isResend /* = 0 */)
{
	if(!player) {
		return;
	}

	boost::system::error_code error;
	packet.Send(socket->socket, error);

	/*
	if(error) 
	{
		isResend++;
		if(error == boost::asio::error::not_connected) {
			std::cout << "[ASIO] [" << isResend << "] Lost connection - " << player->getName() << endl;
		}
		else if(error == boost::asio::error::broken_pipe) {
			std::cout << "[ASIO] [" << isResend << "] Broken Pipe - " << player->getName() << endl;
		}
		else {
			std::cout << "[ASIO] [" << isResend << "] Other error - " << player->getName() << " - " << error.message() << endl;
		}

		if(isResend < 3) {
			sendDataPacket(packet, isResend);
		}
		else {
			std::cout << player->getName() <<  " - Player removed due lost connection or broken pipe" << endl;
			g_game.removeCreature(player);
		}
	}*/
}

void Client::sendPing()
{
	Packet packet;
	packet.AddByte(0x00);
	sendDataPacket(packet);
}

void Client::sendLogin()
{
	//eigen identiteit
	Packet packet;
	packet.AddByte(0xAA);
	packet.AddLong(player->getGUID());
	packet.AddString(player->getName());
	packet.AddInt(player->getX());
	packet.AddInt(player->getY());
	packet.AddInt(player->getDirection());
	packet.AddLong(player->getMoney());

	// Outfit
	CreatureOutfit outfit = player->getOutfit();
	packet.AddByte(outfit.upper_style);
	packet.AddLong(outfit.upper_colour);
	packet.AddByte(outfit.nek_style);
	packet.AddLong(outfit.nek_colour);
	packet.AddByte(outfit.head_style);
	packet.AddLong(outfit.head_colour);
	packet.AddByte(outfit.feet_style);
	packet.AddLong(outfit.feet_colour);
	packet.AddByte(outfit.lower_style);
	packet.AddLong(outfit.lower_colour);

	sendDataPacket(packet);

	// Items
	//player->sendItems();

	//de map sturen
	sendMap(DIR_NULL);
}

void Client::sendDisconnect()
{
	Packet packet;
	packet.AddByte(0x02);
	
	sendDataPacket(packet);
}

void Client::sendMap(Direction dir)
{
	int xT = 1, xM = VIEWPORT_X;
	int yT = 1, yM = VIEWPORT_Y;

	if(dir != NULL)
	{
		switch(dir)
		{
		case DIR_NORTH:
			xT = 1;
			xM = VIEWPORT_X;
			yT = 1;
			yM = 1;
			break;

		case DIR_EAST:
			xT = VIEWPORT_X;
			xM = VIEWPORT_X; 
			yT = 1;
			yM = VIEWPORT_Y;
			break;

		case DIR_SOUTH:
			xT = 1;
			xM = VIEWPORT_X;
			yT = VIEWPORT_Y;  
			yM = VIEWPORT_Y;
			break;

		case DIR_WEST:
			xT = 1; 
			xM = 1; 
			yT = 1;
			yM = VIEWPORT_Y;
			break;		
		}
	}

	std::vector<Tile*> list;
	int playerX = (player->getX() - ((VIEWPORT_X - 1) / 2));
	int playerY = (player->getY() - ((VIEWPORT_Y - 1) / 2));
	int playerZ = player->getZ();

	for(int x = xT; x <= xM; x++)
	{
		if(dir == NULL)
			list.clear();

		for(int y = yT; y <= yM; y++)
		{
			int mapx, mapy;
			mapx = playerX + x;
			mapy = playerY + y;

			Tile *tile = g_game.getTile(mapx, mapy, playerZ);
			if(tile) {		
				list.push_back(tile);
			}
		}

		if(dir == NULL && !list.empty()) {
			sendTile(&list);
		}
	}

	if(dir != NULL && !list.empty())
	{
		sendTile(&list);
	}
}

void Client::sendSpeedChange(const Creature *creature, int speed)
{
	Packet packet;
	packet.AddByte(0xA1);
	packet.AddLong(creature->getGUID());
	packet.AddInt(speed);

	sendDataPacket(packet);
}

void Client::sendTile(std::vector<Tile*> *list)
{
	if(list->empty())
		return;

	Packet packet;
	packet.AddByte(0xC1);
	packet.AddInt(list->size());

	for(std::vector<Tile*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		Tile *tile = (*it);

		// Load tile objects
		int tileObjects = 0;
		if(tile->getObject(0) != NULL) { tileObjects++; }
		if(tile->getObject(1) != NULL) { tileObjects++; }
		if(tile->getObject(2) != NULL) { tileObjects++; }

		packet.AddInt(tile->getX());
		packet.AddInt(tile->getY());
		packet.AddInt(tileObjects);

		for(int i = 0; i <= 2; i++)
		{
			if(tile->getObject(i) == NULL) { continue; }

			long tileID = tile->getObject(i)->getTileID();
			packet.AddInt(tile->getObject(i)->getZ());
			packet.AddLong(tileID);
			packet.AddInt(tile->getObject(i)->getMovement());	
		}

		packet.AddInt(tile->getTown());
		packet.AddString(tile->getTownName());
	}

	sendDataPacket(packet);	
}

void Client::sendCreatureSay(Creature *creature, SpeakClasses type, std::string text)
{
	sendCreatureSay(creature, type, text, 0);
}

void Client::sendCreatureSay(Creature* creature, SpeakClasses type, std::string text, uint16_t channelId, uint32_t time /*= 0*/)
{
	Packet packet;
	packet.AddByte(0x10);

	//Add name to packet
	if(creature == NULL) {
		packet.AddString("Server"); // Anon
	}
	else if(type == SPEAK_BROADCAST) {
		packet.AddString("Gamemaster");
	}
	else {
		packet.AddString(creature->getName());
	}

	packet.AddByte(type);
	packet.AddInt(channelId);
	packet.AddString(text);
	
	sendDataPacket(packet);
}

void Client::sendToChannel(Creature* creature, SpeakClasses type, std::string& text, uint16_t channelId, uint32_t time)
{
	sendCreatureSay(creature, type, text, channelId, time);
}

void Client::sendChannel(int channelID, std::string channelName, bool openChannel)
{
	Packet packet;
	packet.AddByte(0x12);
	packet.AddInt(channelID);
	packet.AddString(channelName);
	packet.AddByte(openChannel ? 1 : 0);
	sendDataPacket(packet);
}

void Client::sendCreatureTeleport(Creature *creature)
{
	Packet packet;
	packet.AddByte(0xB3);
	packet.AddInt(creature->getX());
	packet.AddInt(creature->getY());

	sendDataPacket(packet);
}

void Client::sendCreatureTurn(Creature *creature, Direction dir)
{
	Packet packet;
	packet.AddByte(0xB4);
	packet.AddLong(creature->getGUID());
	packet.AddInt(dir);

	sendDataPacket(packet);
}

void Client::sendMessage(MessageClasses mclass, const std::string message)
{
	Packet packet;
	packet.AddByte(0x11); 
	packet.AddByte(mclass);
	packet.AddString(message);
	packet.AddInt(1000);

	sendDataPacket(packet);
}

void Client::sendCancelMessage(const std::string msg)
{
	sendMessage(MSG_STATUS_SMALL, msg);
}

void Client::sendCreatureMove(const Creature *creature, Tile *fromTile, Tile *toTile)
{
	if(Creature *p = const_cast<Creature*>(creature))
	{
		Packet packet;
		packet.AddByte(0xB1);
		packet.AddLong(p->getGUID());
		packet.AddInt(fromTile->getX());
		packet.AddInt(fromTile->getY());
		packet.AddInt(toTile->getX());
		packet.AddInt(toTile->getY());
		
		sendDataPacket(packet);
	}
}

void Client::sendCancelWalk(Tile *fromCylinder)
{
	/*Packet packet;
	packet.AddByte(0xB2);

	sendDataPacket(packet);*/

	sendCreatureMove(player, fromCylinder, fromCylinder);
}


void Client::sendWarpPlayer(Tile *toTile)
{
	Packet packet;
	packet.AddByte(0xB3);

	packet.AddInt(toTile->getX());
	packet.AddInt(toTile->getY());

	sendDataPacket(packet);
}


void Client::sendAddCreature(const Creature *creature)
{
	if(creature == player) {
		return;
	}

	if(Creature *p = const_cast<Creature*>(creature))
	{
		Packet packet;

		if(p->getCreatureType() == POKEMON)
		{
			packet.AddByte(0xC9);
			packet.AddLong(p->getGUID());
			packet.AddInt(p->getPokemonP()->getPokemonRealID());
			packet.AddInt(p->getX());
			packet.AddInt(p->getY());
			packet.AddInt(p->getDirection());
		}
		else
		{
			packet.AddByte(0xC2);
			packet.AddLong(p->getGUID());
			packet.AddString(p->getName());
			packet.AddInt(p->getX());
			packet.AddInt(p->getY());
			packet.AddInt(p->getDirection());
			// Outfit
			CreatureOutfit outfit = p->getOutfit();
			packet.AddByte(outfit.upper_style);
			packet.AddLong(outfit.upper_colour);
			packet.AddByte(outfit.nek_style);
			packet.AddLong(outfit.nek_colour);
			packet.AddByte(outfit.head_style);
			packet.AddLong(outfit.head_colour);
			packet.AddByte(outfit.feet_style);
			packet.AddLong(outfit.feet_colour);
			packet.AddByte(outfit.lower_style);
			packet.AddLong(outfit.lower_colour);
		}
		sendDataPacket(packet);
	}
}

void Client::sendRemoveCreature(const Creature *creature)
{
	if(creature == player) {
		return;
	}

	if(Creature *p = const_cast<Creature*>(creature))
	{
		Packet packet;
		packet.AddByte(0xC3);
		packet.AddLong(p->getGUID());
		
		sendDataPacket(packet);
	}
}

void Client::sendPokemonData(PokemonList PokemonParty)
{
	Packet packet;
	packet.AddByte(0xD1);
	packet.AddInt(PokemonParty.size());

	BOOST_FOREACH(PokemonList::value_type it, PokemonParty)
	{
		if(it.second)
		{
			Pokemon *pokemon = it.second;
			long TotalExp = StatCalculator::Calculate_LevelEXP((pokemon->getPokemonLvl()+1));

			packet.AddInt(it.first); // Slot
			packet.AddLong(pokemon->getGUID());
			packet.AddInt(pokemon->getPokemonRealID()); // Pokedex ID
			packet.AddString(pokemon->getName());
			packet.AddInt(pokemon->getPokemonLvl());
			packet.AddInt(pokemon->getPokemonHP_Total()); // Total HP
			packet.AddInt(pokemon->getPokemonHP_Left()); // HP Left
			
			packet.AddInt(pokemon->getExpPercent());
			packet.AddLong(pokemon->getCurrentExp());
			packet.AddLong(TotalExp);
			
			packet.AddString(pokemon->getType1());
			packet.AddString(pokemon->getType2());
			packet.AddString(pokemon->getFlavor());
			packet.AddInt(pokemon->getGender());

			packet.AddInt(pokemon->stat_at);
			packet.AddInt(pokemon->stat_de);
			packet.AddInt(pokemon->stat_sa);
			packet.AddInt(pokemon->stat_sd);
			packet.AddInt(pokemon->stat_sp);


			packet.AddInt(pokemon->getAttackCount());

			for(int a = 0; a < pokemon->getAttackCount(); a++)
			{
				Attack *attack = pokemon->getAttack(a);
				string AttackName = attack->getAttackName();
				string AttackDesc = attack->getAttackDesc();
				string AttackType = attack->getAttackType();
				int UsageLeft = attack->getUsageLeft();
				int UsageTotal = attack->getUsageTotal();

				int power = attack->getAttackPower();
				int accuracy = attack->getAttackAccuracy();
				std::string categorie = attack->getAttackClass();
				std::string target = attack->getTarget();
				std::string type = attack->getAttackType();
				std::string contact = attack->getContact();

				packet.AddInt(a); // Attack Slot
				packet.AddString(AttackName);
				packet.AddString(AttackDesc);
				packet.AddString(AttackType);
				packet.AddInt(UsageLeft);
				packet.AddInt(UsageTotal);

				packet.AddInt(power);
				packet.AddInt(accuracy);
				packet.AddString(categorie);
				packet.AddString(target);
				packet.AddString(contact);
			}
		}
	}

	sendDataPacket(packet);
}

void Client::sendDialogueClose()
{
	Packet packet;
	packet.AddByte(0x20);
	packet.AddByte(DIALOGUE_CLOSE);

	sendDataPacket(packet);
}

void Client::sendDialogueQuestions(DIALOGUE_TYPES type, std::string question, std::map<int, std::string> answers, int npc_id /*= 0*/)
{
	Packet packet;
	packet.AddByte(0x20);
	packet.AddByte(type);
	
	if(type == DIALOGEU_NPC || type == DIALOGUE_NPCTEXT) {
		packet.AddInt(npc_id);
	}

	if(type != DIALOGUE_OPTIONS) {
		packet.AddString(question.c_str());
	}

	if(type == DIALOGUE_NPCTEXT) {
		sendDataPacket(packet);
		return;
	}

	packet.AddByte(answers.size());

	for(std::map<int, std::string>::iterator it = answers.begin(); it != answers.end(); ++it)
	{
		packet.AddInt((*it).first);
		packet.AddString((*it).second.c_str());
	}

	sendDataPacket(packet);
}

void Client::sendItems( ItemMap items )
{
	if(items.size() <= 0) {
		return;
	}

	Packet packet;
	packet.AddByte(0xC5);
	packet.AddInt(items.size());

	BOOST_FOREACH(ItemMap::value_type it, items)
	{
		Item *tmp = it.second;
		int slot = (char)it.first;

		packet.AddInt(tmp->getCategoryID());
		packet.AddInt(slot);
		packet.AddInt(tmp->getID());
		packet.AddString(tmp->getName());
		packet.AddString(tmp->getDescription());
		packet.AddInt(tmp->getCount());
	}

	sendDataPacket(packet);
}

void Client::sendItemsShop(ItemMap items)
{
	Packet packet;
	packet.AddByte(0x21);
	packet.AddInt(items.size());

	BOOST_FOREACH(ItemMap::value_type it, items)
	{
		Item *tmp = it.second;
		int slot = (char)it.first;

		packet.AddInt(tmp->getCategoryID());
		packet.AddInt(slot);
		packet.AddInt(tmp->getID());
		packet.AddString(tmp->getName());
		packet.AddString(tmp->getDescription());
		packet.AddInt(tmp->getCost());
	}

	sendDataPacket(packet);
}

void Client::sendItemSlotUpdate( int _category, int _slot )
{
	int index = Items::createIndex(_slot, _category);
	Item *item = player->getBackPack()->getItem(index);
	if(!item) {
		return;
	}

	Packet itemSlot;
	itemSlot.AddByte(0xC6);
	itemSlot.AddInt(index);
	if(item) {
		itemSlot.AddInt(item->getID());
		itemSlot.AddInt(item->getCount());
	}
	else {
		itemSlot.AddInt(0);
		itemSlot.AddInt(0);
	}
	sendDataPacket(itemSlot);
}

void Client::sendItemAdd( int _index )
{
	int cat = (char)(_index >> 8);
	int slot = (char)_index;

	sendItemAdd(cat, slot);
}

void Client::sendItemAdd(int _category, int _slot)
{
	Item *item = player->getBackPack()->getItem(_category, _slot);
	if(!item) {
		return;
	}

	Packet packet;
	packet.AddByte(0xC7);
	packet.AddInt(_category);
	packet.AddInt(_slot);
	packet.AddInt(item->getID());
	packet.AddString(item->getName());
	packet.AddString(item->getDescription());
	packet.AddInt(item->getCount());

	sendDataPacket(packet);
}

void Client::sendItemRemove( int _index )
{
	Packet packet;
	packet.AddByte(0xC6);
	packet.AddInt(_index);
	sendDataPacket(packet);
}

void Client::sendMusicUpdate(SoundType_t _type, bool _fromStart, std::vector<int> playList)
{
	Packet packet;
	packet.AddByte(0x05);
	packet.AddInt(_type);
	if(_type != SOUND_LIST) {
		packet.AddByte((_fromStart) ? 1 : 0);
	}
	else {
		packet.AddInt(playList.size());
	}

	for(std::vector<int>::iterator it = playList.begin(); it != playList.end(); ++it)
	{
		int musicID = (int)*it;
		if(musicID == 0) {
			musicID = random_range(1,13);
		}

		packet.AddInt(musicID);
	}
	
	sendDataPacket(packet);
}

void Client::sendCreatureChangeOutfit(Creature *creature)
{
	Packet packet;
	packet.AddByte(0xA0);
	packet.AddLong(creature->getGUID());

	CreatureOutfit outfit = creature->getOutfit();
	packet.AddByte(outfit.upper_style);
	packet.AddLong(outfit.upper_colour);
	packet.AddByte(outfit.nek_style);
	packet.AddLong(outfit.nek_colour);
	packet.AddByte(outfit.head_style);
	packet.AddLong(outfit.head_colour);
	packet.AddByte(outfit.feet_style);
	packet.AddLong(outfit.feet_colour);
	packet.AddByte(outfit.lower_style);
	packet.AddLong(outfit.lower_colour);

	sendDataPacket(packet);
}

void Client::sendMoney( int _goldz )
{
	Packet packet;
	packet.AddByte(0x06);
	packet.AddLong(_goldz);
	sendDataPacket(packet);
}

void Client::sendItemsShopCloseWindow()
{
	Packet packet;
	packet.AddByte(0x24);
	sendDataPacket(packet);
}

void Client::sendFriendAdd( std::string _name, int online )
{
	Packet packet;
	packet.AddByte(0x07);
	packet.AddString(_name);
	packet.AddByte(online);
	sendDataPacket(packet);
}

void Client::sendFriendRemove( std::string _name )
{
	Packet packet;
	packet.AddByte(0x08);
	packet.AddString(_name);
	sendDataPacket(packet);
}

void Client::receivePong()
{
	player->resetPong();
}