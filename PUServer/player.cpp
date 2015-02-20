#include "player.h"
#include "game.h"
#include "client.h"

#include <boost/foreach.hpp>

extern Game g_game;

Player::Player(PUSocket *s, const std::string &_name) : Creature()
{	
	playerState = PLS_LOADING;
	pongFail = 0;

	database = new Database();
	if(database->connectDB())
	{
		client = new Client(this, s);

		battle = NULL;
		isInBattle = false;
		isChallenged = false;
		sex = PLAYERSEX_MALE;
		money = 0;
		speed = 300;
		groupFlags = 0;
		shopOwner = NULL;
		follower = NULL;
		lastTown = NULL;
		lastPokeCenter = Position(0,0,0);

		if(loadInfo(_name))
		{
			aprilFoolsHack();
			setName(_name);

			tcp::endpoint remote_ep = s->socket.remote_endpoint();
			boost::asio::ip::address remote_ad = remote_ep.address();
			clientIP = remote_ad.to_string();

			g_game.addCreature(this);
			
			std::cout << "[Login] " << getGUID() << " - " << getName() << " - " << clientIP << std::endl;

			std::stringstream logStr;
			logStr << "[Login] " << getGUID() << " - " << getName() << " - " << clientIP << std::endl;
			g_log.WriteLine(logStr.str());	

			if(playerState == PLS_EXIT) {
				delete this;
				return;
			}

			playerState = PLS_RUNNING;
		}
		else
		{
			playerState = PLS_EXIT;
			delete this;
		}
	}
}

void Player::aprilFoolsHack()
{
	setPosition(random_range(-80, -60), random_range(60,80));
	isRemoved = false;

	// Load Outfit
	outfit.head_style = 1;
	outfit.head_colour = generateColour();
	outfit.nek_style = 1;
	outfit.nek_colour = generateColour();
	outfit.upper_style = 1;
	outfit.upper_colour = generateColour();
	outfit.lower_style = 1;
	outfit.lower_colour = generateColour();
	outfit.feet_style = 1;
	outfit.feet_colour = generateColour();

	// Pokemanz
	Pokemon *pokemon = new Pokemon(database, 129, 99); // Lvl 99 Magikarp! FUCK YEAH!
	//int lvl99 = StatCalculator::Calculate_LevelEXP(99);
	//int lvl100 = StatCalculator::Calculate_LevelEXP(100);
	//cout << lvl99 << " - " << lvl100 << " - " << pokemon->getCurrentExp() << std::endl;
	pokemonCount = 1;
	pokemonParty.insert(std::pair<int, Pokemon*>(0, pokemon) );
	setActivePokemon();
}

int Player::generateColour()
{
	int r = random_range(1,254);
	int g = random_range(1,254);
	int b = random_range(1,254);

	int color = (r << 16) | (g << 8) | (b);
	return color;
}

Player::~Player()
{
	/*
	if(database) 
	{
		std::string logoutQuery = boost::str(boost::format("UPDATE user SET User_Online='0' WHERE User_ID=%d") % getID());
		database->runQuery(logoutQuery);

		database->disconnectDB();
		delete database;
	}*/

	if(client) {
		delete client;
	}

	if(pokemonParty.size() > 0) 
	{
		Pokemon *p = NULL;
		BOOST_FOREACH(PokemonList::value_type it, getPokemonParty())
		{
			if((p = it.second) && p)
			{
				g_game.removeCreature(p);
			}
		}
	}
}

bool Player::loadInfo(const std::string &_name)
{
	return true;
	
	std::stringstream query;
	query << "SELECT u.User_ID, u.User_Name, u.User_PosX, u.User_PosY, u.User_Movement, u.User_Speed, u.User_isInside,"
		  << " u.User_Pkmn1, u.User_Pkmn2, u.User_Pkmn3, u.User_Pkmn4, u.User_Pkmn5, u.User_Pkmn6, u.User_PCID, u.User_LastTown,"
		  << " u.User_GroupID, u.User_Money, o.head_style, o.nek_style, o.upper_style, o.lower_style, o.feet_style"
		  << " FROM user u"
		  << " LEFT JOIN outfits o ON o.userID=u.User_ID"
		  << " WHERE u.User_Name='" << _name << "'";

	boost::scoped_ptr<Recordset> rst(new Recordset(database, query.str()));

	if(rst->moveNext())
	{
		m_id = rst->getInt("User_ID");
		setName(rst->getString("User_Name"));
		movement = rst->getInt("User_Movement");
		userGroup = rst->getInt("User_GroupID");
		speed = rst->getInt("User_Speed");
		money = rst->getInt("User_Money");
		isInvisible = false;
		isInBattle = false;

		setPosition(rst->getInt("User_PosX"), rst->getInt("User_PosY"), rst->getInt("User_isInside"));
		loadGroupData(userGroup);
		if(userGroup == 1) {
			level = 1337;
		}

		int lastTownID = rst->getInt("User_LastTown");
		if(lastTownID == 0) {
			Tile *tile = g_game.getTile(getPosition());
			lastTownID = tile->getTown();
		}
		setLastTown(lastTownID);

		if(lastPokeCenter == Position(0,0,0)) { lastPokeCenter = Position(rst->getString("User_PCID")); }

		pokemonCount = 0;
		if(rst->getInt("User_Pkmn1") > 0) {
			pokemonCount++;
			pokemonParty.insert(std::pair<int, Pokemon*>(0, new Pokemon(database, rst->getInt("User_Pkmn1"), true) ) );
		}
		if(rst->getInt("User_Pkmn2") > 0) {
			pokemonCount++;
			pokemonParty.insert(std::pair<int, Pokemon*>(1, new Pokemon(database, rst->getInt("User_Pkmn2"), true) ) );
		}
		if(rst->getInt("User_Pkmn3") > 0) {
			pokemonCount++;
			pokemonParty.insert(std::pair<int, Pokemon*>(2, new Pokemon(database, rst->getInt("User_Pkmn3"), true) ) );
		}
		if(rst->getInt("User_Pkmn4") > 0) {
			pokemonCount++;
			pokemonParty.insert(std::pair<int, Pokemon*>(3, new Pokemon(database, rst->getInt("User_Pkmn4"), true) ) );
		}
		if(rst->getInt("User_Pkmn5") > 0) {
			pokemonCount++;
			pokemonParty.insert(std::pair<int, Pokemon*>(4, new Pokemon(database, rst->getInt("User_Pkmn5"), true) ) );
		}
		if(rst->getInt("User_Pkmn6") > 0) {
			pokemonCount++;
			pokemonParty.insert(std::pair<int, Pokemon*>(5, new Pokemon(database, rst->getInt("User_Pkmn6"), true) ) );
		}
		setActivePokemon();

		setFollower(getActivePokemon());

		// Load Outfit
		int headKey = rst->getInt("head_style");
		outfit.head_style = (char)(headKey >> 24);
		outfit.head_colour = headKey & 0xFFFFFF;
		int nekKey = rst->getInt("nek_style");
		outfit.nek_style = (char)(nekKey >> 24);
		outfit.nek_colour = nekKey & 0xFFFFFF;
		int upperKey = rst->getInt("upper_style");
		outfit.upper_style = (char)(upperKey >> 24);
		outfit.upper_colour = upperKey & 0xFFFFFF;
		int lowerKey = rst->getInt("lower_style");
		outfit.lower_style = (char)(lowerKey >> 24);
		outfit.lower_colour = lowerKey & 0xFFFFFF;
		int feetKey = rst->getInt("feet_style");
		outfit.feet_style = (char)(feetKey >> 24);
		outfit.feet_colour = feetKey & 0xFFFFFF;

		// Load items
		backPack = new Depot(getID(), 25);
		depotItems = new Depot(getID());
		loadItems();
		loadQuests();
		loadFriends();

		// Set user online
		std::stringstream u_user;
		u_user << "UPDATE user SET User_Online='1' WHERE User_ID='" << getID() << "'";
		database->runQuery(u_user.str());

		return true;
	}

	return false;
}

void Player::loadGroupData( int group_id )
{
	std::string query = boost::str(boost::format("SELECT flags, access FROM groups WHERE id=%d") % group_id);
	boost::scoped_ptr<Recordset> groupInfo(new Recordset(database, query));

	if(groupInfo->moveNext())
	{
		userStatus = groupInfo->getInt("access");
		groupFlags = groupInfo->getInt("flags");
	}
}

void Player::loadFriends()
{
	std::string query = boost::str(boost::format("SELECT friendName FROM user_friends WHERE userID=%d") % getID());
	boost::scoped_ptr<Recordset> friendInfo(new Recordset(database, query));

	while(friendInfo->moveNext())
	{
		std::string friendName = friendInfo->getString("friendName");
		addFriend(friendName);
	}
}

void Player::saveInfo()
{
	return;

	std::stringstream query;

	int pkmn1 = (getPokemon(0) != NULL) ? getPokemon(0)->getPokemonID() : 0;
	int pkmn2 = (getPokemon(1) != NULL) ? getPokemon(1)->getPokemonID() : 0;
	int pkmn3 = (getPokemon(2) != NULL) ? getPokemon(2)->getPokemonID() : 0;
	int pkmn4 = (getPokemon(3) != NULL) ? getPokemon(3)->getPokemonID() : 0;
	int pkmn5 = (getPokemon(4) != NULL) ? getPokemon(4)->getPokemonID() : 0;
	int pkmn6 = (getPokemon(5) != NULL) ? getPokemon(5)->getPokemonID() : 0;

	int lastTownID = (lastTown) ? lastTown->getTownID() : 0;

	query << "UPDATE user SET ";
	query << " User_PosX=" << getX();
	query << ",User_PosY=" << getY();
	query << ",User_Movement=" << movement;
	query << ",User_isInside=" << getZ();
	query << ",User_Online='0'";
	query << ",User_PCID='" << lastPokeCenter.toString() << "'";
	query << ",User_LastTown=" << lastTownID;
	query << ",User_Speed=" << speed;
	query << ",User_Money=" << money;
	query << ",User_Pkmn1=" << pkmn1;
	query << ",User_Pkmn2=" << pkmn2;
	query << ",User_Pkmn3=" << pkmn3;
	query << ",User_Pkmn4=" << pkmn4;
	query << ",User_Pkmn5=" << pkmn5;
	query << ",User_Pkmn6=" << pkmn6;
	query << " WHERE User_ID ='" << getID() << "'";

	database->runQuery(query.str().c_str());

	// Save outfit
	int head_key = (outfit.head_style << 24) | outfit.head_colour;
	int nek_key = (outfit.nek_style << 24) | outfit.nek_colour;
	int upper_key = (outfit.upper_style << 24) | outfit.upper_colour;
	int lower_key = (outfit.lower_style << 24) | outfit.lower_colour;
	int feet_key = (outfit.feet_style << 24) | outfit.feet_colour;

	std::string tmp = "UPDATE outfits SET head_style=%d, nek_style=%d, upper_style=%d, lower_style=%d, feet_style=%d WHERE userID=%d";
	std::string  outfitQuery = boost::str(boost::format(tmp) % head_key % nek_key % upper_key % lower_key % feet_key % getID());
	database->runQuery(outfitQuery);

	saveFriends();
	saveQuests();
	saveItems();
}

void Player::saveFriends()
{
	std::string deleteQuery = boost::str(boost::format("DELETE FROM user_friends WHERE userID=%d") % getID());
	database->runQuery(deleteQuery);

	BOOST_FOREACH(FriendList::value_type it, friends)
	{
		std::string insertQuery = boost::str(boost::format("INSERT INTO user_friends VALUES (%d,'%s')") % getID() % it.first);
		database->runQuery(insertQuery);
	}
}

void Player::updatePokedex(int _pokemonID, int _cought)
{
	stringstream pokedexSelect;
	pokedexSelect << "SELECT cought FROM user_pokedex WHERE userID='" << getID() << "' AND pokemonID='" << _pokemonID << "'";
	Recordset *rst = new Recordset(database, pokedexSelect.str());

	if(rst->moveNext())
	{
		if(_cought > -1 && rst->getInt("cought") != (unsigned)_cought)
		{
			stringstream updateQuery;
			updateQuery << "UPDATE user_pokedex SET cought='" << _cought << "' WHERE userID='" << getID() << "' AND pokemonID='" << _pokemonID << "'";
			database->runQuery(updateQuery.str());
		}
	}
	else 
	{
		stringstream insertQuery; 
		insertQuery << "INSERT INTO user_pokedex (userID, pokemonID, cought) VALUES ('" << getID() << "', '" << _pokemonID << "'";

		if(_cought >= 0)
			insertQuery << ", '" << _cought << "')";
		else
			insertQuery << ", '0')";

		database->runQuery(insertQuery.str());
	}

	delete rst;
}

void Player::savePokemon(int slotID)
{
	if(slotID >= 0)
	{
		if(getPokemon(slotID) == NULL)
			return;

		getPokemon(slotID)->savePokemonData(true);
	}
	else
	{
		for(int i = 0; i < pokemonCount; i++)
		{
			if(getPokemon(i) == NULL)
				continue;

			getPokemon(i)->savePokemonData(true);
		}
	}
}

bool Player::moveToPokeCenter()
{
	if(g_game.internalMoveCreaturePokeCenter(this))
	{
		std::vector<int> playList(1, 31);
		sendMusicUpdate(SOUND_PLAY, true, playList);

		return true;
	}

	return false;
}

void Player::challengePlayer(std::string playerName)
{
	Player *challenged = g_game.getPlayerByName(playerName);

	if(!challenged) {
		sendCreatureSay(NULL, SPEAK_SAY, "Challange failed: can not find this player.");
		return;
	}
	else if(challenged->battle || challenged->getChallenged()) {
		sendCreatureSay(NULL, SPEAK_SAY, "Challange failed: Player is already challenged or is in battle.");
		return;
	}
	else if(!Position::areInRange<1,1,0>(getPosition(), challenged->getPosition())) {
		sendCreatureSay(NULL, SPEAK_SAY, "Challange failed: you are not close enough to this player.");
		return;
	}

	// set challenged
	setChallenged(true);
	setChallengedID(challenged->getID());
	challenged->setChallenged(true);
	challenged->setChallengedID(getID());

	// send packet
	Packet challengerPacket; // uitdager
	challengerPacket.AddByte(0xE1);
	challengerPacket.AddString(challenged->getName());
	challengerPacket.AddByte(CHALLENGING);
	sendPacket(challengerPacket);	

	Packet challengedPacket; // uitgedaagde
	challengedPacket.AddByte(0xE1);
	challengedPacket.AddString(getName());
	challengedPacket.AddByte(CHALLENGED);
	challenged->sendPacket(challengedPacket);
}

void Player::challengeCancel(bool hazChallenged)
{
	if(!this) {
		return;
	}

	if(hazChallenged)
	{
		Player *player = g_game.getPlayerByUID(getChallengedID());
		if(player) {
			player->challengeCancel();
		}
	}

	this->setChallenged(false);
	this->setChallengedID(0);

	// Send packet
	Packet cancelChallenge;
	cancelChallenge.AddByte(0xE2);

	sendPacket(cancelChallenge);
}

void Player::challengeHandle(int handle)
{
	Player *challenged = g_game.getPlayerByUID(getChallengedID());

	if(!challenged)	{
		challengeCancel();
	}
	else if(!Position::areInRange<1,1,0>(getPosition(), challenged->getPosition())) {
		challengeCancel();
		challenged->challengeCancel();
	}
	else
	{	
		Packet handlePacket;
		handlePacket.AddByte(0xE3);
		handlePacket.AddByte(handle);

		sendPacket(handlePacket);
		challenged->sendPacket(handlePacket);

		if(handle == 1)
		{		
			// Start new battle
			Battle *newBattle = new Battle(ONE_VS_ONE);
			newBattle->addPlayer(this->getCreature(), 0);
			newBattle->addPlayer(challenged->getCreature(), 1);
			
			challengeCancel(true);
			newBattle->startBattle();
		}
		else
		{
			setChallenged(false);
			setChallengedID(0);
			challenged->challengeCancel(false);
		}
	}
}

bool Player::challengeRadius()
{
	if(!getChallenged()) {
		return false;
	}

	Player *challenged = g_game.getPlayerByUID(getChallengedID());
	if(!challenged) {
		return false;
	}

	return Position::areInRange<1,1,0>(getPosition(), challenged->getPosition());
}

void Player::onCreatureMove(const Creature *creature, Cylinder *fromCylinder, Cylinder *toCylinder)
{
	if(creature == this) {
		return;
	}

	Tile *toTile = toCylinder->getTile();
	Tile *fromTile = fromCylinder->getTile();

	bool canSeeNewPos = canSee(toTile->getPosition());
	bool canSeeOldPos = canSee(fromTile->getPosition());

	Creature *p = const_cast<Creature*>(creature);
	if(canSeeNewPos && !canSeeOldPos) // Creature enters viewport
	{
		if(!knowsCreature(creature)) {
			addVisibleCreature(creature);
		}

		if(!p->knowsCreature(this)) {
			p->addVisibleCreature(this);
		}

		sendCreatureMove(creature, fromTile, toTile);
	}
	else if(!canSeeNewPos && canSeeOldPos) // Creature leaves viewport
	{
		sendCreatureMove(creature, fromTile, toTile);

		if(knowsCreature(creature))	{ 
			removeVisibleCreature(creature); 
		}
		if(p->knowsCreature(this)) {
			p->removeVisibleCreature(this);
		}
	}
	else if(canSeeNewPos && canSeeOldPos) // Creature moves in viewport
	{
		if(!knowsCreature(creature)) {
			addVisibleCreature(creature);
		}
		if(!p->knowsCreature(this)) {
			p->addVisibleCreature(this);
		}

		sendCreatureMove(creature, fromTile, toTile);
	}
}

void Player::addVisibleCreature(const Creature *creature)
{
	client->sendAddCreature(creature);
	Creature::addVisibleCreature(creature);
}

void Player::removeVisibleCreature(const Creature *creature)
{	
	client->sendRemoveCreature(creature);
	Creature::removeVisibleCreature(creature);
}

void Player::sendCancelMessage(ReturnValue message) const
{
	std::string msg;
	switch(message)
	{
	case RET_YOUAREINACTIVE:
		msg = "You are inactive for 10 minutes, you will be logged out in 5 minutes.";
		break;
	case RET_NOTPOSSIBLE:
	default:
		msg = "Sorry, not possible.";
		break;
	}

	client->sendCancelMessage(msg);
}

void Player::sendPacket(Packet packet)
{
	client->sendDataPacket(packet);
}

bool Player::sendPing()
{
	if(getPong() >= 3) {
		std::cout << "[Timeout]  ID: " << getGUID() << " - IP: " << getIP() << endl;
		
		resetPong();
		g_game.removeCreature(this);

		return true;
	}
	else {
		increasePong();
		client->sendPing();
		
		return false;
	}
}

void Player::sendLogin() 
{ 
	client->sendLogin(); 
}

void Player:: sendDisconnect() 
{ 
	client->sendDisconnect(); 
}

void Player::sendCreatureSay(Creature* creature, SpeakClasses type, std::string text, int channelId) 
{ 
	client->sendCreatureSay(creature, type, text, channelId); 
}

void Player::sendCreatureTeleport(Creature *creature)
{
	client->sendCreatureTeleport(creature);
}	

void Player::sendCreatureTurn(Creature* creature, Direction dir)
{ 
	client->sendCreatureTurn(creature, dir); 
}

void Player::sendCreatureMove(const Creature* creature, Tile* fromTile, Tile* toTile)
{ 
	client->sendCreatureMove(creature, fromTile, toTile); 
}

void Player::sendCancelWalk(Tile *fromCylinder)
{ 
	client->sendCancelWalk(fromCylinder); 
}

void Player::sendMap(Direction dir) 
{ 
	client->sendMap(dir); 
}

void Player::sendItems()
{
	client->sendItems(backPack->getItemList());
}

void Player::sendToChannel( Creature* creature, SpeakClasses type, std::string text, uint16_t channelId, uint32_t time /*= 0*/ )
{
	client->sendToChannel(creature, type, text, channelId, time);
}

void Player::sendDialogueClose()
{
	client->sendDialogueClose();
}

void Player::sendDialogueQuestions( DIALOGUE_TYPES type, std::string question, std::map<int, std::string> answers, int npc_id /*= 0*/ )
{
	client->sendDialogueQuestions(type, question, answers, npc_id);
}

void Player::sendMusicUpdate(SoundType_t _type, bool _fromStart, std::vector<int> playList)
{
	client->sendMusicUpdate(_type, _fromStart, playList);
}

void Player::setBattlePacket(boost::shared_ptr<Packet> packet)
{
	if(!battle) {
		return;
	}

	int type = packet->ReadInt();
	int parameter = packet->ReadInt();

	switch(type)
	{
	case MOVE_RUN:
		battle->validateRun(this);
		break;
	case MOVE_ATTACK:
		battle->validateAttack(this, parameter);
		break;
	case MOVE_FINISH:
		setPlayerStatus(PS_READY);
		break;
	case MOVE_POKEMON:
		battle->validatePokemonChange(this, parameter);
		break;
	case MOVE_ANSWER:
		battle->validateAnswer(this, parameter);
		break;
	case MOVE_BAG:
		int _slot = packet->ReadInt();
		battle->validateItem(this, parameter, _slot);
		break;
	}
}

void Player::setBattleDialogueAnswer(int option)
{
	if(!battle) {
		return;
	}

	battle->validateAnswer(this, option);
	
	sendDialogueClose();
}

void Player::doAction()
{
	Tile *actionTile = NULL;
	switch(this->getDirection())
	{
		case DIR_NORTH:
		{
			actionTile = g_game.getTile(getX(), getY()-1, getZ());
		}
		break;

		case DIR_EAST:
		{
			actionTile = g_game.getTile(getX()+1, getY(), getZ());
		}
		break;

		case DIR_SOUTH:
		{
			actionTile = g_game.getTile(getX(), getY()+1, getZ());
		}
		break;

		case DIR_WEST:
		{
			actionTile = g_game.getTile(getX()-1, getY(), getZ());
		}
		break;
	}
	if(actionTile)
	{
		CreatureList actionCreatures;
		g_game.getCreaturesByPos(actionTile->getX(), actionTile->getY(), actionTile->getZ(), actionCreatures);

		CreatureList::iterator it = actionCreatures.begin();
		CreatureList::iterator ite = actionCreatures.end();
		for(; it != ite; it++)
		{
			if((*it)->getNpc())
			{
				(*it)->getNpc()->startDialogue(this);
			}
		}
	}
}

uint16_t Player::getQuestProgress(int questid)
{
	PlayerQuestMap::iterator it = quests.find(questid);
	if(it == quests.end()) {
		return 0;
	}
	return it->second;
}

void Player::updateQuest(int questid, int progress)
{
	PlayerQuestMap::iterator it = quests.find(questid);
	if(it == quests.end())
	{
		std::pair<uint16_t,uint16_t> newQuest(questid, progress);
		quests.insert(newQuest);

		std::string query = boost::str(boost::format("INSERT INTO user_quests (user_id, quest_id, progress) VALUES (%d, %d, %d)") % getID() % questid % progress);
		database->runQuery(query);
	}
	else
	{
		it->second = progress;
	}
}

void Player::saveQuests()
{
	PlayerQuestMap::iterator it = quests.begin();
	PlayerQuestMap::iterator ite = quests.end();
	for(; it != ite; it++)
	{
		int quest_id = it->first;
		int quest_progress = it->second;

		std::string query = boost::str(boost::format("UPDATE user_quests SET progress = %d WHERE user_id = %d AND quest_id = %d") % quest_progress % getID() % quest_id);
		database->runQuery(query);
	}
}

void Player::loadQuests()
{
	std::string query = boost::str(boost::format("SELECT quest_id, progress FROM user_quests WHERE user_id = %d") % getID());
	boost::scoped_ptr<Recordset> questlist(new Recordset(database, query));

	if(questlist->numOfRows() <= 0)
		return;

	int quest_id, quest_progress;
	while(questlist->moveNext())
	{
		quest_id = questlist->getInt("quest_id");
		quest_progress = questlist->getInt("progress");

		std::pair<uint16_t,uint16_t> newQuest(quest_id, quest_progress);
		quests.insert(newQuest);
	}
}

void Player::loadItems()
{
	// Backpack
	std::string query = boost::str(boost::format("SELECT itemID, itemCount, slot FROM user_backpack WHERE userID = %d") % getID());
	boost::scoped_ptr<Recordset> itemlist(new Recordset(database, query));

	if(itemlist->numOfRows() > 0)
	{
		int item_id, count, slot;
		while(itemlist->moveNext())
		{
			item_id = itemlist->getInt("itemID");
			count = itemlist->getInt("itemCount");
			slot = itemlist->getInt("slot");

			getBackPack()->addItem(item_id, count, slot);
		}
	}

	// Load depot Pokemon
	std::stringstream queryExt;
	queryExt << "SELECT uniqueID FROM user_pokemon WHERE userID=" << getID();
	BOOST_FOREACH(PokemonList::value_type it, pokemonParty) {
		Pokemon* tmp = it.second;
		if(!tmp)
			continue;

		queryExt << " AND uniqueID != " << tmp->getPokemonID();
	}
	boost::scoped_ptr<Recordset> pokemonlist(new Recordset(database, queryExt.str()));
	
	if(pokemonlist->numOfRows() > 0)
	{
		while(pokemonlist->moveNext()) 
		{
			int uniqueID = pokemonlist->getInt("uniqueID");
			depotPokemon.insert(std::pair<int, Pokemon*>(uniqueID, new Pokemon(database, uniqueID, true)));
		}
	}
}

void Player::saveItems()
{	
	if(!getBackPack()) {
		return;
	}

	// Clear backpack
	std::string query = boost::str(boost::format("DELETE FROM user_backpack WHERE userID=%d") % getID());
	if(database->runQuery(query)) 
	{
		// Rewrite all data
		BOOST_FOREACH(ItemMap::value_type it, getBackPack()->getItemList())
		{
			int key = it.first;
			int slot = (char)key;

			Item *item = getBackPack()->getItem(key);
			if(item)
			{
				std::string saveQuery = boost::str(boost::format("INSERT INTO user_backpack (userID, itemID, itemCount, slot) VALUES (%d, %d, %d, %d)") % getID() % item->getID() % item->getCount() % slot);
				database->runQuery(saveQuery);
			}
		}
	}
}

void Player::addNewPokemonToParty( int slot, int uniquePokemonID )
{
	pokemonParty.insert(std::pair<int, Pokemon*>(slot, new Pokemon(database, uniquePokemonID, true) ) );
	pokemonCount = pokemonParty.size();
}

void Player::addDepotPokemonToParty( int slot, int uniquePokemonID )
{
	Pokemon *inDepot = depotPokemon[uniquePokemonID];
	if(inDepot)
	{
		// get pokemon in the slot to write to
		Pokemon *inSlot = pokemonParty[slot];
		if(inSlot)
		{
			// Put it to the depot
			depotPokemon[inSlot->getPokemonID()] = inSlot;
		}

		pokemonParty[slot] = inDepot;
		depotPokemon.erase(uniquePokemonID);
	}
}

bool Player::addItem( int _itemId, int _count /* = 1 */, int _slot /* = -1 */ )
{
	bool ret = true;
	if((getBackPack()->getStorageCount() + _count) > getBackPack()->getMaxItems()) {
		return false;
	}

	Item *item = g_game.getItem(_itemId);
	if(!item) {
		return false;
	}

	int category = item->getCategoryID();
	int slot = getBackPack()->searchItem(category, _itemId);
	int itemIndex = -1;

	if(slot == -1)
	{
		int newSlot = getBackPack()->getCategoryFreeSlot(category);
		ret = getBackPack()->addItem(_itemId, _count, newSlot);	

		itemIndex = Items::createIndex(newSlot, category);
	}
	else
	{
		itemIndex = Items::createIndex(slot, category);
		ret = getBackPack()->updateItem(itemIndex, _count);
	}

	if(ret && itemIndex > -1) {
		client->sendItemAdd(itemIndex);
	}

	return ret;
}

bool Player::updateItem(int _itemIndex, int _count)
{
	Item *item = getBackPack()->getItem(_itemIndex);
	if(!item) {
		return false;
	}

	getBackPack()->updateItem(_itemIndex, _count);

	if((item->getCount() - _count) <= 0) {
		client->sendItemRemove(_itemIndex);
	}
	else {
		client->sendItemAdd(_itemIndex);
	}

	return true;
}

void Player::removeItem(int _itemIndex)
{
	getBackPack()->removeItem(_itemIndex);
	client->sendItemRemove(_itemIndex);
}

void Player::useItem( int _itemIndex )
{
	setLastUsedItemIndex(-1);
	Item *item = getBackPack()->getItem(_itemIndex);
	if(!item) {
		return;
	}

	Packet packet;
	packet.AddByte(0xC8);
	if(item->getPlusHP() > 0) 
	{
		packet.AddInt(0); // Use on pokemon
		sendPacket(packet);

		setLastUsedItemIndex(_itemIndex);

		sendCreatureSay(NULL, SPEAK_SAY, "Please choose a Pokemon to use this item on...");
	}
	else if(item->getPlusPP() > 0) 
	{
		packet.AddInt(1); // Use on attack
		sendPacket(packet);

		setLastUsedItemIndex(_itemIndex);

		sendCreatureSay(NULL, SPEAK_SAY, "Please choose a Pokemon and attack to use this item on...");
	}
}

void Player::useItemPokemon( int _pokemon, int _attack )
{
	int itemIndex = getLastUsedItemIndex();
	
	Item *item = getBackPack()->getItem(itemIndex);
	Pokemon *pokemon = getPokemon(_pokemon);
	bool isUsed = false;

	if(item && pokemon)
	{
		if(item->getPlusHP() > 0) 
		{
			if(pokemon->getPokemonHP_Left() != pokemon->getPokemonHP_Total()) {
				pokemon->increaseHP(item->getPlusHP());
				isUsed = true;
			}
		}
		else if(item->getPlusPP() > 0)
		{
			Attack *attack = pokemon->getAttack(_attack);
			if(attack && attack->getUsageLeft() != attack->getUsageTotal()) {
				attack->setUsageLeft(attack->getUsageLeft() + item->getPlusPP());
				isUsed = true;
			}
		}

		if(isUsed) {
			getBackPack()->updateItem(itemIndex, -1);
			client->sendPokemonData(pokemonParty);
		}
	}
	
	setLastUsedItemIndex(-1);
}

void Player::kickPlayer()
{
	if(client){
		client->sendDisconnect();
	}
	else{
		g_game.removeCreature(this);
	}
}

void Player::setLastTown( int _townID )
{
	if(_townID == 0) 
		return;

	Town *newTown = g_game.getTown(_townID);

	if(!newTown) {
		return;
	}

	if(lastTown && lastTown->getTownID() == _townID) {
		return;
	}

	lastTown = newTown;
	if(lastTown->getPokeCenter() != Position(0,0,0) && lastTown->getPokeCenter() != lastPokeCenter) {
		lastPokeCenter = lastTown->getPokeCenter();
	}

	if(lastTown->getMusicID() != 0)	{
		std::vector<int> playList(1, lastTown->getMusicID());
		sendMusicUpdate(SOUND_FADEIN, true, playList);
	}
}

void Player::sendCreatureChangeOutfit( Creature *creature )
{
	client->sendCreatureChangeOutfit(creature);
}

void Player::openShopWindow( Npc* npc )
{
	setShopOwner(npc);
	client->sendItemsShop(npc->getItemDepot()->getItemList());
}

void Player::shopBuyItem( int _itemIndex, int _count )
{
	Npc *npc = getShopOwner();
	if(!npc)
		return;

	npc->onPlayerTrade(this, SHOPEVENT_BUY, -1, _itemIndex, _count, false);
}

void Player::closeShopWindow()
{
	Npc *npc = getShopOwner();
	if(npc) {
		npc->onPlayerCloseShopWindow(this);
	}

	client->sendItemsShopCloseWindow();

	setShopOwner(NULL);
}

void Player::addMoney( int _money )
{
	money += _money;

	client->sendMoney(money);
}

void Player::removeMoney( int _money )
{
	money -= _money;
	if(money < 0) {
		money = 0;
	}

	client->sendMoney(money);
}

void Player::addFriend( std::string _name )
{
	Creature *creature = g_game.getCreatureByName(_name);
	if(creature && !creature->getPlayer()){
		return;
	}

	std::string friendName;
	int isOnline = 0;
	bool added = false;

	if(!creature)
	{
		std::string query = boost::str(boost::format("SELECT User_Name FROM user WHERE User_Name='%s'") % _name);
		boost::scoped_ptr<Recordset> friendQuery(new Recordset(database, query));
		if(friendQuery->numOfRows() > 0 && friendQuery->moveNext()) {
			friendName = friendQuery->getString("User_Name");
			added = true;
		}
	}
	else
	{
		isOnline = 1;
		friendName = creature->getName();
		added = true;
	}

	if(added) 
	{
		if(!hasFriend(friendName)) {
			friends.insert(std::pair<std::string, int>(friendName, isOnline));
		}

		client->sendFriendAdd(friendName, isOnline);
	}
}

void Player::removeFriend( std::string _name )
{
	if(!hasFriend(_name)) {
		return;
	}

	friends.erase(_name);

	client->sendFriendRemove(_name);
}

bool Player::hasFriend( std::string _name )
{
	BOOST_FOREACH(FriendList::value_type it, friends)
	{
		if(it.first == _name)
			return true;
	}

	return false;
}

void Player::setOnline( std::string _name, int online )
{
	FriendList::iterator it = friends.find(_name);
	if(it != friends.end())
	{
		it->second = online;
		std::string name = it->first;

		client->sendFriendAdd(name, online);
	}
}

void Player::setFollower( Pokemon *p )
{
	if(p == NULL) {
		g_game.removeCreature(getFollower(), false);
		return;
	}

	if(p->getPokemonHP_Left() <= 0) {
		return;
	}

	Direction dir;
	Position followerPos;

	if(getFollower()) 
	{
		g_game.removeCreature(getFollower(), false);

		dir = getFollower()->getDirection();
		followerPos = getFollower()->getPosition();
	}
	else
	{
		dir = getDirection();
		followerPos = getPosition();
		switch (dir)
		{
		case DIR_NORTH:
			followerPos.y++;
			break;
		case DIR_EAST:
			followerPos.x++;
			break;
		case DIR_SOUTH:
			followerPos.y--;
			break;
		case DIR_WEST:
			followerPos.x--;
			break;
		}
	}

	p->setDirection(dir);
	p->setPosition(followerPos);
	follower = p;

	g_game.addCreature(follower);
}