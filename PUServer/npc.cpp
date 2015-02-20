#include "npc.h"
#include "creature.h"
#include "player.h"
#include "luascript.h"
#include "pokecenter.h"
#include "pokemon.h"

#ifdef __BUGTRAP__
#include <eh.h>
#include "BugTrap.h"
#endif

extern Game g_game;

NpcScriptInterface* Npc::m_scriptInterface = NULL;

NpcManager::NpcManager()
{
	firstLoad = true;
}

NpcManager::~NpcManager()
{

}

bool NpcManager::reload()
{
	if(firstLoad)
	{
		std::stringstream npcQuery;
		npcQuery << "SELECT n.npcID, n.npcName, n.direction, n.posX, n.posY, n.posZ, n.script, n.talkRadius, n.walkInterval,"
				 << " o.head_style, o.nek_style, o.upper_style, o.lower_style, o.feet_style"
				 << " FROM npc n LEFT JOIN outfits o ON o.npcID=n.npcID";
		boost::scoped_ptr<Recordset> npcList(new Recordset(&g_db, npcQuery.str()));

		if(npcList->numOfRows() <= 0)
			return true;

		while(npcList->moveNext())
		{
			int npcID = npcList->getInt("npcID");
			std::string name = npcList->getString("npcName");
			int posX = npcList->getInt("posX");
			int posY = npcList->getInt("posY");
			int posZ = npcList->getInt("posZ");
			int talkRadius = npcList->getInt("talkRadius");
			int walkInterval = npcList->getInt("walkInterval");
			Direction dir = (Direction)npcList->getInt("direction");
			std::string script = npcList->getString("script");

			Npc *createNpc = new Npc(npcID, name);
			createNpc->setPosition(posX, posY, posZ);
			createNpc->setDirection(dir);
			createNpc->setBasePosition(createNpc->getPosition());
			createNpc->setScript(script);
			createNpc->setWalkInterval(walkInterval);
			createNpc->setTalkRadius(talkRadius);
			
			createNpc->loadItems(&g_db);
			// TODO: load Pokemon

			// Load Outfit
			CreatureOutfit outfit;
			int headKey = npcList->getInt("head_style");
			outfit.head_style = (char)(headKey >> 24);
			outfit.head_colour = headKey & 0xFFFFFF;
			int nekKey = npcList->getInt("nek_style");
			outfit.nek_style = (char)(nekKey >> 24);
			outfit.nek_colour = nekKey & 0xFFFFFF;
			int upperKey = npcList->getInt("upper_style");
			outfit.upper_style = (char)(upperKey >> 24);
			outfit.upper_colour = upperKey & 0xFFFFFF;
			int lowerKey = npcList->getInt("lower_style");
			outfit.lower_style = (char)(lowerKey >> 24);
			outfit.lower_colour = lowerKey & 0xFFFFFF;
			int feetKey = npcList->getInt("feet_style");
			outfit.feet_style = (char)(feetKey >> 24);
			outfit.feet_colour = feetKey & 0xFFFFFF;
			createNpc->setOutfit(outfit);

			if(!createNpc->load()) {
				std::cout << " [WARNING] Could not load NPC with name: " << name << endl;
			}
			else {
				// Add Npc to loaded list
				loadedNpc.push_back(createNpc);
			}

			// Add Npc to global list
			listNpc.push_back(createNpc);
		}
	}
	else
	{
		// Loop Npc List and reload everyone of them
		for(NpcList::iterator it = listNpc.begin(); it != listNpc.end(); ++it)
		{
			if(!(*it)->reload())
				std::cout << " [WARNING] Could not load NPC with name: " << (*it)->getName() << std::endl;
		}
	}

	return true;
}

void NpcManager::killThreads()
{
	for(NpcList::iterator it = loadedNpc.begin(); it != loadedNpc.end(); ++it)
	{
		g_game.removeCreature((*it));
		(*it)->stopThread();
	}
}

/*########################################*/
// NPC

Npc::Npc(int _id, std::string _name) : Creature()
{	
	m_datadir =	g_config.pString("dataFolder");
	m_script = "";
	
	loaded = false;
	threadRunning = false;
	isMarket = false;
	m_id = _id;
	name = _name;

	m_npcEventHandler = NULL;
	reset();

	g_game.addCreature(this);
}

Npc::~Npc()
{
	// Unload / stop thread / do something with allocated resources
	reset();
}

bool Npc::load()
{
	if(isLoaded()){
		return true;
	}

	if(!m_scriptInterface){
		m_scriptInterface = new NpcScriptInterface();
		m_scriptInterface->loadNpcLib(std::string(m_datadir + "/npc/scripts/lib/npc.lua"));
	}

	if(!m_script.empty())
	{
		m_npcEventHandler = new NpcScript(m_datadir + "/npc/scripts/" + m_script, this);
		if(!m_npcEventHandler->isLoaded())
		{
			return false;
		}
	}

	loaded = true;
	return true;
}

bool Npc::loadPokemon(Database *_db)
{
	std::string pokemonQuery = boost::str(boost::format("SELECT uniqueID, pokemonID, pokemonLevel FROM npc_pokemon WHERE npcID=%d") % getID());
	boost::scoped_ptr<Recordset> npcPokemon(new Recordset(_db, pokemonQuery));

	if(npcPokemon->numOfRows() > 0) 
	{
		int i = 0;
		while(npcPokemon->moveNext())
		{
			//pokemonParty.insert(std::make_pair<int, Pokemon*>(i, new Pokemon()))
		}
	}

	return true;
}

bool Npc::loadItems(Database *_db)
{
	std::stringstream marketQueryStream;
	marketQueryStream << "SELECT p.marktID, p.marktName, pi.itemID FROM pokemarkt p"
					  << " LEFT JOIN pokemarkt_items pi ON pi.marktID=p.marktID"
					  << " WHERE p.npcID=%d";

	std::string marketQuery = boost::str(boost::format(marketQueryStream.str()) % getID());
	boost::scoped_ptr<Recordset> marketList(new Recordset(_db, marketQuery));

	if(marketList->numOfRows() > 0)
	{
		depotItems = new Depot(getID(), 99999);

		while(marketList->moveNext())
		{
			int itemID = marketList->getInt("itemID");
			addItem(itemID);
		}

		isMarket = true;
		return true;
	}

	return false;	
}

bool Npc::addItem( int _itemId)
{
	bool ret = true;

	Item *item = g_game.getItem(_itemId);
	if(!item) {
		return false;
	}

	int category = item->getCategoryID();
	int slot = getItemDepot()->getCategoryFreeSlot(category);
	ret = getItemDepot()->addItem(_itemId, 1, slot);	

	return ret;
}

bool Npc::reload()
{
	reset();
	load();

	return isLoaded();
}

void Npc::reset()
{
	walkRadius = 2;
	walkTicks = 10;
	talkRadius = 2;
	idleTime = 0;
	idleInterval = 300;
	focusCreature = 0;

	loaded = false;

	delete m_npcEventHandler;
	m_npcEventHandler = NULL;

	queueList.clear();
}

void Npc::runThread()
{
	if(walkTicks > 0 && !threadRunning)
	{
		threadRunning = true;
		npcThread = boost::thread(boost::bind(&Npc::MainThread, this));
	}
}

void Npc::stopThread()
{
	if(threadRunning)
	{
		npcThread.interrupt();
		threadRunning = false;	
	}
}

bool Npc::canWalk(Direction dir)
{
	int y = getY();
	int x = getX();

	Position newPos = getPosition();
	switch(dir)
	{
	case DIR_SOUTH: newPos.y++; break;
	case DIR_NORTH: newPos.y--; break;
	case DIR_EAST: newPos.x++; break;
	case DIR_WEST: newPos.x--; break;
	default: return false;
	}

	if ((newPos.x >= basePosition.x - walkRadius) && 
		(newPos.x <= basePosition.x + walkRadius) && 
		(newPos.y >= basePosition.y - walkRadius) && 
		(newPos.y <= basePosition.y + walkRadius))
	{
		return true;
	}

	return false;
}

void Npc::onCreatureMove(const Creature *creature, Cylinder *fromCylinder, Cylinder *toCylinder)
{
	if(!creature && creature->isRemoved) {
		return;
	}

	Tile *toTile = toCylinder->getTile();
	Tile *fromTile = fromCylinder->getTile();

	if(fromTile == NULL || toTile == NULL) {
		return;
	}

	if(creature != this)
	{
		if(Player* player = const_cast<Player*>(creature->getPlayer()))
		{
			bool canSeeNewPos = canSee(toTile->getPosition());
			bool canSeeOldPos = canSee(fromTile->getPosition());

			if(canSeeNewPos && !canSeeOldPos) // Enter
			{
				if(!knowsCreature(creature)) {
					addVisibleCreature(creature);
				}
				if(!player->knowsCreature(this)) {
					player->addVisibleCreature(this);
				}

				onPlayerEnter(player);
			}
			else if(!canSeeNewPos && canSeeOldPos) // Leave
			{
				if(knowsCreature(creature)) {
					removeVisibleCreature(creature);
				}
				if(player->knowsCreature(this)) {
					player->removeVisibleCreature(this);
				}

				onPlayerLeave(player);
			}
		}
	}
}

void Npc::startDialogue(Player *player)
{
	if(!interactingPlayers.empty())
	{
		PlayerList::iterator find = std::find(interactingPlayers.begin(), interactingPlayers.end(), player);
		if(find != interactingPlayers.end())
		{
			return; // dialogue already started
		}
	}
	
	interactingPlayers.push_back(player);
	if(m_npcEventHandler)
	{
		const Position& creaturePos = player->getPosition();
		const Position& myPos = getPosition();
		int32_t dx = myPos.x - creaturePos.x;
		int32_t dy = myPos.y - creaturePos.y;

		Direction dir = DIR_SOUTH;
		float tan = 0;

		if(dx != 0){
			tan = dy/dx;
		}
		else{
			tan = 10;
		}

		if(std::abs(tan) < 1){
			if(dx > 0){
				dir = DIR_WEST;
			}
			else{
				dir = DIR_EAST;
			}
		}
		else{
			if(dy > 0){
				dir = DIR_NORTH;
			}
			else{
				dir = DIR_SOUTH;
			}
		}

		g_game.internalCreatureTurn(this, dir);
		m_npcEventHandler->onAnswer(player, 0); // answer 0 = greet/start dialogue
	}
}

void Npc::setAnswer(Player *player, int answer)
{
	if(m_npcEventHandler)
	{
		m_npcEventHandler->onAnswer(player, answer);
	}
}

void Npc::addVisibleCreature(const Creature *creature)
{
	if(creature->getCreatureType() == NPC) { // Npc's don't need to see each other
		return;
	}

	Creature::addVisibleCreature(creature);

	if(!isRunning() && visibleCreatures.size() > 0) {
		runThread();
	}
}

void Npc::onPlayerEnter(Player* player)
{
	if(!visibleCreatures.empty())
	{
		if(!isRunning()) 
		{ 
			runThread(); // Start thread
		}
	}
}

void Npc::onPlayerLeave(Player* player)
{
	removeInteractingPlayer(player);
	if(visibleCreatures.empty())
	{
		stopThread();
	}
}

void Npc::onPlayerTrade(Player* player, ShopEvent_t type, int callback, int itemId, int count, bool ignoreCapacity)
{
	if(type == SHOPEVENT_BUY)
	{
		Item *item = depotItems->getItem(itemId);
		if(!item)
			return;

		int price = item->getCost();
		int priceTotal = price * count;
		int playerMoney = player->getMoney();
		int playerBackpackCap = player->getBackPack()->getStorageCount();
		int playerBackpackMax = player->getBackPack()->getMaxItems();
		int playerBackpackRoom = playerBackpackMax - playerBackpackCap;

		if((playerMoney - priceTotal) < 0) {
			callback = 1;
		}
		else if(!ignoreCapacity && (playerBackpackRoom - count) < 0) {
			callback = 2;
		}
		else {
			// Remove moneyz and add item to backpack
			player->removeMoney(priceTotal);
			player->addItem(item->getID(), count);

			callback = 0;
		}
	}
	else if(type == SHOPEVENT_SELL)
	{

	}

	if(m_npcEventHandler) {
		m_npcEventHandler->onBuy(player, callback);
	}
}

void Npc::onPlayerCloseShopWindow( Player *player )
{
	removeInteractingPlayer(player);

	if(m_npcEventHandler) {
		m_npcEventHandler->onShopWindowClose(player);
	}
}

void Npc::MainThread()
{
#ifdef __BUGTRAP__
	BT_SetTerminate();
#endif // __BUGTRAP__

	int timeNow = time(0);

	lastWalk = timeNow - walkTicks;
	lastIdleInterval = timeNow;
	
	do
	{
		boost::this_thread::interruption_point();

		if(walkTicks < idleInterval) {
			boost::this_thread::sleep(boost::posix_time::milliseconds((walkTicks * 1000) ) );
		}
		else {
			boost::this_thread::sleep(boost::posix_time::milliseconds((idleInterval * 1000) ) );
		}

		if(interactingPlayers.empty())
		{
			timeNow = time(0);

			// Let's the NPC move
			int walkTime = timeNow - lastWalk;

			if(walkTicks <= 0 || walkTime < walkTicks)
				continue;

			if(focusCreature != 0)
				continue;

			// Generate random walk direction
			int direction = StatCalculator::randomNumber() % 4 + 1;

			Direction moveDir;
			switch(direction)
			{
				case 1: moveDir = DIR_SOUTH; break;
				case 2: moveDir = DIR_WEST; break;
				case 3: moveDir = DIR_NORTH; break;
				case 4: moveDir = DIR_EAST; break;

				default: moveDir = DIR_SOUTH; break;
			}

			if(canWalk(moveDir))
			{
				if(g_game.internalCreatureMove(this, moveDir) == RET_NOERROR) {
					lastWalk = timeNow;
				}
			}

		}

		// Extra sleep to prevent spawn spamming
		if(visibleCreatures.size() <= 0) 
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds((walkTicks * 1000) ) );
		}

	} 
	while(visibleCreatures.size() > 0);

	threadRunning = false;
}

void Npc::removeInteractingPlayer(Player *player)
{
	PlayerList::iterator find = std::find(interactingPlayers.begin(), interactingPlayers.end(), player);
	if(find != interactingPlayers.end())
	{
		interactingPlayers.erase(find);
	}
}

NpcScriptInterface* Npc::getScriptInterface()
{
	return m_scriptInterface;
}

NpcScriptInterface::NpcScriptInterface() :
	LuaScriptInterface("Npc interface")
{
	m_libLoaded = false;
	initState();
}


NpcScriptInterface::~NpcScriptInterface()
{
	//
}

bool NpcScriptInterface::initState()
{
	return LuaScriptInterface::initState();
}

bool NpcScriptInterface::closeState()
{
	m_libLoaded = false;
	return LuaScriptInterface::closeState();
}

bool NpcScriptInterface::loadNpcLib(std::string file)
{
	if(m_libLoaded)
		return true;

	if(loadFile(file) == -1)
	{
		std::cout << std::endl << "Warning: [NpcScriptInterface::loadNpcLib] Can not load " << file;
		return false;
	}

	m_libLoaded = true;
	return true;
}

void NpcScriptInterface::registerFunctions()
{
	LuaScriptInterface::registerFunctions();

	//npc exclusive functions
	lua_register(m_luaState, "selfSay", NpcScriptInterface::luaActionSay);
	lua_register(m_luaState, "sendDialogue", NpcScriptInterface::luaActionDialogue);
	lua_register(m_luaState, "endDialogue", NpcScriptInterface::luaActionEndDialogue);
	lua_register(m_luaState, "hideDialogue", NpcScriptInterface::luaActionHideDialogue);
	lua_register(m_luaState, "healParty", NpcScriptInterface::luaActionHealParty);
	lua_register(m_luaState, "getQuestProgress", NpcScriptInterface::luaActionGetQuestProgress);
	lua_register(m_luaState, "setQuestProgress", NpcScriptInterface::luaActionSetQuestProgress);
	lua_register(m_luaState, "havePokemon", NpcScriptInterface::luaActionCheckPokemon);
	lua_register(m_luaState, "getMoney", NpcScriptInterface::luaActionGetMoney);
	lua_register(m_luaState, "addMoney", NpcScriptInterface::luaActionAddMoney);
	lua_register(m_luaState, "removeMoney", NpcScriptInterface::luaActionRemoveMoney);
	lua_register(m_luaState, "addItem", NpcScriptInterface::luaActionAddItem);
	lua_register(m_luaState, "hasItem", NpcScriptInterface::luaActionCheckItem);
	lua_register(m_luaState, "removeItem", NpcScriptInterface::luaActionRemoveItem);
	lua_register(m_luaState, "startBattle", NpcScriptInterface::luaActionStartBattle);
	lua_register(m_luaState, "openShopWindow", NpcScriptInterface::luaOpenShopWindow);
	lua_register(m_luaState, "closeShopWindow", NpcScriptInterface::luaCloseShopWindow);
}

int NpcScriptInterface::luaActionSay(lua_State* L)
{
	uint32_t parameters = lua_gettop(L);
	uint32_t target = 0;

	if(parameters > 1)
	{
		target = popNumber(L);
	}

	std::string msg(popString(L));

	ScriptEnviroment* env = getScriptEnv();

	Npc* npc = env->getNpc();
	Player* focus = env->getPlayerByUID(target);

	if(npc)
	{
		g_game.internalCreatureSay(npc, SPEAK_SAY, msg);
	}

	return 0;
}

int NpcScriptInterface::luaActionDialogue(lua_State* L)
{
	uint32_t parameters = lua_gettop(L);
	ScriptEnviroment* env = getScriptEnv();

	int cid = popNumber(L);
	Player *player = env->getPlayerByUID(cid);
	Npc* npc = env->getNpc();

	std::string question = "";
	std::map<int, std::string> options;
	int numOptions = parameters - 2;
	if(numOptions > 0)
	{
		for(int i = 0; i < numOptions; i++)
		{
			std::string option(popString(L));

			int mid = option.find("-");
			if(mid != std::string::npos)
			{
				int id = atoi(option.substr(0, mid).c_str());
				option.erase(0,mid+1);
				std::string text = option;

				options.insert(std::pair<int, std::string>(id,text));
			}
		}
	}
	question = popString(L);

	player->setInteractingNPC(npc->getGUID());
	if(numOptions > 0)
	{
		player->sendDialogueQuestions(DIALOGEU_NPC, question, options, npc->getGUID());
	}
	else
	{
		player->sendDialogueQuestions(DIALOGUE_NPCTEXT, question, options, npc->getGUID());
	}

	return 0;
}

int NpcScriptInterface::luaActionEndDialogue(lua_State* L)
{
	uint32_t parameters = lua_gettop(L);
	uint32_t target = popNumber(L);

	ScriptEnviroment* env = getScriptEnv();

	Npc *npc = env->getNpc();
	Player *player = env->getPlayerByUID(target);

	if(npc && player)
	{
		npc->removeInteractingPlayer(player);
		player->sendDialogueClose();
	}

	return 0;
}

int NpcScriptInterface::luaActionHideDialogue(lua_State* L)
{
	uint32_t parameters = lua_gettop(L);
	uint32_t target = popNumber(L);

	ScriptEnviroment* env = getScriptEnv();

	Player *player = env->getPlayerByUID(target);

	if(player) {
		player->sendDialogueClose();
	}

	return 0;
}

int NpcScriptInterface::luaActionHealParty(lua_State* L)
{
	uint32_t parameters = lua_gettop(L);
	uint32_t target = popNumber(L);

	ScriptEnviroment* env = getScriptEnv();
	Player *player = env->getPlayerByUID(target);

	if(player) {
		PokeCenter::healActivePokemon(player);
	}

	return 0;
}

int NpcScriptInterface::luaActionGetQuestProgress(lua_State* L)
{
	uint32_t parameters = lua_gettop(L);
	uint32_t target = popNumber(L);
	uint32_t quest = popNumber(L);

	ScriptEnviroment* env = getScriptEnv();
	Player *player = env->getPlayerByUID(target);

	uint16_t progress = 0;
	if(player) {
		progress = player->getQuestProgress(quest);
	}

	lua_pushnumber(L, progress);

	return 1;
}

int NpcScriptInterface::luaActionSetQuestProgress(lua_State* L)
{
	uint32_t parameters = lua_gettop(L);
	uint16_t progress = popNumber(L);
	uint16_t quest = popNumber(L);
	uint32_t target = popNumber(L);

	ScriptEnviroment* env = getScriptEnv();
	Player *player = env->getPlayerByUID(target);

	if(player)
	{
		player->updateQuest(quest, progress);
	}

	return 1;
}

int NpcScriptInterface::luaActionCheckPokemon(lua_State* L)
{
	uint32_t parameters = lua_gettop(L);
	uint16_t pokemon = popNumber(L);
	uint32_t target = popNumber(L);

	ScriptEnviroment* env = getScriptEnv();
	Player *player = env->getPlayerByUID(target);

	bool found = false;
	if(player)
	{
		PokemonList party = player->getPokemonParty();
		PokemonList::iterator it = party.begin();
		PokemonList::iterator ite = party.end();
		for(; it != ite; it++)
		{
			if(it->second && it->second->getPokemonRealID() == pokemon)
				found = true;
		}
	}

	lua_pushboolean(L, found);

	return 1;
}

int NpcScriptInterface::luaActionGetMoney(lua_State* L)
{
	uint32_t parameters = lua_gettop(L);
	uint32_t target = popNumber(L);

	ScriptEnviroment* env = getScriptEnv();
	Player *player = env->getPlayerByUID(target);

	int money = 0;
	if(player)
	{
		money = player->getMoney();
	}

	lua_pushnumber(L, 0);

	return 1;
}

int NpcScriptInterface::luaActionAddMoney(lua_State* L)
{
	uint32_t parameters = lua_gettop(L);
	uint32_t money = popNumber(L);
	uint32_t target = popNumber(L);

	ScriptEnviroment* env = getScriptEnv();
	Player *player = env->getPlayerByUID(target);

	if(player)
	{
		player->addMoney(money);
	}

	return 0;
}

int NpcScriptInterface::luaActionRemoveMoney(lua_State* L)
{
	uint32_t parameters = lua_gettop(L);
	uint32_t money = popNumber(L);
	uint32_t target = popNumber(L);

	ScriptEnviroment* env = getScriptEnv();
	Player *player = env->getPlayerByUID(target);

	if(player)
	{
		player->removeMoney(money);
	}

	return 0;
}

int NpcScriptInterface::luaActionAddItem(lua_State* L)
{
	//addItem(cid, itemid, <optional> count <default: 1>)
	uint32_t parameters = lua_gettop(L);
	uint32_t count = 1;
	if(parameters > 2) {
		count = popNumber(L);
		if(count < 1) {
			count = 1;
		}
	}
	uint32_t itemID = popNumber(L);
	uint32_t target = popNumber(L);

	ScriptEnviroment* env = getScriptEnv();
	Player *player = env->getPlayerByUID(target);

	if(!player || !player->addItem(itemID, count))	{
		lua_pushnumber(L, 0);
	}
	else {
		lua_pushnumber(L, 1);
	}

	return 1;
}

int NpcScriptInterface::luaActionCheckItem(lua_State* L)
{
	//removeItem(cid, itemid)
	uint32_t parameters = lua_gettop(L);
	uint32_t itemID = popNumber(L);
	uint32_t target = popNumber(L);

	ScriptEnviroment* env = getScriptEnv();
	Player *player = env->getPlayerByUID(target);
	Item *item = g_game.getItem(itemID);

	if(!player || !item)	{
		lua_pushnumber(L, 0);
		return 1;
	}
	
	int slot = player->getBackPack()->searchItem(item->getCategoryID(), itemID);
	if(slot == -1) {
		lua_pushnumber(L, 0);
	}
	else {
		int itemIndex = Items::createIndex(slot, item->getCategoryID());
		lua_pushnumber(L, player->getBackPack()->getItem(itemIndex)->getCount());
	}

	return 1;
}

int NpcScriptInterface::luaActionRemoveItem(lua_State* L)
{
	//removeItem(cid, itemid, <optional> count <default: 1>)
	uint32_t parameters = lua_gettop(L);

	uint32_t count = 1;
	if(parameters > 2) {
		count = popNumber(L);
	}
	uint32_t itemID = popNumber(L);
	uint32_t target = popNumber(L);
	

	ScriptEnviroment* env = getScriptEnv();
	Player *player = env->getPlayerByUID(target);
	Item *item = g_game.getItem(itemID);

	if(!player || !item)	{
		lua_pushnumber(L, 0);
		return 1;
	}

	int slot = player->getBackPack()->searchItem(item->getCategoryID(), itemID);
	if(slot == -1) {
		lua_pushnumber(L, 0);
		return 1;
	}
	else 
	{
		int itemIndex = Items::createIndex(slot, item->getCategoryID());
		if(count == 0) { // remove all
			player->getBackPack()->removeItem(itemIndex);
		}
		else if(!player->getBackPack()->updateItem(itemIndex, count)) 
		{
			lua_pushnumber(L, 0);
			return 1;
		}
	}

	lua_pushnumber(L, 1);
	return 1;
}

int NpcScriptInterface::luaActionStartBattle(lua_State *L)
{
	//startBattle(cid)
	uint32_t parameters = lua_gettop(L);
	uint32_t target = popNumber(L);

	ScriptEnviroment* env = getScriptEnv();
	Npc *npc = env->getNpc();
	Player *player = env->getPlayerByUID(target);

	if(!player)	{
		lua_pushnumber(L, 0);
		return 1;
	}

	Battle *battle = new Battle(ONE_VS_ONE);
	battle->addPlayer(player, 0);
	battle->addPlayer(npc, 1);

	if(battle->startBattle()) {
		lua_pushnumber(L, 1);
	}
	else {
		lua_pushnumber(L, 0);
	}

	return 1;
}

int NpcScriptInterface::luaOpenShopWindow( lua_State* L )
{
	uint32_t parameters = lua_gettop(L);
	uint32_t target = popNumber(L);

	ScriptEnviroment* env = getScriptEnv();
	Npc *npc = env->getNpc();
	Player *player = env->getPlayerByUID(target);

	if(!player)	{
		return 0;
	}

	if(npc->isMarket) {
		player->openShopWindow(npc);
	}

	return 1;
}

int NpcScriptInterface::luaCloseShopWindow( lua_State* L )
{
	uint32_t parameters = lua_gettop(L);
	uint32_t target = popNumber(L);

	ScriptEnviroment* env = getScriptEnv();
	Npc *npc = env->getNpc();
	Player *player = env->getPlayerByUID(target);

	if(!player)	{
		return 0;
	}

	player->closeShopWindow();
	return 1;
}

void NpcScriptInterface::pushState(lua_State *L, NpcState* state)
{
	/*lua_newtable(L);

	setField(L, "n1", state->scriptVars.n1);
	setField(L, "n2", state->scriptVars.n2);
	setField(L, "n3", state->scriptVars.n3);

	setFieldBool(L, "b1", state->scriptVars.b1);
	setFieldBool(L, "b2", state->scriptVars.b2);
	setFieldBool(L, "b3", state->scriptVars.b3);

	setField(L, "s1", state->scriptVars.s1);
	setField(L, "s2", state->scriptVars.s2);
	setField(L, "s3", state->scriptVars.s3);*/
}

void NpcScriptInterface::popState(lua_State *L, NpcState* &state)
{
	/*state->scriptVars.n1 = getField(L, "n1");
	state->scriptVars.n2 = getField(L, "n2");
	state->scriptVars.n3 = getField(L, "n3");

	state->scriptVars.b1 = getFieldBool(L, "b1");
	state->scriptVars.b2 = getFieldBool(L, "b2");
	state->scriptVars.n3 = getFieldBool(L, "b3");

	state->scriptVars.s1 = getFieldString(L, "s1");
	state->scriptVars.s2 = getFieldString(L, "s2");
	state->scriptVars.s3 = getFieldString(L, "s3");*/
}

NpcEventsHandler::NpcEventsHandler(Npc* npc)
{
	m_npc = npc;
	m_loaded = false;
}

NpcEventsHandler::~NpcEventsHandler()
{
	//
}

bool NpcEventsHandler::isLoaded()
{
	return m_loaded;
}


NpcScript::NpcScript(std::string file, Npc* npc) 
	: NpcEventsHandler(npc)
{
	m_scriptInterface = npc->getScriptInterface();

	if(m_scriptInterface->loadFile(file, npc) == -1)
	{
		std::cout << "Warning: [NpcScript::NpcScript] Can not load script. " << file << std::endl;
		std::cout << m_scriptInterface->getLastLuaError() << std::endl;
		m_loaded = false;
		return;
	}

	m_onAnswer = m_scriptInterface->getEvent("onAnswer");
	m_onBuy = m_scriptInterface->getEvent("onBuy");
	m_onShopWindowClose = m_scriptInterface->getEvent("onShopWindowClose");

	m_loaded = true;
}

NpcScript::~NpcScript()
{
	//
}

void NpcScript::onAnswer(const Creature* creature, int answer)
{
	if(m_onAnswer == -1){
		return;
	}

	if(m_scriptInterface->reserveScriptEnv())
	{
		ScriptEnviroment* env = m_scriptInterface->getScriptEnv();

		env->setScriptId(m_onAnswer, m_scriptInterface);
		env->setNpc(m_npc);

		uint32_t cid = creature->getGUID();

		lua_State* L = m_scriptInterface->getLuaState();
		m_scriptInterface->pushFunction(m_onAnswer);
		lua_pushnumber(L, cid);
		lua_pushnumber(L, answer);
		m_scriptInterface->callFunction(2);
		m_scriptInterface->releaseScriptEnv();
	}
	else
	{
		std::cout << std::endl << "[Error] Call stack overflow. NpcScript::onAnswer";
	}
}

void NpcScript::onBuy(const Creature* creature, int callback)
{
	if(m_onBuy == -1){
		return;
	}

	if(m_scriptInterface->reserveScriptEnv())
	{
		ScriptEnviroment* env = m_scriptInterface->getScriptEnv();

		env->setScriptId(m_onBuy, m_scriptInterface);
		env->setNpc(m_npc);

		uint32_t cid = creature->getGUID();

		lua_State* L = m_scriptInterface->getLuaState();
		m_scriptInterface->pushFunction(m_onBuy);
		lua_pushnumber(L, cid);
		lua_pushnumber(L, callback);
		m_scriptInterface->callFunction(2);
		m_scriptInterface->releaseScriptEnv();
	}
	else {
		std::cout << std::endl << "[Error] Call stack overflow. NpcScript::onBuy";
	}
}

void NpcScript::onShopWindowClose(const Creature* creature)
{
	if(m_onShopWindowClose == -1){
		return;
	}

	if(m_scriptInterface->reserveScriptEnv())
	{
		ScriptEnviroment* env = m_scriptInterface->getScriptEnv();

		env->setScriptId(m_onShopWindowClose, m_scriptInterface);
		env->setNpc(m_npc);

		uint32_t cid = creature->getGUID();

		lua_State* L = m_scriptInterface->getLuaState();
		m_scriptInterface->pushFunction(m_onShopWindowClose);
		lua_pushnumber(L, cid);
		m_scriptInterface->callFunction(1);
		m_scriptInterface->releaseScriptEnv();
	}
	else {
		std::cout << std::endl << "[Error] Call stack overflow. NpcScript::onShopWindowClose";
	}
}