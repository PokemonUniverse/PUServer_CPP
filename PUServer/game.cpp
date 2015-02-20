#include "game.h"
#include "serverthread.h"

extern ServerThread gameThread;

Game::Game()
{
	map = NULL;
	towns = new Towns();
	gameState = GAME_STATE_NORMAL;
}

Game::~Game()
{
	if(map) {
		delete map;
	}
}

bool Game::saveServer(bool closeServer /* = false */, bool shutDownServer /* = false */)
{
	if(getGameState() == GAME_STATE_CLOSED) {
		return false;
	}

	if(shutDownServer || closeServer) {
		gameState = GAME_STATE_CLOSED;
	}
	
	PlayerList::iterator it = playerList.begin();
	PlayerList::iterator ite = playerList.end();
	for(; it != ite; ++it)
	{
		Player *player = NULL;
		if(player = (*it)) 
		{
			if(closeServer) {
				player->sendDisconnect();
			}
			else {
				player->saveInfo();
			}
		}
	}

	if(shutDownServer) {
		gameThread.StopThread();
	}

	return true;
}

bool Game::openServer()
{
	setGameState(GAME_STATE_NORMAL);
	return true;
}

bool Game::reloadMap()
{
	bool ret = true;
	if(getGameState() != GAME_STATE_CLOSED) {
		ret = saveServer(true, false);
	}
	
	if(ret && map->reload())
	{
		// Place creatures back on the map
		CreatureList::iterator it = creatureList.begin();
		CreatureList::iterator ite = creatureList.end();
		for(; it != ite; ++it)
		{
			Creature *creature = NULL;
			if(creature = (*it))
			{
				Tile *tile = getTile(creature->getPosition());
				if(tile) {
					tile->addCreature(creature);
				}
			}
		}
		
		ret = true;
	}

	if(ret) {
		setGameState(GAME_STATE_NORMAL);
	}

	return ret;
}

bool Game::loadMap()
{
	if(!map) {
		map = new Map();
	}

	return map->load();
}

bool Game::loadQuests()
{
	if(!quests) {
		quests = new Quests();
	}

	return quests->load();
}

bool Game::loadItems()
{
	if(!items) {
		items = new Items();
	}

	return items->reload();
}

Item *Game::getItem(int _id)
{
	return items->getItem(_id);
}

Tile *Game::getTile(int x, int y)
{
	return map->getTile(x, y);
}

Tile *Game::getTile(int x, int y, int z)
{
	return map->getTile(x, y, z);
}

Tile *Game::getTile(const Position pos)
{
	return getTile(pos.x, pos.y, pos.z);
}

void Game::addEncounterSection(int id, int rate)
{
	encounterSections.insert(std::make_pair(id, rate));
}

void Game::addPokeCenter(int id, Position coords)
{
	pokeCenterList.insert(std::make_pair(id, coords));
}

Position Game::getPokeCenter(int _id)
{
	PokeCenters::iterator it = pokeCenterList.find(_id);
	if(it != pokeCenterList.end())
	{
		return (*it).second;
	}

	return Position(0,0,0);
}

void Game::addTown(int _id, std::string _name, int _pokeCenter, int _musicID)
{
	Position pokecenterPos = getPokeCenter(_pokeCenter);
	
	Town *town = new Town(_id);
	town->setName(_name);
	town->setPokeCenter(pokecenterPos);
	town->setMusicID(_musicID);

	towns->addTown(_id, town);
}

Town *Game::getTown(int _id)
{
	return towns->getTown(_id);
}

bool Game::playerSayCommand(Player* player, SpeakClasses type, const std::string& text)
{
	//First, check if this was a command
	for(uint32_t i = 0; i < commandTags.size(); i++){
		if(commandTags[i] == text.substr(0,1)){
			if(commands.exeCommand(player, text)) { // || player->getAccessLevel() > 0){
				return true;
			}
		}
	}

	return false;
}

void Game::addCommandTag(std::string tag)
{
	bool found = false;
	for(uint32_t i=0; i< commandTags.size() ;i++) {
		if(commandTags[i] == tag) {
			found = true;
			break;
		}
	}

	if(!found){
		commandTags.push_back(tag);
	}
}

void Game::resetCommandTag()
{
	commandTags.clear();
}

void Game::disconnectAllPlayers()
{
	for(PlayerList::iterator it = playerList.begin(); it != playerList.end(); ++it)
	{
		if((*it)) {
			(*it)->sendDisconnect();
		}
	}	
}

void Game::addCreature(Creature *creature)
{
	if(!creature) {
		return;
	}

	internalAddCreature(creature);

	bool hasFollower = false;
	Pokemon *follower = NULL;
	if(creature->getPlayer() && (follower = creature->getPlayer()->getFollower()) ) {
		hasFollower = true;
	}

	boost::recursive_mutex::scoped_lock l_creature(creature_mutex);
	for(CreatureList::iterator it = creatureList.begin(); it != creatureList.end(); ++it)
	{
		Creature *tmpCreature = NULL;
		if((tmpCreature = (*it)) && tmpCreature != creature)
		{
			if(!tmpCreature || tmpCreature->isRemoved || !creature->canSee(tmpCreature))
				continue;

			// Creature to self
			if(!creature->knowsCreature(tmpCreature)) {
				creature->addVisibleCreature(tmpCreature);
			}

			// Self to creature
			if(!tmpCreature->knowsCreature(creature)) {
				tmpCreature->addVisibleCreature(creature);
				
				if(hasFollower) {
					tmpCreature->addVisibleCreature(follower);
				}
			}

			if(creature->getPlayer()) // Player friends
			{
				Player *player = creature->getPlayer();
				if(player->hasFriend(tmpCreature->getName())) {
					player->setOnline(tmpCreature->getName(), 1);
				}
			}

			if(tmpCreature->getPlayer()) 
			{
				Player *player = tmpCreature->getPlayer();
				if(player->hasFriend(creature->getName())) {
					player->setOnline(creature->getName(), 1);
				}
			}
		}
	}
}

void Game::internalAddCreature(Creature *creature)
{
	Tile *tile = getTile(creature->getPosition());
	if(tile) {
		tile->addCreature(creature);
	}
	
	creature->addVisibleCreature(creature);

	boost::recursive_mutex::scoped_lock l_creature(creature_mutex);
	creatureList.push_back(creature);

	if(creature->getPlayer()) {
		boost::recursive_mutex::scoped_lock l_player(player_mutex);
		playerList.push_back(creature->getPlayer());
	}
}

void Game::removeCreature(Creature *creature, bool deletePtr /*=true*/)
{
	if(!creature) {
		//std::cout << "[Error] removing creature: Doesn't exists" << endl;
		return;
	}

	// Check if creature is in battle
	if(creature->getBattle()) 
	{
		creature->isRemoved = true;
		creature->removeFromBattle();
	}
	else
	{		
		// Remove creature from tile
		Tile *tile = getTile(creature->getPosition());
		if(tile) {
			tile->removeCreature(creature);
		}

		boost::recursive_mutex::scoped_lock l_creature(creature_mutex);

		// Remove creature from players who knows creature
		for(CreatureList::iterator it = creatureList.begin(); it != creatureList.end(); ++it)
		{
			if((*it) && (*it) != creature)
			{
				if((*it)->knowsCreature(creature)) {
					(*it)->removeVisibleCreature(creature);
				}

				if((*it)->getPlayer())
				{
					if((*it)->getPlayer()->hasFriend(creature->getName())) {
						(*it)->getPlayer()->setOnline(creature->getName(), 0);
					}
				}
			}		
		}

		// Remove creature from player list (if player)
		if(creature->getPlayer()) 
		{
			if(creature->getPlayer()->playerState == PLS_LOADING) {
				creature->getPlayer()->playerState = PLS_EXIT;
				return;
			}

			creature->getPlayer()->playerState = PLS_EXIT;
			
			// Check if player is/has challenged
			if(creature->getPlayer()->getChallenged()) {
				creature->getPlayer()->challengeCancel(true);
			}

			creature->getPlayer()->savePokemon();
			creature->getPlayer()->saveInfo();

			boost::recursive_mutex::scoped_lock l_player(player_mutex);
			PlayerList::iterator delPlayer = std::find(playerList.begin(), playerList.end(), creature);
			if(delPlayer != playerList.end()) {
				playerList.erase(delPlayer);
			}

			std::cout << "[Logout] ID: " << creature->getGUID() << " - IP: " << creature->getPlayer()->getIP() << endl;
		}

		// Remove creature from list
		CreatureList::iterator delCreature = std::find(creatureList.begin(), creatureList.end(), creature);
		if(delCreature != creatureList.end()) {
			creatureList.erase(delCreature);
		}

		if(deletePtr) {
			delete creature;
		}
	}
}

Creature *Game::getCreatureByID(uint32_t creatureID)
{
	if(creatureID == 0)
		return NULL;

	for(CreatureList::iterator it = creatureList.begin(); it != creatureList.end(); ++it)
	{
		if((*it) && (*it)->getGUID() == creatureID)	{
			return (*it);
		}
	}

	return NULL;
}

Creature *Game::getCreatureByName(const std::string &s)
{
	std::string creatureName = asUpperCaseString(s);
	for(CreatureList::iterator it = creatureList.begin(); it != creatureList.end(); ++it)
	{
		if((*it) && asUpperCaseString((*it)->getName()) == creatureName) {
			return (*it);
		}
	}
	
	return NULL;
}

void Game::getCreaturesByPos(int x, int y, int z, CreatureList &list)
{
	CreatureList::iterator it = creatureList.begin();
	CreatureList::iterator ite = creatureList.end();
	for(; it != ite; it++)
	{
		if((*it)->getX() == x && (*it)->getY() == y)
		{
			list.push_back((*it));
		}
	}
}

Player *Game::getPlayerByID(uint32_t playerID)
{
	if(playerID == 0)
		return NULL;

	for(PlayerList::iterator it = playerList.begin(); it != playerList.end(); ++it)
	{
		if((*it) && (*it)->getGUID() == playerID) {
			return (*it);
		}
	}

	return NULL;
}

Player *Game::getPlayerByUID(uint32_t playerUID)
{
	if(playerUID == 0)
		return NULL;

	for(PlayerList::iterator it = playerList.begin(); it != playerList.end(); ++it)
	{
		if((*it) && (*it)->getID() == playerUID) {
			return (*it);
		}
	}

	return NULL;
}

Player *Game::getPlayerByName(const std::string &s)
{
	std::string playerName = asUpperCaseString(s);
	for(PlayerList::iterator it = playerList.begin(); it != playerList.end(); ++it)
	{
		if((*it) && asUpperCaseString((*it)->getName()) == playerName) {
			return (*it);
		}
	}
	
	return NULL;
}

ReturnValue Game::internalTeleport(Thing *thing, const Position &newPos, uint32_t flags /* = 0 */)
{
	if(newPos == thing->getPosition()) {
		return RET_NOERROR;
	}

	Cylinder *fromCylinder = getTile(thing->getPosition());
	Cylinder *toCylinder = getTile(newPos);
	
	if(fromCylinder != NULL && toCylinder != NULL)
	{
		if(Creature* creature = thing->getCreature()) 
		{		
			fromCylinder->getTile()->moveCreature(creature, creature->getDirection(), toCylinder);

			bool hasFollower = false;
			Pokemon *f = NULL;
			Cylinder *followFrom = NULL;
			if(creature->getPlayer() && (f = creature->getPlayer()->getFollower())) {
				hasFollower = true;
				followFrom = getTile(f->getPosition());
			}

			Creature *tmpCreature = NULL;
			for(CreatureList::iterator it = creatureList.begin(); it != creatureList.end(); ++it)
			{
				if(tmpCreature = (*it))	{
					tmpCreature->onCreatureMove(creature, fromCylinder, toCylinder);

					if(hasFollower) {
						tmpCreature->onCreatureMove(f, followFrom, fromCylinder);
					}
				}
			}

			return RET_PLAYERISTELEPORTED;
		}
//		else if(Item* item = thing->getItem()) {
//			return internalMoveItem(item->getParent(), toTile, INDEX_WHEREEVER, item, item->getItemCount(), NULL, flags);
//		}
	}

	return RET_NOTPOSSIBLE;
}

bool Game::internalCreatureTurn(Creature *creature, Direction dir)
{
	if(creature && creature->getDirection() != dir)
	{
		creature->setDirection(dir);

		CreatureList list = creature->getVisibleCreatures();
		Player *player = NULL;
		CreatureList::iterator it = list.begin();
		CreatureList::iterator ite = list.end();
		for(; it != ite; ++it)
		{
			if(player = (*it)->getPlayer()) {
				player->sendCreatureTurn(creature, dir);
			}
		}

		return true;
	}

	return false;
}

ReturnValue Game::internalCreatureMove(Creature *creature, Direction dir)
{
	if(!creature)
		return RET_NOTPOSSIBLE;

	const Position& currentPos = creature->getPosition();
	Position destPos = currentPos;

	Cylinder *fromCylinder = getTile(currentPos);
	Cylinder *toCylinder = NULL;
	switch(dir)
	{
		case DIR_NORTH:
			destPos.y -= 1;
			break;

		case DIR_SOUTH:
			destPos.y += 1;
			break;

		case DIR_WEST:
			destPos.x -= 1;
			break;

		case DIR_EAST:
			destPos.x += 1;
			break;
	}
	toCylinder = getTile(destPos);

	ReturnValue ret = RET_NOTPOSSIBLE;

	// Check movement of destination tile
	if(toCylinder != NULL) {
		ret = toCylinder->getTile()->checkMovement(creature, dir);
	}

	if(ret == RET_NOERROR) 
	{	
		bool toRet = fromCylinder->getTile()->moveCreature(creature, dir, toCylinder);

		if(creatureList.size() <= 0)
		{
			if(Player* player = creature->getPlayer()){
				player->sendCancelMessage(ret);
				player->sendCancelWalk(fromCylinder->getTile());
			}	

			return RET_NOTPOSSIBLE;
		}

		bool hasFollower = false;
		Pokemon *f = NULL;
		Cylinder *followFrom = NULL;
		if(creature->getPlayer() && (f = creature->getPlayer()->getFollower())) {
			hasFollower = true;
			followFrom = getTile(f->getPosition());
			f->setPosition(fromCylinder->getPosition());
		}

		// Send walk event to visible players
		Creature *tmpCreature = NULL;
		boost::recursive_mutex::scoped_lock l_creature(creature_mutex);
		CreatureList::iterator it = creatureList.begin();
		CreatureList::iterator ite = creatureList.end();
		for(; it != ite; ++it)
		{
			if(tmpCreature = (*it)) {
				if(tmpCreature->isRemoved) 
					continue;

				tmpCreature->onCreatureMove(creature, fromCylinder, toCylinder);
				if(hasFollower) {
					tmpCreature->onCreatureMove(f, followFrom, fromCylinder);
				}
			}
		}

		if(creature->getPlayer() != NULL) // Player only! 
		{
			Player *player = creature->getPlayer();
			/*if(player->getChallenged() && !player->challengeRadius()) {
				player->challengeCancel(true);
			}*/
			
			if(toRet) // Invoke teleport if true 
			{
				string *warpEvent = new string[4];
				PokeCode::Compile(warpEvent, (string)toCylinder->getTile()->getObject(1)->getEventData());

				int newX = atoi(warpEvent[1].c_str());
				int newY = atoi(warpEvent[2].c_str());
				int newZ = atoi(warpEvent[3].c_str());

				warpEvent = NULL;

				Tile *newCylinder = getTile(newX, newY, newZ);
				if(newCylinder == NULL) {
					ret = RET_NOTPOSSIBLE;
				}
				else {
					ret = internalTeleport(creature, newCylinder->getPosition());
				}
			}
			else // Check for battle encounters
			{
				int sectionID = 1337; //toCylinder->getTile()->getSection();
				if(sectionID > 0 && checkWilEncounter(creature, sectionID)) {
					player->sendCancelWalk(toCylinder->getTile());
				}
			}
		}
	}
	
	if(ret == RET_NOTPOSSIBLE) // Cancel movement
	{
		if(Player* player = creature->getPlayer()){
			player->sendCancelMessage(ret);

			if(fromCylinder)
				player->sendCancelWalk(fromCylinder->getTile());
		}		
	}

	return ret;
}

bool Game::internalCreatureSay(Creature *creature, SpeakClasses type, const std::string &text, int channelId)
{
	Position pos = creature->getPosition();

	boost::recursive_mutex::scoped_lock l_creature(creature_mutex);

	CreatureList list;
	if(type == SPEAK_YELL) 
	{
		CreatureList::iterator it = creatureList.begin();
		CreatureList::iterator ite = creatureList.end();
		for(; it != ite; ++it)
		{
			if(Position::areInRange<22,18>((*it)->getPosition(), pos)) {
				list.push_back(creature);
			}
		}
	}
	else if (type == SPEAK_SAY) {
		list = creature->getVisibleCreatures();
	}

	//send to client
	Player* tmpPlayer = NULL;
	CreatureList::iterator it = list.begin();
	CreatureList::iterator ite = list.end();
	for(; it != ite; ++it)
	{
		if((*it) && (tmpPlayer = (*it)->getPlayer())) {
			tmpPlayer->sendCreatureSay(creature, type, text, channelId);
		}
	}

	//event based methods
	Npc* tmpNcp = NULL;
	it = list.begin();
	for(; it != ite; ++it)
	{
		if((*it) && (tmpNcp = (*it)->getNpc()) ) {
			tmpNcp->onCreatureSay(creature, type, text);
		}
	}

	return true;
}

bool Game::internalBroadcastMessage(Player* player, const std::string& text)
{
	if(!player->hasFlag(PlayerFlag_CanBroadcast))
		return false;

	PlayerList::iterator it = playerList.begin();
	PlayerList::iterator ite = playerList.end();
	for(; it != ite; ++it)
	{
		Player *player = NULL;
		if(player = (*it)) {
			player->sendCreatureSay(player, SPEAK_BROADCAST, text);
		}
	}

	return true;
}

bool Game::internalMoveCreaturePokeCenter( Creature* creature )
{
	if(!creature->getPlayer())
		return false;

	Town *lastTown = creature->getPlayer()->getLastTown();
	Position warpPosition = lastTown->getPokeCenter();

	if(warpPosition == Position(0,0,0)) {
		return false;
	}

	if(internalTeleport(creature, warpPosition) == RET_PLAYERISTELEPORTED)
	{
		Tile *toCylinder = getTile(warpPosition);
		creature->getPlayer()->sendCancelWalk(toCylinder);	

		creature->getPlayer()->sendMap(DIR_NULL);
		return true;
	}

	return false;
}

bool Game::globalCreatureSay( Creature* creature, const std::string& text, int channelId /*= 0*/ )
{
	if(!creature) return false;

	Player* tmpPlayer = NULL;
	PlayerList::iterator it = playerList.begin();
	PlayerList::iterator ite = playerList.end();
	for(; it != ite; ++it)
	{
		(*it)->sendCreatureSay(creature, SPEAK_SAY, text, channelId);
	}	

	return true;
}

bool Game::playerPrivateMessage(Creature* creature, const std::string& text, int channelId)
{
	if(!creature) return false;

	Player* tmpPlayer = getPlayerByUID(channelId-10);
	if(tmpPlayer)
	{
		tmpPlayer->sendCreatureSay(creature, SPEAK_PRIVATE, text, creature->getID()+10);
	}
	tmpPlayer = dynamic_cast<Player*>(creature);
	if(tmpPlayer)
	{
		tmpPlayer->sendCreatureSay(creature, SPEAK_PRIVATE, text, channelId); // to self
	}

	return true;
}

bool Game::playerMove(Player *player, Direction direction, bool sendMap /*= true*/)
{
	if(!player) {
		return false;
	}
	else if(player->getTimeSinceLastMove() < player->getSpeed() || player->getBattle()) 
	{
		const Position& currentPos = player->getPosition();
		Tile *fromTile = getTile(currentPos);
		player->sendCancelWalk(fromTile);

		return false;
	}

	ReturnValue ret = internalCreatureMove(player, direction);
	if(ret == RET_NOERROR && sendMap) {
		// Send piece of map
		player->sendMap(direction);
	}
	else if(ret == RET_PLAYERISTELEPORTED) {	
		player->sendCreatureTeleport(player);

		// Send whole map
		player->sendMap(DIR_NULL);
	}

	return true;
}

bool Game::playerTurn(Player *player, Direction direction)
{
	if(!player) return false;

	return internalCreatureTurn(player, direction);
}

bool Game::playerSay(Player *player, uint16_t channelId, SpeakClasses type, const std::string& receiver, const std::string& text)
{
	if(!player) {
		return false;
	}

	//TODO: Add mute function

	//TODO: Add quote, white-space and bad-word filtering

	//TODO: Check if what player says is a command

	// Switch speak type
	if(channelId == CHANNEL_LOCAL)
	{
		switch(type)
		{
			case SPEAK_SAY:
				return internalCreatureSay(player, SPEAK_SAY, text, channelId);
				break;
			case SPEAK_YELL:
				return playerYell(player, text);
				break;
			case SPEAK_WHISPER:
				return playerWhisper(player, text);
				break;
			default:
				break;
		}
	}
	else if(channelId == CHANNEL_BATTLE) 
	{
		playerBattleChat(player, text);
	}
	else 
	{
		if(type == SPEAK_PRIVATE)
		{
			playerPrivateMessage(player, text, channelId);
		}
		else
		{
			globalCreatureSay(player, text, channelId);
		}
	}

	return false;	
}

bool Game::playerYell(Player *player, const std::string &text)
{
	if(!player) {
		return false;
	}

	//TODO: Check if player is not spamming, exhausted function (Conditions)

	internalCreatureSay(player, SPEAK_YELL, asUpperCaseString(text));
	
	return true;
}

bool Game::playerWhisper(Player *player, const std::string &text)
{
	if(!player) {
		return false;
	}

	//send to client
	CreatureList list = player->getVisibleCreatures();
	Player* tmpPlayer = NULL;
	for(CreatureList::iterator it = list.begin(); it != list.end(); ++it)
	{
		if((tmpPlayer = (*it)->getPlayer()))
		{
			if(Position::areInRange<1,1,0>(player->getPosition(), (*it)->getPosition())){
				tmpPlayer->sendCreatureSay(player, SPEAK_WHISPER, text);
			}
			else{
				tmpPlayer->sendCreatureSay(player, SPEAK_WHISPER, "pspspspsps");
			}
		}
	}	

	//event based methods
	for(CreatureList::iterator it = list.begin(); it != list.end(); ++it){
		(*it)->onCreatureSay(player, SPEAK_WHISPER, text);
	}

	return true;
}

bool Game::playerBattleChat(Creature *creature, const std::string& text)
{
	if(!creature || !creature->getBattle()) {
		return false;
	}

	Player *tmp = NULL;
	std::vector<Creature*> list = creature->getBattlePointer()->getPlayerList();
	for(CreatureList::iterator it = list.begin(); it != list.end(); ++it)
	{
		if(tmp = (*it)->getPlayer()) {
			tmp->sendCreatureSay(creature, SPEAK_SAY, text, CHANNEL_BATTLE);
		}
	}

	return true;
}

bool Game::checkWilEncounter(Creature *player, int sectionID)
{
	if(!player || !player->checkNextPokemon() || player->getPlayer()->hasFlag(PlayerFlag_AdminRepel)){
		return false;
	}

	if (random_range(1,100) > 50) {
		Battle *playerBattle = new Battle(ONE_VS_ONE);
		playerBattle->addPlayer(player, 0);
		int rnd = random_range(1,1000);
		if(rnd == 666) { 
			playerBattle->addPlayer(151, 99, 1); // Mew
		} else {
			playerBattle->addPlayer(41, 5, 1); // Zubat!!!
		}

		playerBattle->startBattle();

		return true;
	}
	return false;

	//////////////////////////////////////////////////////////////////////////

	SectionList::iterator it = encounterSections.find(sectionID);
	if(it == encounterSections.end()) {
		return false;
	}

	int sectionEncounterRate = it->second;
	int randomnRate = random_range(1,100);
	if(sectionEncounterRate >= randomnRate)
	{
		Battle *playerBattle = new Battle(ONE_VS_ONE);
		playerBattle->addPlayer(player, 0);
		int pokemon_id = playerBattle->addPlayer(sectionID, 1);

		// Update Pokedex
		player->getPlayer()->updatePokedex(pokemon_id, 0);

		playerBattle->startBattle();

		return true;
	}	

	return false;
}

void Game::playerChangeOutfit( Player *player, CreatureOutfit outfit )
{
	if(!player) {
		return;
	}

	player->setOutfit(outfit);

	CreatureList list = player->getVisibleCreatures();

	//send to client
	Player* tmpPlayer = NULL;
	CreatureList::iterator it = list.begin();
	CreatureList::iterator ite = list.end();
	for(; it != ite; ++it)
	{
		if(tmpPlayer = (*it)->getPlayer()) {
			tmpPlayer->sendCreatureChangeOutfit(player);
		}
	}
}
