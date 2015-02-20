#include "battle.h"
#include "battlePlayer.h"
#include "battleevents.h"
#include "packet.h"
#include "tools.h"
#include "game.h"
#include "pokecenter.h"

#include <math.h>

#ifdef __BUGTRAP__
#include <eh.h>
#include "BugTrap.h"
#endif

extern Game g_game;

Battle::Battle(Battle_t type)
{
	battleType = type;

	totalPlayers = 0;
	playerCounter = 0;
	minPlayers = 2;
	turnMoveID = 0;
	freePokemonChange = false;
	isWildBattle = false;
	
	if(battleType == TWO_VS_ONE || battleType == ONE_VS_TWO) {
		minPlayers = 3;
	}
	else if(battleType == TWO_VS_TWO) {
		minPlayers = 4;
	}

	maxPlayers = minPlayers;

	srand(time(0));

	db = new Database();
	db->connectDB();
}

Battle::~Battle()
{
	delete db; // This will also disconnect the DB
}

bool Battle::addPlayer(Creature *creature, int team)
{
	if(!checkPlaceInTeam(team))	{
		return false;
	}

	BattlePlayer *newPlayer = new BattlePlayer(creature);
	newPlayer->setBattleID(totalPlayers++);
	newPlayer->setTeam(team);
	battlePlayerList[totalPlayers] = newPlayer;

	creature->setBattlePointer(this);

	return true;
}

bool Battle::addPlayer(int pokemonID, int level, int team)
{
	if(!checkPlaceInTeam(team))	{
		return false;
	}

	BattlePlayer *newPlayer = new BattlePlayer(db, pokemonID, level);

	if(!newPlayer || !newPlayer->getPokemonActive()) { 
		return false;
	}

	newPlayer->setBattleID(totalPlayers++);
	newPlayer->setTeam(team);
	battlePlayerList[totalPlayers] = newPlayer;

	isWildBattle = true;

	return true;
}

int Battle::addPlayer(int sectionID, int team)
{
	if(!checkPlaceInTeam(team))	{
		return false;
	}

	std::stringstream sectionData;
	sectionData << "SELECT pokemon_id, rarity, min_level, max_level FROM location_encounters WHERE section_id='" << sectionID << "' AND method='grass'";
	Recordset *rst = new Recordset(db, sectionData.str().c_str());

	EncounterMap pokemonList;
	int i = 0;
	while(rst->moveNext())
	{
		for(unsigned int p = 1; p <= rst->getInt("rarity"); p++)
		{
			pokemonList[i] = WildPokemonEncounter(rst->getInt("pokemon_id"), rst->getInt("min_level"), rst->getInt("max_level"));
			i++;
		}
	}

	delete rst;

	int RandomIndex = random_range(0, i-1);
	WildPokemonEncounter FinalEncounter = pokemonList[RandomIndex];
	if(!FinalEncounter.pokemonID) {
		FinalEncounter = pokemonList[0];
	}
	int pokemonLvl = random_range(FinalEncounter.minLevel, FinalEncounter.maxLevel);

	BattlePlayer *newPlayer = new BattlePlayer(db, FinalEncounter.pokemonID, pokemonLvl);
	newPlayer->setBattleID(totalPlayers++);
	newPlayer->setTeam(team);
	battlePlayerList[totalPlayers] = newPlayer;

	isWildBattle = true;

	return newPlayer->getPokemonActive()->getPokemonDbID();
}

void Battle::removePlayer(BattlePlayer *_player, bool punishPlayer /*= false*/)
{
	if(_player == NULL) {
		return;
	}

	if(punishPlayer)
	{
		//TODO: Kill all pokemon, lose moneyz?
	}

	_player->isRemoved = true;
}

void Battle::deleteBattlePlayer(BattlePlayer *_player)
{
	if(!_player) {
		return;
	}

	if(_player->getCreatureType() == PLAYER)
	{
		_player->getCreature()->setBattle(false);
		_player->getCreature()->setBattlePointer(NULL);

		if(_player->getCreature()->isRemoved) {
			g_game.removeCreature(_player->getCreature());
		}
		else 
		{
			Town *town = _player->getCreature()->getPlayer()->getLastTown();
			int musicID = 0;
			if(town) { musicID = town->getMusicID(); }
			std::vector<int> playList(1, musicID);
			_player->getCreature()->getPlayer()->sendMusicUpdate(SOUND_PLAY, false, playList);

			_player->getCreature()->setActivePokemon();
			_player->getCreature()->getPlayer()->setFollower(_player->getPokemonActive());

			BattleEvents::_EndBattlePacket(_player);
		}
	}

	delete _player;
}

BattlePlayer *Battle::getPlayer( int playerID )
{
	BattleList::iterator it = battlePlayerList.find(playerID);
	if(it != battlePlayerList.end()) {
		return it->second;
	}

	return NULL;
}

BattlePlayer *Battle::getBattlePlayerByPlayer(Player *player)
{
	for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
	{
		if((*it).second->getCreatureType() != POKEMON && (*it).second->getCreature()->getGUID() == player->getGUID()) {
			return (*it).second;
		}
	}

	return NULL;
}

int Battle::getPlayersTeam(int team)
{
	int counter = 0;
	for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
	{
		if((*it).second->getTeam() == team) {
			counter++;
		}
	}	
	
	return counter;
}

bool Battle::checkPlaceInTeam(int team)
{
	int teamSize = getPlayersTeam(team);

	if(battleType == ONE_VS_ONE && teamSize == 1) {
		return false;
	}
	else if(battleType == TWO_VS_ONE && ((team == 0 && teamSize == 2) || (team == 1 && teamSize == 1))) {
		return false;
	}
	else if(battleType == ONE_VS_TWO && ((team == 0 && teamSize == 1) || (team == 1 && teamSize == 2))) {
		return false;
	}
	else if(battleType == TWO_VS_TWO && teamSize == 2) {
		return false;
	}
	
	return true;
}

int Battle::getPlayersTotal()
{
	int count = 0;

	for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
	{
		BattlePlayer *tmp;
		if((tmp = (*it).second) && !tmp->isRemoved)
		{
			count++;
		}
	}
	return count;
}

bool Battle::startBattle()
{
	if(getPlayersTotal() < minPlayers) {
		return false;
	}

	// setup players
	setupBattleOrder();

	// send prebattle data
	preBattleData();

	// Start thread
	battleThread = boost::thread(boost::bind(&Battle::run, this));

	return true;
}

void Battle::endBattle()
{
	battleState = STATE_STOPPING;

	for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
	{
		BattlePlayer *tmp;
		if(tmp = (*it).second)
		{
			deleteBattlePlayer(tmp);
		}
	}

	delete this;
}

bool Battle::checkBattleContinue()
{
	int count = 0;
	BattleList::iterator tmp = battlePlayerList.begin();
	BattleList::iterator tmpEnd = battlePlayerList.end();
	for (; tmp != tmpEnd; ++tmp) 
	{						
		BattlePlayer *tmpIt;
		if((tmpIt = (*tmp).second) && !tmpIt->isRemoved && 
			(tmpIt->getPokemonActive()->getPokemonHP_Left() > 0 || 
			(tmpIt->getPokemonActive()->getPokemonHP_Left() <= 0 && tmpIt->checkPokemonNext())
			)) {
			count++;
		}
	}

	if(count < minPlayers) {
		return false;
	}

	return true;
}

void Battle::setupBattleOrder()
{
	int speedTeam0 = 0;
	int speedTeam1 = 0;

	for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
	{
		BattlePlayer *tmpPlayer;
		if((tmpPlayer = it->second) && tmpPlayer->getPokemonActive())
		{
			if(tmpPlayer->getTeam() == 0) {
				speedTeam0 += tmpPlayer->getPokemonActive()->getPokemonSpeed();
			}
			else {
				speedTeam1 += tmpPlayer->getPokemonActive()->getPokemonSpeed();
			}
		}
	}

	int sizeTeam0 = getPlayersTeam(0);
	if(sizeTeam0 > 1) {
		speedTeam0 = speedTeam0 / sizeTeam0;
	}

	int sizeTeam1 = getPlayersTeam(1);
	if(sizeTeam1 > 1) {
		speedTeam1 = speedTeam1 / sizeTeam1;
	}

	if(speedTeam0 == speedTeam1) {
		if((StatCalculator::randomNumber() % 100) < 50) {
			speedTeam0 += 100;
		}
		else {
			speedTeam1 += 100;
		}
	}

	// Sort all players
	BattleList newPlayers;
	int count = 0;
	if(speedTeam0 > speedTeam1) 
	{
		for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) {
			if((*it).second->getTeam() == 0) {
				(*it).second->setBattleID(count);
				newPlayers[count++] = (*it).second;
			}
		}

		for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) {
			if((*it).second->getTeam() == 1) {
				(*it).second->setBattleID(count);
				newPlayers[count++] = (*it).second;
			}
		}
	}
	else if(speedTeam0 < speedTeam1)
	{
		for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) {
			if((*it).second->getTeam() == 1) {
				(*it).second->setBattleID(count);
				newPlayers[count++] = (*it).second;
			}
		}

		for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) {
			if((*it).second->getTeam() == 0) {
				(*it).second->setBattleID(count);
				newPlayers[count++] = (*it).second;
			}
		}
	}
	// Overwrite old players list with the new one
	battlePlayerList = newPlayers;
}

// Validate MOVE_RUN
void Battle::validateRun(Player* player)
{
	BattlePlayer *bPlayer = getBattlePlayerByPlayer(player);
	if(bPlayer == NULL) {
		return;
	}

	if(isWildBattle)
	{
		player->setMoveOption(MOVE_RUN);
		player->setMoveValidated(1);
		player->setPlayerStatus(PS_READY);

		std::string params[] = { "" };
		BattleEvents::_TextPacket(player, GT_BATTLEWAITING, params);
	}
	else 
	{
		BattleEvents::_ControlPacket(bPlayer, BATTLECONTROL_CHOOSEMOVE);

		std::string params[] = { bPlayer->getPokemonActive()->getName() };
		BattleEvents::_TextPacket(bPlayer, GT_POKEMONCHOOSEMOVE, params);
	}
}

// Change pokemon move
void Battle::validatePokemonChange(Player* player, int pokemonSlot)
{	
	BattlePlayer *bPlayer = getBattlePlayerByPlayer(player);
	if(bPlayer == NULL) {
		return;
	}

	if(player->getPokemon(pokemonSlot) && 
		player->getPokemon(pokemonSlot)->getPokemonHP_Left() > 0 &&
		player->getActivePokemonSlot() != pokemonSlot)
	{
		BattleEvents::SendBattlePokemonChangeAccept(bPlayer, pokemonSlot);

		player->setMoveOption(MOVE_POKEMONCHANGE);
		player->setMoveParameter(pokemonSlot);
		player->setMoveValidated(1);
		player->setPlayerStatus(PS_READY);
	}
	else
	{
		BattleEvents::_ControlPacket(bPlayer, BATTLECONTROL_CHOOSEMOVE);

		std::string params[] = { bPlayer->getPokemonActive()->getName() };
		BattleEvents::_TextPacket(bPlayer, GT_POKEMONCHOOSEMOVE, params);
	}
}

// Attack move validator
void Battle::validateAttack(Player* player, int attackSlot)
{
	int playerPokemonID = player->getActivePokemonSlot();

	BattlePlayer *bPlayer = getBattlePlayerByPlayer(player);

	if(bPlayer == NULL) {
		return;
	}	

	if(player->getPokemon(playerPokemonID)->getAttack(attackSlot))
	{
		int attackUsageLeft = player->getPokemon(playerPokemonID)->getAttack(attackSlot)->getUsageLeft();

		if(attackUsageLeft > 0)
		{
			BattleEvents::SendBattleMoveAccept(bPlayer, attackSlot);

			player->setMoveOption(MOVE_ATTACK);
			player->setMoveParameter(attackSlot);
			player->setMoveValidated(1);
			player->setPlayerStatus(PS_READY);
		}
		else
		{
			BattleEvents::_ControlPacket(bPlayer, BATTLECONTROL_CHOOSEMOVE);

			std::string params[] = { bPlayer->getPokemonActive()->getName() };
			BattleEvents::_TextPacket(bPlayer, GT_POKEMONCHOOSEMOVE, params);
		}
	}
	else
	{
		std::string params[] = { bPlayer->getPokemonActive()->getName() };
		BattleEvents::_TextPacket(bPlayer, GT_POKEMONCHOOSEMOVE, params);
	}
}

// Yes / No Answer
void Battle::validateAnswer(Player* player, int parameter)
{
	BattlePlayer *bPlayer = getBattlePlayerByPlayer(player);
	if(bPlayer == NULL) {
		return;
	}

	if(bPlayer->getPlayerStatus() == PS_ATTACKLEARN)
	{
		player->setMoveParameter(parameter);
		assignPokemonAttack(bPlayer);
	}
	else if(bPlayer->getPlayerStatus() == PS_ITEMCHOOSEPOKEMON)
	{
		if(parameter == -1) // Cancel
		{
			bPlayer->setPlayerStatus(PS_WAITING);

			BattleEvents::_ControlPacket(bPlayer, BATTLECONTROL_CHOOSEMOVE);

			std::string params[] = { bPlayer->getPokemonActive()->getName() };
			BattleEvents::_TextPacket(bPlayer, GT_POKEMONCHOOSEMOVE, params);
		}
		else
		{
			player->setMoveOption(MOVE_ITEM_POWERUP);
			long parm = (player->getMoveParameter() | (parameter << 16));
			player->setMoveParameter(parm);
			player->setMoveValidated(1);

			bPlayer->setPlayerStatus(PS_READY);
			BattleEvents::_TextPacket(bPlayer, GT_BATTLEWAITING);
		}
	}
	else if(freePokemonChange)
	{		
		if(parameter == BATTLEANSWER_NO)
		{
			player->setMoveOption(MOVE_NONE);
			player->setMoveParameter(0);
			player->setMoveValidated(1);
			player->setPlayerStatus(PS_READY);

			BattleEvents::_TextPacket(bPlayer, GT_BATTLEWAITING);
		}
		else if(parameter == BATTLEANSWER_YES)
		{
			BattleEvents::_ControlPacket(bPlayer, BATTLECONTROL_CHOOSEPOKEMON);
			BattleEvents::_TextPacket(bPlayer, GT_BATTLECHANGEPOKEMON);
		}
	}
}

void Battle::validateItem(Player* player, int _category, int _slot)
{
	BattlePlayer *bPlayer = getBattlePlayerByPlayer(player);
	if(bPlayer == NULL) {
		return;
	} 
	
	int itemIndex = Items::createIndex(_slot, _category);
	Item *item = player->getBackPack()->getItem(itemIndex);
	if(item != NULL && item->getCount() > 0)
	{
		if(item->getCategoryID() == ITEM_POKEBALL && isWildBattle)
		{
			player->setMoveOption(MOVE_ITEM_POKEBALL);
			player->setMoveParameter(itemIndex);
			player->setMoveValidated(1);
			player->setPlayerStatus(PS_READY);

			BattleEvents::_TextPacket(bPlayer, GT_BATTLEWAITING);

			return;
		}
		else if(item->getCategoryID() == ITEM_POWERUPS || item->getCategoryID() == ITEM_BERRIES)
		{
			player->setMoveParameter(itemIndex);
			player->setPlayerStatus(PS_ITEMCHOOSEPOKEMON);
			
			if(item->getPlusPP() > 0) {
				BattleEvents::_ControlPacket(bPlayer, BATTLECONTROL_CHOOSEATTACK_ITEM);
			}
			else {
				BattleEvents::_ControlPacket(bPlayer, BATTLECONTROL_CHOOSEPOKEMON_ITEM);
			}

			BattleEvents::_TextPacket(bPlayer, GT_BATTLEUSEITEMONPOKEMON);

			return;
		}
	}

	BattleEvents::_ControlPacket(bPlayer, BATTLECONTROL_CHOOSEMOVE);

	std::string params[] = { bPlayer->getPokemonActive()->getName() };
	BattleEvents::_TextPacket(bPlayer, GT_POKEMONCHOOSEMOVE, params);
}

void Battle::preBattleData()
{
	BOOST_FOREACH(BattleList::value_type it, battlePlayerList)
	{
		BattlePlayer *battlePlayer;
		int place = it.first;

		if(!(battlePlayer = it.second) || battlePlayer->getCreatureType() != PLAYER) {
			continue;
		}

		battlePlayer->setActivePokemon();

		std::string opponent_pokemonName = "";
		std::string own_pokemonName = "";
		std::string opponent_name = "";

		Packet preBattle;
		preBattle.AddByte(0xD0);
		preBattle.AddInt(battleType);
		preBattle.AddInt(getPlayersTotal());

		BOOST_FOREACH(BattleList::value_type it2, battlePlayerList)
		{
			BattlePlayer *iPlayer = it2.second;
			int iPlace = it2.first;

			if(!iPlayer) {
				continue;
			}
			
			if(place == iPlace) 
			{
				preBattle.AddInt(SELF);
				preBattle.AddInt(iPlace);
				preBattle.AddInt(iPlayer->getTeam());
				preBattle.AddInt(iPlayer->getPokemonActiveSlot()); // Slot of active pokemon 

				own_pokemonName = iPlayer->getPokemonActive()->getName();
			}
			else if(iPlayer->getType() == POKEMON || iPlayer->getType() == NPC)
			{
				preBattle.AddInt(iPlayer->getType());
				preBattle.AddInt(iPlace);
				preBattle.AddInt(iPlayer->getTeam());
				preBattle.AddInt(iPlayer->getPokemon(0)->getPokemonRealID());
				preBattle.AddString(iPlayer->getPokemon(0)->getName());
				preBattle.AddInt(iPlayer->getPokemon(0)->getPokemonLvl());

				opponent_pokemonName = iPlayer->getPokemon(0)->getName();
			}
			else if(iPlayer->getType() == PLAYER)
			{
				Pokemon *activePokemon = iPlayer->getPokemonActive();
				if(!activePokemon) {
					iPlayer->setActivePokemon();
					activePokemon = iPlayer->getPokemonActive();
				}

				double TotalPokemonHP = (iPlayer->getPokemonActive()->getPokemonHP_Total() == 0) ? 1 : iPlayer->getPokemonActive()->getPokemonHP_Total();
				int PokemonHP = (int)floor(((double)iPlayer->getPokemonActive()->getPokemonHP_Left() / TotalPokemonHP) * 100.0);

				preBattle.AddInt(iPlayer->getType());
				preBattle.AddInt(iPlace);
				preBattle.AddInt(iPlayer->getTeam());
				preBattle.AddString(iPlayer->getName());
				preBattle.AddInt(iPlayer->getPokemonActive()->getPokemonRealID());
				preBattle.AddString(iPlayer->getPokemonActive()->getName());
				preBattle.AddInt(iPlayer->getPokemonActive()->getPokemonLvl());
				preBattle.AddInt(PokemonHP);

				opponent_pokemonName = iPlayer->getPokemonActive()->getName();
				opponent_name = iPlayer->getName();
			}
		}

		battlePlayer->sendDataPacket(preBattle);

		// Send intro events
		std::vector<int> playList;
		if(isWildBattle) {
			BattleEvents::SendBattleIntro(battlePlayer, opponent_pokemonName, own_pokemonName);
			playList.push_back(14);
			playList.push_back(15);
		}
		else {
			BattleEvents::SendBattleIntro(battlePlayer, opponent_pokemonName, own_pokemonName, false, opponent_name);
			playList.push_back(16);
			playList.push_back(17);
		}

		battlePlayer->getCreature()->getPlayer()->sendMusicUpdate(SOUND_LIST, true, playList);
	}
}

void Battle::run()
{
#ifdef __BUGTRAP__
	BT_SetTerminate();
#endif // __BUGTRAP__

	battleState = STATE_WAITING;

	do
	{
		// Let's wait until all players have choosen an option
		if(battleState == STATE_WAITING)
		{
			battleState = STATE_RUNNING;
			BOOST_FOREACH(BattleList::value_type it, battlePlayerList)
			{
				BattlePlayer *tmp;

				if(tmp = it.second)
				{
					if(tmp->getPlayerStatus() == PS_WAITING || tmp->getPlayerStatus() == PS_ITEMCHOOSEPOKEMON)
					{
						if(tmp->getType() == POKEMON || tmp->getCreatureType() == NPC) {
							// Do AI thingy because it's a wild pokemon or NPC
							autoMoveChooser(tmp);
							tmp->setPlayerStatus(PS_READY);

							continue;
						}
						else if(tmp->getType() == PLAYER && tmp->isRemoved) {
							tmp->setMoveOption(MOVE_NONE);
							tmp->setMoveValidated(1);
							tmp->setPlayerStatus(PS_READY);

							continue;
						}

						// Set state back to waiting because not all players are ready
						battleState = STATE_WAITING;
					}
				}
				else
				{
					battlePlayerList.erase(it.first);
				}
			}

			if(!checkBattleContinue()) {
				break;
			}
			else {
				boost::this_thread::sleep(boost::posix_time::milliseconds(1000)); 
			}
		}
		// All players are ready, let's execute their battle options
		else if(battleState == STATE_RUNNING)
		{
			turnMoveID++;

			BOOST_FOREACH(BattleList::value_type it, battlePlayerList)
			{
				BattlePlayer *tmp;	
				if(tmp = it.second)
				{
					MoveType_t selectedOption = tmp->getMoveOption();

					if(tmp->isFainted) {
						tmp->isFainted = false;
						continue;
					}
					
					bool ret = false;
					switch(selectedOption)
					{
					case MOVE_RUN:
						//moveRun(tmp);
						break;
					case MOVE_ATTACK:
						moveAttack(tmp);
						break;
					case MOVE_POKEMONCHANGE:
						//moveChangePokemon(tmp);
						break;
					case MOVE_ITEM_POKEBALL:
						//ret = moveUsePokeball(tmp);
						break;
					case MOVE_ITEM_POWERUP:
						//moveUseItem(tmp);
						break;
					}

					if(selectedOption == MOVE_ITEM_POKEBALL && ret)	{
						break;
					}
				}
			}

			battleState = STATE_ROUNDFINISH;
		}
		else if(battleState == STATE_ROUNDFINISH)
		{			
			bool canContinue = true;
			// Check for attack learning
			BOOST_FOREACH(BattleList::value_type it, battlePlayerList)
			{
				BattlePlayer *playerIt;	
				if((playerIt = it.second) && (playerIt->getPlayerStatus() == PS_CANATTACKLEARN || playerIt->getPlayerStatus() == PS_ATTACKLEARN || playerIt->getPlayerStatus() == PS_ATTACKASSIGN) )
				{
					if(!playerIt->isRemoved && playerIt->getPlayerStatus() == PS_CANATTACKLEARN) // Seems like a pokemon can learn a new move, send this to the player
					{
						checkPokemonAttackLearn(playerIt);
					}
					canContinue = false;
				}
			}

			if(!canContinue) 
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(1000)); 

				battleState = STATE_ROUNDFINISH;
				continue; // Continue with main loop
			}

			// Check if battle has ended
			if(roundCleanUp()) {
				battleState = STATE_WAITING;
			}
			else {
				battleState = STATE_STOPPING;
			}
		}
	} while(battleState != STATE_STOPPING);

	endBattle(); //gayfag
}

// For NPC and wild pokemon
void Battle::autoMoveChooser(BattlePlayer *player)
{
	if(freePokemonChange)
	{
		player->setMoveOption(MOVE_NONE);
		player->setMoveParameter(0);
	}
	else
	{
		int totalAttacks = player->getPokemonActive()->getAttackCount();
		int attackSlot = 0;

		if(totalAttacks > 1 && player->getPokemonActive()->moveHistory.size() > 0)
		{
			std::map<int, TurnMoveInfo>	tmp = player->getPokemonActive()->moveHistory;
			std::map<int, TurnMoveInfo>::iterator it = --player->getPokemonActive()->moveHistory.end();
			int lastAttackID = it->second.attackID;
			int attackID = lastAttackID;

			attackSlot = 0;
			while(attackID == lastAttackID)
			{
				attackSlot = random_range(0, totalAttacks-1);
				attackID = player->getPokemonActive()->getAttack(attackSlot)->getAttackID();
			}
		}
		else
		{
			attackSlot = random_range(0, totalAttacks-1);
		}

		player->setMoveOption(MOVE_ATTACK);
		player->setMoveParameter(attackSlot);
	}

	player->setMoveValidated(1);
	player->setPlayerStatus(PS_FINISHED);
}

bool Battle::preMoveStatusCheck(BattlePlayer *player)
{
	// Check for flags and do damage?
	if(player->getFlags() != 0)
	{
		if(player->hasFlag(BPStatus_TURNLOCKED)) {
			player->removeFlag(BPStatus_TURNLOCKED);
			return false;
		}
	}

	if(player->getPokemonActive()->getFlags() != 0) 
	{
		
	}

	return true;
}

void Battle::moveRun(BattlePlayer *bPlayer)
{
	int runChance = random_range(0, 255);
	bool canRun = true;

	// Get defender
	BattlePlayer *defender = NULL;
	for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
	{
		BattlePlayer *tmp;	

		if((tmp = (*it).second) && tmp != bPlayer && tmp->getTeam() != bPlayer->getTeam())
		{	
			defender = tmp;
			
			if(tmp->getCreatureType() != POKEMON) {
				canRun = false;
			}
		}
	}

	double B = (double)defender->getPokemonActive()->getPokemonSpeed() / 4.;
	double F = (((double)bPlayer->getPokemonActive()->getPokemonSpeed() * 32.) / B) + 30. * 1.;

	if(F < 255 && runChance > F) {
		canRun = false;
	}

	for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
	{
		BattlePlayer *tmp;	
		if((tmp = (*it).second) && tmp->getCreatureType() == PLAYER)
		{
			BattleEvents::SendBattleAttack_RUN(tmp, canRun, bPlayer->getPokemonActive()->getName());

			if(canRun)
			{
				if(tmp == bPlayer) {
					// End battle
					BattleEvents::_EndBattlePacket(bPlayer);

					removePlayer(bPlayer);
				}
			}
		}
	}
}

void Battle::moveUseItem(BattlePlayer *bPlayer)
{
	int attackSlot = (char)(bPlayer->getMoveParameter() >> 24);
	int pokemonSlot = (char)(bPlayer->getMoveParameter() >> 16);
	int itemCat = (char)(bPlayer->getMoveParameter() >> 8);
	int itemSlot = (char)bPlayer->getMoveParameter();

	int itemIndex = Items::createIndex(itemSlot, itemCat);

	Player *player = bPlayer->getCreature()->getPlayer();
	Pokemon *pokemon = player->getPokemon(pokemonSlot);
	Item *item = player->getBackPack()->getItem(itemIndex);

	if(!player || !pokemon || !item) {
		return;
	}

	std::string sendText[] = { "", "", "" };
	std::string sendText2[] = { "", "" };

	sendText[0] = player->getName();
	sendText[1] = item->getName();
	sendText[2] = pokemon->getName();

	if(item->getPlusHP() > 0) {
		pokemon->increaseHP(item->getPlusPP());
		sendText2[0] = pokemon->getName();
		sendText2[1] = "HP";
	}
	if(item->getPlusPP() > 0) {
		Attack *attack = pokemon->getAttack(attackSlot);
		if(attack)
		{
			attack->setUsageLeft(attack->getUsageLeft() + item->getPlusPP());

			sendText2[0] = pokemon->getName();
			sendText2[1] = boost::str(boost::format("%s attack PP") % pokemon->getAttack(attackSlot)->getAttackName());
		}
	}

	//TODO: Add flinch things and check other stuff like happiness

	int hp = (int)((double)pokemon->getPokemonHP_Left() / (double)pokemon->getPokemonHP_Total());
	int hp_perc = (int)floor(hp * 100.);

	BattlePlayer *tmp = NULL;
	for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
	{
		if((tmp = (*it).second) && tmp->getType() == PLAYER)
		{
			BattleEvents::_TextPacket(tmp, GT_BATTLEITEMONPOKEMON, sendText);
			
			if(item->getPlusHP() > 0) {
				BattleEvents::_AnimationPacket(tmp, 0, ANIMATION_STATS, ANIMATION_STATS_UP);

				if(tmp == bPlayer) {
					BattleEvents::_ChangeHPPacket(tmp, bPlayer->getBattleID(), hp);
				}
				else {
					BattleEvents::_ChangeHPPacket(tmp, bPlayer->getBattleID(), hp_perc);
				}
			}
			else {
				if(tmp == bPlayer) {
					BattleEvents::_ChangePPPacket(bPlayer, bPlayer->getPokemonActiveSlot(), attackSlot, pokemon->getAttack(attackSlot)->getUsageLeft());				
				}
				BattleEvents::_SleepPacket(tmp, 1500);
			}

			BattleEvents::_TextPacket(tmp, GT_BATTLEITEMEFFECT_RESTORE, sendText2);
		}
	}

	player->getBackPack()->updateItem(itemIndex, -1);
}

bool Battle::moveUsePokeball(BattlePlayer *bPlayer)
{
	int itemCat = (char)(bPlayer->getMoveParameter() >> 8);
	int itemSlot = (char)bPlayer->getMoveParameter();
	int itemIndex = Items::createIndex(itemSlot, itemCat);

	Player *player = bPlayer->getCreature()->getPlayer();
	Item *item = player->getBackPack()->getItem(itemIndex);

	if(!player || !item || item->getCategoryID() != ITEM_POKEBALL) {
		return false;
	}

	// Get defender
	BattlePlayer *defender = NULL;
	for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
	{
		BattlePlayer *tmp;	

		if((tmp = (*it).second) && tmp != bPlayer && tmp->getTeam() != bPlayer->getTeam())
		{	
			defender = tmp;
			break;
		}
	}

	if(!defender) {
		player->setPlayerStatus(PS_FINISHED);
		return false;
	}

	int pokeball_id = item->getID();
	double catchRate = 1.0;

	if(pokeball_id == 3) {// great balls of fire
		catchRate = 1.5;
	}
	else if(pokeball_id == 2) {//ultra ball
		catchRate = 2.0;
	}
	else if(pokeball_id == 5) {// Safari ball
		catchRate = 0.0;
	}
	else if(pokeball_id == 7) { //Dive ball
		catchRate = 1.0;
		//TODO: Underwater check
	}
	else if(pokeball_id == 6) {//Net ball
		if(defender->getPokemonActive()->getType1() == "water" || defender->getPokemonActive()->getType2() == "bug") {
			catchRate = 3.0;
		}
	}
	else if(pokeball_id == 8) { // Nest ball
		int lvl = defender->getPokemonActive()->getPokemonLvl();
		if(lvl < 20) {
			catchRate = 3.0;
		}
		else if(lvl < 30) {
			catchRate = 2.0;
		}
	}
	else if(pokeball_id == 9) { // Repeat ball
		std::string query = boost::str(boost::format("SELECT cought FROM user_pokedex WHERE userID=%d AND pokemonID=%d AND cought=1") % player->getID() % defender->getPokemonActive()->getPokemonDbID());
		boost::scoped_ptr<Recordset> itemlist(new Recordset(db, query));

		if(itemlist->numOfRows() <= 0) {
			catchRate = 3.0;
		}
	}
	else if(pokeball_id == 10) { // Timer ball
		if(turnMoveID < 10) {
			catchRate = 1.0;
		}
		else if(turnMoveID < 20) {
			catchRate = 2.0;
		}
		else if(turnMoveID < 30) {
			catchRate = 3.0;
		}
		else {
			catchRate = 4.0;
		}
	}
	else if(pokeball_id == 13) { // Dusk ball
		if(player->getPosition().z == 1) {
			catchRate = 3.0;
		}
	}
	else if(pokeball_id == 15) { // Quick ball
		if(turnMoveID < 5) {
			catchRate = 4.0;
		}
		else if(turnMoveID <= 10) {
			catchRate = 3.0;
		}
		else if(turnMoveID <= 15) {
			catchRate = 2.0;
		}
	}
	
	double Status = 1.0; //TODO: Check status ailment
	Pokemon *pokemanz = defender->getPokemonActive();
	double a = ( ( ( ((double)pokemanz->getPokemonHP_Total() * 3.) - ((double)pokemanz->getPokemonHP_Left() * 2.) ) * (double)pokemanz->getCaptureRate() * catchRate) / ((double)pokemanz->getPokemonHP_Total() * 3.) ) * Status;
	
	bool isCatched = false;
	int n = 0;
	if(pokeball_id == 1 || a >= 255) { // Master Ball!
		isCatched = true;
	}
	else 
	{
		double b = 1048560 / sqrt(sqrt(16711680. / a));
		for(int i = 0; i < 4; i++)
		{
			int rnd = random_range(0, 65535);
			if(rnd <= b) {
				n++;
			}
		}

		if(n == 4) {
			isCatched = true;
		}
	}

	std::string param[] = { bPlayer->getName(), item->getName(), defender->getName() };
	BattleEvents::_TextPacket(bPlayer, GT_BATTLETHROWPOKEBALL, param);
	BattleEvents::_SleepPacket(bPlayer, (n*500));

	// Shake ball event here

	if(isCatched) 
	{
		defender->setPlayerStatus(PS_ISCATCHED);

		if(pokeball_id == 14 || player->getPokemonCount() == 6) { // Heal Ball or Full Party
			pokemanz->setPokemonHP_Left(pokemanz->getPokemonHP_Total());
		}

		std::stringstream newPokemon;
		newPokemon << "INSERT INTO user_pokemon (userID, pokemonID, canTrade, level, pkmn_hp, iv_hp, iv_at, iv_de, iv_sa, iv_sd, iv_sp, pkmn_exp, happiness, gender) VALUES"
				   << "(" << player->getID() << ","
				   << pokemanz->getPokemonDbID() << ",1,"
				   << pokemanz->getPokemonLvl() << ","
				   << (pokemanz->getPokemonHP_Total() - pokemanz->getPokemonHP_Left()) << ","
				   << pokemanz->iv_hp << ","
				   << pokemanz->iv_at << ","
				   << pokemanz->iv_de << ","
				   << pokemanz->iv_sa << ","
				   << pokemanz->iv_sd << ","
				   << pokemanz->iv_sp << ","
				   << pokemanz->getCurrentExp() << ",70,"
				   << pokemanz->getGender() << ")";
		db->runQuery(newPokemon.str());
		int uniquePokemonID = db->getLastID();

		// Insert moves into DB
		for(int i = 0; i < pokemanz->getAttackCount(); i++)
		{
			Attack *attack = pokemanz->getAttack(i);
			if(attack)
			{
				std::string newAttack = boost::str(boost::format("INSERT INTO user_pokemon_moves (uniqueID, moveID) VALUES (%d, %d)") % uniquePokemonID % attack->getAttackID());
				db->runQuery(newAttack);
			}
		}

		std::cout << "Yeey! You catched a Pokemanz named: " << pokemanz->getName() << std::endl; // Yeey!
		std::string paramCaught[] = { pokemanz->getName() };
		BattleEvents::_TextPacket(bPlayer, GT_BATTLEPOKEMON_CAUGHT, paramCaught);		

		// Got a spot left in party?
		if(player->getPokemonCount() < 6) 
		{
			int slot = player->getPokemonCount();
			player->addNewPokemonToParty(slot ,uniquePokemonID);
				
			std::string slotUpdate = boost::str(boost::format("UPDATE user SET User_Pkmn%d = %d WHERE User_ID=%d") % (slot+1) % uniquePokemonID % player->getID());
			db->runQuery(slotUpdate);

			std::cout << "- Pokemon has been put in party slot: " << slot << std::endl; // Yeey!
			player->client->sendPokemonData(player->getPokemonParty());
		}
		else { 
			// Send text message; pokemon has been transported to depot box
		}
	}
	else 
	{
		std::cout << "Aww the Pokemanz escaped from the pokeball" << std::endl;

		std::string paramCaught[] = { pokemanz->getName(), item->getName() };
		BattleEvents::_TextPacket(bPlayer, GT_BATTLEPOKEMON_ESCAPED, paramCaught);

		// Send pokemon catch fail event
	}

	player->getBackPack()->updateItem(itemIndex, -1);

	return isCatched;
}

void Battle::moveAttack(BattlePlayer *attacker)
{
	// Check active pokemon HP level
	if(attacker->getPokemonActive()->getPokemonHP_Left() <= 0) {
		return;
	}
	
	// Check for status modifiers damage thingy
	if(!preMoveStatusCheck(attacker))
	{
		attacker->setPlayerStatus(PS_FINISHED);
		return;
	}

	// Get defender
	BattlePlayer *defender = NULL;
	for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
	{
		BattlePlayer *tmp;	
			
		if((tmp = (*it).second) && tmp != attacker && tmp->getTeam() != attacker->getTeam())
		{	
			defender = tmp;
			break;
		}
	}

	if(!defender) {
		attacker->setPlayerStatus(PS_FINISHED);
		return;
	}

	StatCalculator statCalc;

	int attackSlot = attacker->getMoveParameter();
	Pokemon *attackPokemon = attacker->getPokemonActive();
	Pokemon *defendPokemon = defender->getPokemonActive();

	if(!defendPokemon || defendPokemon->getPokemonHP_Left() <= 0) {
		attacker->setPlayerStatus(PS_FINISHED);
		return;
	}

	if(attackPokemon->hasFlag(StatusAilment_ENCORE)) 
	{	
		if(attackPokemon->encoreRounds <= 0 || attackPokemon->getAttack(attackSlot)->getUsageLeft() <= 0) {
			attackPokemon->removeFlag(StatusAilment_ENCORE);
		}
		else {
			attackSlot = attackPokemon->getLastTurnInfo().slotID;
			attackPokemon->encoreRounds--;
		}
	}

	// Use attack
	attackPokemon->getAttack(attackSlot)->useAttack();

	// Attack data
	int pokemonAccuracy_at = attackPokemon->getAttack(attackSlot)->getAttackAccuracy();
	int accuracyRand = random_range(1, 100);
	int AttackDamage = 0;
	int attackID = attackPokemon->getAttack(attackSlot)->getAttackID();

	int attackReturn[6] = { STAT_NONE, STATMOD_NONE, STAT_NONE, STATMOD_NONE, STAT_NONE, 1 };

	bool attackMissed = false;
	bool isDisabled = false;
	bool isCharging = false;

	if(attackID == 12 || attackID == 32 || attackID == 90 || attackID == 329) // Guillotine, Horn Drill, Fissure, Sheer Cold
	{
		if(attackPokemon->getPokemonLvl() - defendPokemon->getPokemonLvl()) {
			attackMissed = true;
		} else {
			accuracyRand = (attackPokemon->getPokemonLvl() - defendPokemon->getPokemonLvl()) + 30;
		}

	}
	else if(attackID == 197) // Detect
	{
		if(attackPokemon->getLastAttackID() == 197) {
			pokemonAccuracy_at = 50;
		}
	}
	else if(attackID == 332) { // Aerial Ace
		pokemonAccuracy_at = 100;
	}

	if(attackPokemon->getAttack(attackSlot)->getDisable()) {
		isDisabled = true;
	}
	else if(attackID == 345 || attackID == 443) { // Magical Leaf, Magnet Bomb
		attackMissed = false;
	}
	else if(pokemonAccuracy_at < accuracyRand || attackPokemon->hasFlag(StatusAilment_FLINCH)) {
		attackMissed = true;
	}
	else if(attackID == 252) // Fake Out
	{
		std::map<int, TurnMoveInfo> attackList = attackPokemon->moveHistory;
		for (std::map<int, TurnMoveInfo>::iterator it = attackList.begin(); it != attackList.end(); ++it) 
		{
			TurnMoveInfo tmp = it->second;
			if(tmp.attackID == 252) {
				attackMissed = true;
			}
		}
	}
	else if(attackID == 264) // Focus Punch
	{
		TurnMoveInfo info = defendPokemon->getLastTurnInfo();
		if(info.turnID == turnMoveID && info.damage > 0 && info.foe == attacker->getBattleID() && info.attackID != 220) { // Not Pain Split
			attackMissed = true;
		}
	}
	else if(attackID == 303) // Slack Off
	{
		int extraHP = attackPokemon->getPokemonHP_Total() >> 1;
		attackPokemon->increaseHP(extraHP);
	}
	else if(attackID == 327) // Sky Uppercut
	{
		attackMissed = true;
		if((defendPokemon->getLastAttackID() == 19 || defendPokemon->getLastAttackID() == 340) && defender->getAttackCharge() > 0)
		{
			attackMissed = false;
		}
	}
	else if(attackID == 387) // Last Resort
	{
		// Check if other attacks have PP left
		std::map<int, std::string> list = attackPokemon->getAttackList();
		for(std::map<int, std::string>::iterator a = list.begin(); a != list.end(); ++a)
		{
			if(attackPokemon->getAttack(a->first)->getUsageLeft() > 0) {
				attackMissed = true;
				break;
			}
		}
	}
	else if(defendPokemon->hasFlag(StatusAilment_DETECT) && 
		(attackID == 367 || attackID == 174 || attackID == 353 || 
		attackID == 248 || attackID == 286 || attackID == 195 || 
		attackID == 244 || attackID == 272 || attackID == 161) )
	{
		attackMissed = true;
	}

	if(attackPokemon->hasFlag(StatusAilment_LOCK_ON)) 
	{
		attackPokemon->removeFlag(StatusAilment_LOCK_ON);
		attackMissed = false;
	}
	else if(attackPokemon->hasFlag(StatusAilment_MIND_READER))
	{
		attackPokemon->removeFlag(StatusAilment_MIND_READER);
		attackMissed = false;
	}
	else if(attackPokemon->hasFlag(StatusAilment_TAUNT))
	{
		// Assist, Copycat, Me First, Metronome, Mirror Move, Sleep Talk
		if(attackID == 274 || attackID == 383 || attackID == 382 || attackID == 118 || attackID == 119 || attackID == 214) 
		{
			attackMissed = true;
		}
	}
	else if(attackPokemon->hasFlag(StatusAilment_TORMENT))
	{
		if(attacker->getAttackCharge() == 0 && attackID == attackPokemon->getLastAttackID()) {
			attackMissed = true;
		}
	}
	
	if(!attackMissed)
	{
		if (attackID == 109) {
			defendPokemon->setFlag(StatusAilment_CONFUSION);	

			attackReturn[2] = STAT_CONFUSED;
			attackReturn[3] = STATMOD_NONE;
		}
		else if(attackID == 12 || attackID == 32 || attackID == 90) { // Guillotine Horn Drill, Fissure
			AttackDamage = defendPokemon->getPokemonHP_Total();
		}
		else if(attackID == 13) // Razor Wind
		{
			if(attacker->getAttackCharge() == 0) 
			{
				attacker->setAttackCharge(2);
				isCharging = true;
			}
			else if(attacker->getAttackCharge() == 1) 
			{
				attacker->setAttackCharge(0);
				AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
			}			
		}
		else if(attackID == 19) // Fly
		{
			if(attacker->getAttackCharge() == 0) 
			{
				attacker->setAttackCharge(2);
				isCharging = true;
			}
			else if(attacker->getAttackCharge() == 1) 
			{
				attacker->setAttackCharge(0);
				AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
			}
		}
		else if(attackID == 49) // Sonicboom
		{
			AttackDamage = 20;
		}
		else if(attackID == 50) // Disable
		{
			std::map<int, TurnMoveInfo> attackList = defendPokemon->moveHistory;
			if(attackList.size() > 0)
			{
				for (std::map<int, TurnMoveInfo>::iterator it = attackList.begin(); it != attackList.end(); ++it) 
				{
					TurnMoveInfo tmp = it->second;
					if(!tmp.hit) {
						attackMissed = true;
					}
				}
			}
			else {
				attackMissed = true;
			}

			if(!attackMissed)
			{
				attackMissed = true;
				int lastAttackID = defendPokemon->getLastAttackID();
				for(int i = 0; i < defendPokemon->getAttackCount(); i++)
				{
					if(defendPokemon->getAttack(i)->getAttackID() == lastAttackID && defendPokemon->getAttack(i)->getAttackPower() > 0)
					{
						defendPokemon->getAttack(i)->setDisable(true);
						attackMissed = false;
						break;
					}
				}
			}
		}
		else if(attackID == 68) // Counter
		{
			if(defendPokemon->getType1() == "ghost" || defendPokemon->getType2() == "ghost") {
				attackMissed = true;
			}
			else
			{
				// Get last physical attack
				int damageDone = 0;
				std::map<int, TurnMoveInfo> attackList = attackPokemon->moveHistory;
				for (std::map<int, TurnMoveInfo>::iterator it = attackList.begin(); it != attackList.end(); ++it) 
				{
					TurnMoveInfo tmp = it->second;
					if(tmp.isPhysical) {
						damageDone = tmp.damage;
					}
				}

				AttackDamage = damageDone * 2;
			}
		}
		else if(attackID == 69) // Seismic Toss
		{
			if(defendPokemon->getType1() == "ghost" || defendPokemon->getType2() == "ghost") {
				attackMissed = true;
			}
			else {
				AttackDamage = attackPokemon->getPokemonLvl();
			}
		}
		else if(attackID == 76) // SolarBeam
		{
			if(attacker->getAttackCharge() == 0) 
			{
				attacker->setAttackCharge(2);
				isCharging = true;
			}
			else if(attacker->getAttackCharge() == 1) 
			{
				attacker->setAttackCharge(0);
				AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
			}			
		}
		else if(attackID == 82) // Dragon Rage
		{
			AttackDamage = 40;
		}	

		else if(attackID == 91) // Dig
		{
			if(attacker->getAttackCharge() == 0) 
			{
				attacker->setAttackCharge(2);
				isCharging = true;
			}
			else if(attacker->getAttackCharge() == 1) 
			{
				attacker->setAttackCharge(0);
				AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
			}
		}
		else if(attackID == 100) // Teleport
		{
			for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
			{
				if((*it).second != attacker && (*it).second->getType() != POKEMON) 
				{
					attackMissed = true;
					break;
				}
			}
		}
		else if(attackID == 101) // Night Shade
		{
			if(defendPokemon->getType1() == "normal" || defendPokemon->getType2() == "normal") {
				attackMissed = true;
			}
			else {
				AttackDamage = attackPokemon->getPokemonLvl();
			}
		}
		else if(attackID == 105) // Recover
		{
			int recover = attackPokemon->getPokemonHP_Total() >> 1; // +50%
			attackPokemon->increaseHP(recover);
		}
		else if(attackID == 107) { // Minimize
			attackPokemon->setFlag(StatusAilment_MINIMIZE);
		}
		else if(attackID == 114) 
		{
			for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
			{
				BattlePlayer *playerIt;
				if((playerIt = (*it).second) && playerIt->getCreatureType() == PLAYER)
				{
					for(int i = 0; i < playerIt->getPokemonTotal(); i++)
					{
						playerIt->getPokemon(i)->resetStatModifiers();
					}
				}
			}
		}
		else if(attackID == 130) // Skull Bash
		{
			if(attacker->getAttackCharge() == 0) {
				attacker->setAttackCharge(2);
				isCharging = true;
			}
			else if(attacker->getAttackCharge() == 1)
			{
				attacker->setAttackCharge(0);
				
				AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
			}
		}
		else if(attackID == 143) // Sky Attack
		{
			if(attacker->getAttackCharge() == 0) {
				attacker->setAttackCharge(2);
				isCharging = true;
			}
			else if(attacker->getAttackCharge() == 1)
			{
				attacker->setAttackCharge(0);

				AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
			}
		}
		else if(attackID == 149) // Psywave
		{
			double randomN = (random_range(0, 10) + 5) * (attackPokemon->getPokemonLvl() / 10);
			AttackDamage = (randomN < 1) ? 1 : (int)floor(randomN);
		}
		else if(attackID == 162) // Super Fang
		{
			double tmp = defendPokemon->getPokemonHP_Left() / 2;
			int tmp2 = (int)floor(tmp);
			AttackDamage = (tmp2 < 0) ? 0 : tmp2;
		}
		else if(attackID == 169) // Spider Web
		{
			defender->setFlag(BPStatus_SWITCHLOCK);
		}
		else if(attackID == 117)
		{
			if(attacker->getAttackCharge() == 0) { // Charging
				attacker->setAttackCharge(3);
				isCharging = true;
			}
			else if(attacker->getAttackCharge() == 1)
			{
				attacker->setAttackCharge(0);
				int firstTurn = turnMoveID - 2;
				int totalDamage = 0;
				std::map<int, TurnMoveInfo> attackList = attackPokemon->moveHistory;
				for (std::map<int, TurnMoveInfo>::iterator it = attackList.begin(); it != attackList.end(); ++it) 
				{
					TurnMoveInfo tmp = it->second;
					if(tmp.turnID >= firstTurn && tmp.attackID != 220) { // Not pain split
						totalDamage += tmp.damage;
					}
				}

				AttackDamage = totalDamage * 2;
			}
			else {
				isCharging = true;
			}
		}
		else if(attackID == 135) // Softboiled
		{
			int extraHP = attackPokemon->getPokemonHP_Total() >> 1;
			attackPokemon->increaseHP(extraHP);
		}
		else if(attackID == 156) // Rest
		{
			if(attacker->getAttackCharge() == 0) { // Charging
				attacker->setAttackCharge(3);
				isCharging = true;
			}
			else if(attacker->getAttackCharge() == 1)
			{
				attacker->setAttackCharge(0);
				if(attackPokemon->hasFlag(StatusAilment_BURN)) {
					attackPokemon->removeFlag(StatusAilment_BURN);
				}
				if(attackPokemon->hasFlag(StatusAilment_PARALYSIS)) {
					attackPokemon->removeFlag(StatusAilment_PARALYSIS);
				}
				if(attackPokemon->hasFlag(StatusAilment_POISON)) {
					attackPokemon->removeFlag(StatusAilment_POISON);
				}
				if(attackPokemon->hasFlag(StatusAilment_POISON_BAD)) {
					attackPokemon->removeFlag(StatusAilment_POISON_BAD);
				}

				attackPokemon->setPokemonHP_Left(attackPokemon->getPokemonHP_Total());
			}
			else {
				isCharging = true;
			}
		}
		else if(attackID == 162) // Super Fang
		{
			int AttackDamage = defendPokemon->getPokemonHP_Total() >> 1;
		}
		else if(attackID == 164) // Substitude
		{
			attackPokemon->setFlag(StatusAilment_SUBSITUTE);
		}
		else if(attackID == 165) // Struggle
		{
			for(int i = 0; i < attackPokemon->getAttackCount(); i++)
			{
				Attack *attack = attackPokemon->getAttack(i);
				if(attack && attack->getAttackID() != attackID && attack->getUsageLeft() > 0) 
				{
					attackMissed = true;
				}
			}

			if(!attackMissed)
			{
				AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);

				int recoil = attackPokemon->getPokemonHP_Total() >> 2;
				attackPokemon->decreaseHP(recoil);
			}
		}
		else if(attackID == 170) {
			attackPokemon->setFlag(StatusAilment_MIND_READER);
		}
		else if(attackID == 171)
		{
			if(!defendPokemon->hasFlag(StatusAilment_SLEEP)) {
				attackMissed = true;
			}
		}
		else if(attackID == 173)
		{
			if(attackPokemon->hasFlag(StatusAilment_SLEEP))
			{
				AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
			}
			else {
				attackMissed = true;
			}
		}
		else if(attackID == 174)
		{
			if(defendPokemon->hasFlag(StatusAilment_CURSE)) {
				attackMissed = true;
			}
			else {
				defendPokemon->setFlag(StatusAilment_CURSE);
				if(attackPokemon->getType1() == "ghost" || attackPokemon->getType2() == "ghost") {
					int halfHP = attackPokemon->getPokemonHP_Left() / 2;
					attackPokemon->decreaseHP(halfHP);
				}
				else
				{
					attackPokemon->setModAt(1);
					attackPokemon->setModDe(1);
					attackPokemon->setModSp(-1);
				}
			}
		}
		else if(attackID == 180) // Spite
		{
			TurnMoveInfo info = defendPokemon->getLastTurnInfo();
			if(info.turnID == 0 || !info.hit) {
				attackMissed = true;
			}
			else {
				Attack *attack = defendPokemon->getAttack(info.slotID);
				attack->setUsageLeft(attack->getUsageLeft() - 4);
			}
		}
		else if(attackID == 187) // Belly Drum
		{
			int tmpHP = attackPokemon->getPokemonHP_Total() / 2;
			attackPokemon->decreaseHP(tmpHP);

			if(attackPokemon->getModAt() < 6) {
				attackPokemon->setModAt(6);
			}
			else {
				attackMissed = true;
			}
		}
		else if(attackID == 193) // Foresight
		{
			defendPokemon->setFlag(StatusAilment_FORESIGHT);
		}
		else if(attackID == 194) // Destiny Bond
		{
			defendPokemon->setFlag(StatusAilment_DESTINY_BOND);
		}
		else if(attackID == 197) // Detect
		{
			attackPokemon->setFlag(StatusAilment_DETECT);
		}
		else if(attackID == 199) // Lock-On
		{
			attackPokemon->setFlag(StatusAilment_LOCK_ON);
		}
		else if(attackID == 200 || attackID == 80 || attackID == 37) // Outrage, Petal Dance, Trash
		{
			AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);

			if(attacker->getAttackCharge() <= 0) {
				int rnd = random_range(2,3);
				attacker->setAttackCharge(rnd);
			}
			else if(attacker->getAttackCharge() == 1) {
				attackPokemon->setFlag(StatusAilment_CONFUSION);	

				attackReturn[0] = STAT_CONFUSED;
				attackReturn[1] = STATMOD_NONE;
			}
		}
		else if(attackID == 203) // Endure
		{
			attackPokemon->setFlag(StatusAilment_ENDURE);
		}
		else if(attackID == 208)
		{
			int restoreHP = attackPokemon->getPokemonHP_Total() >> 1;
			attackPokemon->increaseHP(restoreHP);
		}
		else if(attackID == 210) // Fury Cutter
		{
			AttackDamage = 10;
			std::map<int, TurnMoveInfo> attackList = attackPokemon->moveHistory;
			for (std::map<int, TurnMoveInfo>::iterator it = attackList.begin(); it != attackList.end(); ++it) 
			{
				TurnMoveInfo tmp = it->second;
				if(tmp.attackID == 210) 
				{
					if(tmp.hit) {
						AttackDamage = AttackDamage * 2;
					}
					else {
						AttackDamage = 10;
					}
				}
			}
			if(AttackDamage > 160) {
				AttackDamage = 160;
			}
		}
		else if(attackID == 212) {
			defender->setFlag(BPStatus_SWITCHLOCK);
		}
		else if(attackID == 220) // Pain Split
		{
			AttackDamage = 0;
			double tmp = (attackPokemon->getPokemonHP_Left() + defendPokemon->getPokemonHP_Left()) / 2;
			int AverageHP = (int)floor(tmp);
			defendPokemon->setPokemonHP_Left(AverageHP);
			attackPokemon->setPokemonHP_Left(AverageHP);
		}
		else if(attackID == 227) // Encore
		{
			if(!defendPokemon->doneBattleAttack || defender->getMoveOption() == MOVE_ATTACK) {
				attackMissed = true;
			}
			else
			{
				// Get last attack info
				TurnMoveInfo lastAttack = defendPokemon->getLastTurnInfo();
				if(!lastAttack.hit || 
					// Transform, Mimic, Sketch, Mirror Move, Encore, or Struggle
					(lastAttack.attackID == 144 || lastAttack.attackID == 102 || lastAttack.attackID == 166 || 
					lastAttack.attackID == 119 || lastAttack.attackID == 227 || lastAttack.attackID == 165)) 
				{
					attackMissed = true;
				}
				else if(defendPokemon->getAttack(lastAttack.slotID)->getUsageLeft() <= 0) {
					attackMissed = true;
				}
				else
				{
					defendPokemon->setFlag(StatusAilment_ENCORE);
					int numofrounds = random_range(4,8);
					defendPokemon->encoreRounds = numofrounds;
				}
			}
		}
		else if (attackID == 229) // Rapid Spin
		{
			if(attackPokemon->hasFlag(StatusAilment_LEECH_SEED)) {
				attackPokemon->removeFlag(StatusAilment_LEECH_SEED);
			}

			AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
		}
		else if(attackID == 234 || attackID == 235 || attackID == 236) { // Morning Sun, Synthesis, Moonlight (No weather)
			int extrahp = attackPokemon->getPokemonHP_Total() >> 1;
			attackPokemon->increaseHP(extrahp);
		}
		else if(attackID == 238) // Endeavor
		{
			if(defendPokemon->getType1() == "ghost" || defendPokemon->getType2() == "ghost") {
				attackMissed = true;
			}

			int attackHP = attackPokemon->getPokemonHP_Left();
			int defendHP = defendPokemon->getPokemonHP_Left();

			if(defendHP > attackHP) {
				defendPokemon->setPokemonHP_Left(attackHP);
			}
		}
		else if(attackID == 243) // Mirror Coat
		{
			if(defendPokemon->getType1() == "dark" || defendPokemon->getType2() == "dark") {
				attackMissed = true;
			}
			else {
				TurnMoveInfo info = defendPokemon->getLastTurnInfo();
				if(info.foe == attacker->getBattleID() && info.damage > 0) {
					AttackDamage = info.damage << 1;
				}
				else {
					attackMissed = true;
				}
			}
		}
		else if(attackID == 253) // Uproar
		{
			if(attacker->getAttackCharge() == 0)
			{
				int r = random_range(2,5);
				attacker->setAttackCharge(r);
			}

			AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
		}
		else if(attackID == 259) // Torment
		{
			defendPokemon->setFlag(StatusAilment_TORMENT);
		}
		else if(attackID == 269) // Taunt
		{
			defendPokemon->setFlag(StatusAilment_TAUNT);
		}
		else if(attackID == 273) // Whish
		{
			if(attacker->getAttackCharge() == 0) {
				attacker->setAttackCharge(3);
			}
			else if(attacker->getAttackCharge() == 1)
			{
				int extraHP = attackPokemon->getPokemonHP_Total() >> 1;
				attackPokemon->increaseHP(extraHP);
			}
		}
		else if(attackID == 280) // Brick Break
		{
			if(defendPokemon->hasFlag(StatusAilment_LIGHT_SCREEN)) {
				defendPokemon->removeFlag(StatusAilment_LIGHT_SCREEN);
			}

			if(defendPokemon->hasFlag(StatusAilment_REFLECT)) {
				defendPokemon->removeFlag(StatusAilment_REFLECT);
			}

			AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
		}
		else if(attackID == 281) // Yawn
		{
			if(attacker->getAttackCharge() == 0) {
				attacker->setAttackCharge(3);
			}
			else if(attacker->getAttackCharge() == 1)
			{
				defendPokemon->setFlag(StatusAilment_SLEEP);
			}
		}
		else if(attackID == 283) // Edeavor
		{ 
			AttackDamage = defendPokemon->getPokemonHP_Left() - attackPokemon->getPokemonHP_Left();
			if(AttackDamage < 1) {
				attackMissed = true;
				AttackDamage = 0;
			}
		}
		else if(attackID == 284) {
			AttackDamage = 150 * attackPokemon->getPokemonHP_Left() / attackPokemon->getPokemonHP_Total();
		}
		else if(attackID == 287) // Refresh
		{
			if(attackPokemon->hasFlag(StatusAilment_BURN)) {
				attackPokemon->removeFlag(StatusAilment_BURN);
			}
			if(attackPokemon->hasFlag(StatusAilment_PARALYSIS)) {
				attackPokemon->removeFlag(StatusAilment_PARALYSIS);
			}
			if(attackPokemon->hasFlag(StatusAilment_POISON)) {
				attackPokemon->removeFlag(StatusAilment_POISON);
			}
			if(attackPokemon->hasFlag(StatusAilment_POISON_BAD)) {
				attackPokemon->removeFlag(StatusAilment_POISON_BAD);
			}
		}
		else if(attackID == 291) // Dive
		{
			if(attacker->getAttackCharge() == 0) { // Charging
				attacker->setAttackCharge(2);
				isCharging = true;
			}
			else if(attacker->getAttackCharge() == 1)
			{
				attacker->setAttackCharge(0);
				AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
			}
		}
		else if(attackID == 300) // Mud Sport
		{
			attackPokemon->setFlag(StatusAilment_MUD_SPORT);
		}
		else if(attackID == 307) { // Blast Burn
			attacker->setFlag(BPStatus_TURNLOCKED);
		}
		else if(attackID == 215 || attackID == 312) // Heal Bell, Aromatherapy
		{
			for(int i = 0; i < 6; i++)
			{
				Pokemon *tmpPkmn;
				if(tmpPkmn = attacker->getPokemon(i))
				{
					if(tmpPkmn->hasFlag(StatusAilment_BURN)) {
						tmpPkmn->removeFlag(StatusAilment_BURN);
					}
					if(tmpPkmn->hasFlag(StatusAilment_PARALYSIS)) {
						tmpPkmn->removeFlag(StatusAilment_PARALYSIS);
					}
					if(tmpPkmn->hasFlag(StatusAilment_FREEZE)) {
						tmpPkmn->removeFlag(StatusAilment_FREEZE);
					}
					if(tmpPkmn->hasFlag(StatusAilment_POISON)) {
						tmpPkmn->removeFlag(StatusAilment_POISON);
					}
					else if(tmpPkmn->hasFlag(StatusAilment_POISON_BAD)) {
						tmpPkmn->removeFlag(StatusAilment_POISON_BAD);
					}
					if(tmpPkmn->hasFlag(StatusAilment_CONFUSION)) {
						tmpPkmn->removeFlag(StatusAilment_CONFUSION);
					}
					if(tmpPkmn->hasFlag(StatusAilment_SLEEP)) {
						tmpPkmn->removeFlag(StatusAilment_SLEEP);
					}
				}
			}
		}
		else if(attackID == 316) // Odor Sleuth
		{
			defendPokemon->setFlag(StatusAilment_ODOR_SLEUTH);
		}
		else if(attackID == 329) // Sheer Cold
		{
			AttackDamage = defendPokemon->getPokemonHP_Total();
		}
		else if(attackID == 335) { // Block
			if(defender->hasFlag(BPStatus_SWITCHLOCK)) {
				attackMissed = true;
			}
			else {
				defender->setFlag(BPStatus_SWITCHLOCK);
			}
		}
		else if(attackID == 338) { // Frenzy Plant
			defender->setFlag(BPStatus_TURNLOCKED);
			AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
		}
		else if(attackID == 340) // Bounce
		{
			if(attacker->getAttackCharge() == 0) 
			{
				attacker->setAttackCharge(2);
				isCharging = true;
			}
			else if(attacker->getAttackCharge() == 1) 
			{
				attacker->setAttackCharge(0);
				AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
			}
		}
		else if(attackID == 353)
		{
			if(attacker->getAttackCharge() == 0) 
			{
				attacker->setAttackCharge(3);
				isCharging = true;
			}
			else if(attacker->getAttackCharge() == 1) 
			{
				attacker->setAttackCharge(0);
				AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
			}
			else {
				isCharging = true;
			}
		}
		else if(attackID == 355) // Roost
		{
			int recover = attackPokemon->getPokemonHP_Total() >> 1;
			attackPokemon->increaseHP(recover);
		}
		else if(attackID == 357) // Miricle Eye
		{
			attackPokemon->setFlag(StatusAilment_MIRACLE_EYE);
		}
		else if(attackID == 358) // Wake-Up Slap
		{
			if(defendPokemon->hasFlag(StatusAilment_SLEEP))
			{
				defendPokemon->removeFlag(StatusAilment_SLEEP);
			}
			
			AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
		}
		else if(attackID == 361 || attackID == 461) // Healing Wish , Lunar Dance
		{
			attackPokemon->setPokemonHP_Left(0);
			attacker->setFlag(BPStatus_HEALINGWISH);
		}
		else if(attackID == 367) // Acupressure
		{
			int statList[6];
			int count = 0;

			if(attackPokemon->getModAc() < 6) { statList[count] = STAT_ACCURACY; count++; }
			if(attackPokemon->getModAt() < 6) { statList[count] = STAT_ATTACK; count++; }
			if(attackPokemon->getModDe() < 6) { statList[count] = STAT_DEFENSE; count++; }
			if(attackPokemon->getModSa() < 6) { statList[count] = STAT_SPECIALATTACK; count++; }
			if(attackPokemon->getModSd() < 6) { statList[count] = STAT_SPECIALDEFENSE; count++; }
			if(attackPokemon->getModSp() < 6) { statList[count] = STAT_SPEED; count++; }

			if(count == 0) {
				attackMissed = true;
			}
			else
			{
				int rnd = random_range(1, count) - 1;
				switch(statList[rnd])
				{
				case STAT_ACCURACY:
					attackPokemon->setModAc(2);
					break;
				case STAT_ATTACK:
					attackPokemon->setModAt(2);
					break;
				case STAT_DEFENSE:
					attackPokemon->setModDe(2);
					break;
				case STAT_SPECIALATTACK:
					attackPokemon->setModSa(2);
					break;
				case STAT_SPECIALDEFENSE:
					attackPokemon->setModSd(2);
					break;
				case STAT_SPEED:
					attackPokemon->setModSp(2);
					break;
				}

				attackReturn[1] = statList[rnd];
				attackReturn[2] = STATMOD_SHARPLYROSE;
			}
		}
		else if(attackID == 368) // Metal Burst
		{
			TurnMoveInfo lastAttack = defendPokemon->getLastTurnInfo();
			if(lastAttack.attackID != 0 && lastAttack.turnID == turnMoveID && lastAttack.hit) {
				AttackDamage = (double)lastAttack.damage * 1.5;
			} 
			else
			{
				attackMissed = true;
			}
		}
		else if(attackID == 384) // Power Swap
		{
			int oldAt = attackPokemon->getModAt();
			int oldDe = attackPokemon->getModDe();

			attackPokemon->setModAt(oldDe);
			attackPokemon->setModDe(oldAt);
		}
		else if(attackID == 391)
		{
			int oldAc = attackPokemon->getModAc();
			int oldAt = attackPokemon->getModAt();
			int oldDe = attackPokemon->getModDe();
			int oldSa = attackPokemon->getModSa();
			int oldSd = attackPokemon->getModSd();
			int oldSp = attackPokemon->getModSp();

			attackPokemon->setModAc(defendPokemon->getModAc());
			attackPokemon->setModAt(defendPokemon->getModAt());
			attackPokemon->setModDe(defendPokemon->getModDe());
			attackPokemon->setModSa(defendPokemon->getModSa());
			attackPokemon->setModSd(defendPokemon->getModSd());
			attackPokemon->setModSp(defendPokemon->getModSp());

			defendPokemon->setModAc(oldAc);
			defendPokemon->setModAt(oldAt);
			defendPokemon->setModDe(oldDe);
			defendPokemon->setModSa(oldSa);
			defendPokemon->setModSd(oldSd);
			defendPokemon->setModSp(oldSp);
		}
		else if(attackID == 439) // Rock Wrecker
		{
			defender->setFlag(BPStatus_TURNLOCKED);
		}
		else if(attackID == 456) // Heal Order
		{
			int extraHP = attackPokemon->getPokemonHP_Total() >> 1;
			attackPokemon->increaseHP(extraHP);
		}
		else if(attackID == 459) // Roar of Time
		{
			defender->setFlag(BPStatus_TURNLOCKED);
		}
		else if(attackID == 381) // Lucky Chant
		{
			StatusItem_s statusItem;
			statusItem.foeID = attacker->getBattleID();
			statusItem.totalDamage = 0;
			statusItem.turnCount = 5;

			defendPokemon->statusTurnList.insert(std::make_pair<int, StatusItem_s>(attackID, statusItem));
		}
		else if(attackID == 383) // Copy-cat
		{
			attackMissed = true;
		}
		else if(attackID == 385) // Guard Swap
		{
			int tmpDef = attackPokemon->getModDe();
			int tmpSDef = attackPokemon->getModSd();

			attackPokemon->resetModDe(defendPokemon->getModDe());
			attackPokemon->resetModSd(defendPokemon->getModSd());
			defendPokemon->resetModDe(tmpDef);
			defendPokemon->resetModDe(tmpSDef);
		}
		else if(attackID == 416) 
		{
			defender->setFlag(BPStatus_TURNLOCKED);
		}
		else if(attackID == 432) // Defog
		{
			if(attackPokemon->hasFlag(StatusAilment_REFLECT)) {
				attackPokemon->removeFlag(StatusAilment_REFLECT);
			}
			if(attackPokemon->hasFlag(StatusAilment_LIGHT_SCREEN)) {
				attackPokemon->removeFlag(StatusAilment_LIGHT_SCREEN);
			}
		}
		else if(attackID == 90 || attackID == 12 || attackID == 32 || attackID == 329) 
		{
			AttackDamage = defendPokemon->getPokemonHP_Total();
		}
		else if(attackID == 150) {
			AttackDamage = 0;
		}
		else 
		{
			AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
		}	

		if(attackID == 111) // Defence Curl
		{
			std::map<int, TurnMoveInfo> attackList = attackPokemon->moveHistory;
			for (std::map<int, TurnMoveInfo>::iterator it = attackList.begin(); it != attackList.end(); ++it) 
			{
				TurnMoveInfo tmp = it->second;
				if(tmp.attackID == 301  || tmp.attackID == 205) { // Ice Ball or Rollout
					AttackDamage = AttackDamage * 2;
				}
			}
		}

		if(attackID == 63 || attackID == 308) {
			attacker->setFlag(BPStatus_TURNLOCKED);
		}

		//////////////////////////////////////////////////////////////////////////
		// End Pre HP stuff
		defendPokemon->decreaseHP(AttackDamage);

		if(attackID == 36 || attackID == 66) // Take Down, Submission
		{
			int recoil = AttackDamage >> 2;
			attackPokemon->decreaseHP(recoil);
		}
		else if(attackID == 71 || attackID == 72 || attackID == 141 || attackID == 202 || attackID == 409 || // Absorb, Mega Drain, Giga Drain, Leech Life, Drain Punch
			(attackID == 138 && defendPokemon->hasFlag(StatusAilment_SLEEP)) // Dream Eater (only when sleep)
			){ 
			int extraHP = AttackDamage >> 1;
			attackPokemon->increaseHP(extraHP);
		}
		else if(attackID == 38 || attackID == 344 || attackID == 394 || attackID == 413 || attackID == 452) // Double-Edge, Volt Tackle, Flare Blitz, Brave Bird, Wood Hammer
		{
			int removeHP = (int)floor((double)AttackDamage / 3.);
			attackPokemon->decreaseHP(removeHP);
		}
		else if(attackID == 457) {
			int removeHP = AttackDamage >> 1;
			attackPokemon->decreaseHP(removeHP);
		}
		else if(attackID == 120 || attackID == 153 || attackID == 262) { // Selfdestruct, Explosion, Memento
			attackPokemon->setPokemonHP_Left(0);
		}
		else if(attackID == 206 && defendPokemon->getPokemonHP_Left() <= 0) { // False Swipe
			defendPokemon->setPokemonHP_Left(1);
		}
	}

	if(attackMissed && (attackID == 26 || attackID == 136)) // Jump Kick, Hi-Jump Kick
	{
		AttackDamage = statCalc.calculateAttackDamage(attackPokemon, defendPokemon, attackSlot, attackReturn, turnMoveID);
		if(AttackDamage > 0) {
			int removeHP = AttackDamage >> 1;
			attackPokemon->decreaseHP(removeHP);
		}
	}

	if(defendPokemon->getPokemonHP_Left() <= 0)
	{
		TurnMoveInfo info = defendPokemon->getLastTurnInfo();
		if(info.turnID == turnMoveID && info.attackID == 288) // Grudge
		{
			for(int a = 0; a < attackPokemon->getAttackCount(); a++)
			{
				if(attackPokemon->getAttack(a)->getAttackID() == attackPokemon->getLastAttackID()) 
				{
					attackPokemon->getAttack(a)->setUsageLeft(0);
					BattleEvents::_ChangePPPacket(attacker, attacker->getPokemonActiveSlot(), a, 0);

					break;
				}
			}
		}
	}
	
	// Add current move to history
	TurnMoveInfo newMove;
	newMove.foe = defendPokemon->getPokemonID();
	newMove.turnID = turnMoveID;
	newMove.attackID = attackPokemon->getAttack(attackSlot)->getAttackID();
	newMove.damage = AttackDamage;
	newMove.hit = attackMissed;
	newMove.playerID = defender->getBattleID();
	newMove.isPhysical = (attackPokemon->getAttack(attackSlot)->getContact() == "yes");
	newMove.slotID = attackSlot;

	attackPokemon->addMoveHistory(turnMoveID, newMove);
	attackPokemon->doneBattleAttack = true;

	// Check if pokemon has done somekind of healing attack in the previous rounds
	std::map<int, TurnMoveInfo> attackList = attackPokemon->moveHistory;
	for (std::map<int, TurnMoveInfo>::iterator it = attackList.begin(); it != attackList.end(); ++it) 
	{
		TurnMoveInfo tmp = it->second;
			// ????					Ingrain
		if((tmp.attackID == 392 || tmp.attackID == 275 ) && attackPokemon->getPokemonHP_Left() != attackPokemon->getPokemonHP_Total()) {
			int extraHP = attackPokemon->getPokemonHP_Total() / 16;
			attackPokemon->increaseHP(extraHP);
		}
	}

	if(attackPokemon->hasFlag(StatusAilment_CONFUSION)) {
		attackPokemon->decreaseHP(1000);
		AttackDamage = 1000;
	}

	double self = (double)attackPokemon->getPokemonHP_Left() / (double)attackPokemon->getPokemonHP_Total();
	int HP_Perc_Attack = (int)floor(self * 100.);
	double enemy = (double)defendPokemon->getPokemonHP_Left() / (double)defendPokemon->getPokemonHP_Total();
	int HP_Perc_Enemy = (int)floor(enemy * 100.);

	std::stringstream strsHitAmount;
	strsHitAmount << attackReturn[5];

	for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
	{
		BattlePlayer *playerIt;
		if((playerIt = (*it).second) && playerIt->getCreatureType() == PLAYER)
		{
			// <player name>'s turn
			if(attacker->getType() == PLAYER || attacker->getType() == NPC) 
			{
				std::string params[] = { attacker->getName() };
				BattleEvents::_TextPacket(playerIt, GT_BATTLEPLAYERTURN, params);
			}

			if(attackPokemon->hasFlag(StatusAilment_CONFUSION)) {
				std::string params[] = { attackPokemon->getName() };
				BattleEvents::_TextPacket(playerIt, GT_BATTLE_CONFUSED, params);
				BattleEvents::_SleepPacket(playerIt, 2000);
				BattleEvents::_TextPacket(playerIt, GT_BATTLE_CONFUSED_HURT, params);
			} else {
				// <pokemon> uses <attack>
				std::string params[] = { attackPokemon->getName(), attackPokemon->getAttack(attackSlot)->getAttackName() };
				BattleEvents::_TextPacket(playerIt, GT_POKEMONATTACK, params);

				if(attackID == 150) { // Splash
					BattleEvents::_SleepPacket(playerIt, 2000);
					BattleEvents::_TextPacket(playerIt, GT_BATTLE_BUTNOTHINGHAPPENED);
				} else if (attackID == 109) {
					std::string params[] = { defendPokemon->getName() };
					BattleEvents::_SleepPacket(playerIt, 2000);
					BattleEvents::_TextPacket(playerIt, GT_BATTLE_CONFUSED, params);
				}
			}

			if(attackMissed)
			{
				if(attackPokemon->hasFlag(StatusAilment_FLINCH))
				{
					// <pokemon> flinched
					std::string params[] = { attackPokemon->getName() };
					BattleEvents::_TextPacket(playerIt, GT_POKEMONFLINCHED, params);
				}
				else 
				{
					// <pokemon> attack missed
					std::string params[] = { attackPokemon->getName() };
					BattleEvents::_TextPacket(playerIt, GT_BATTLEATTACKMISSED, params);
				}
			}
			else if(isDisabled)
			{
				std::string params[] = { attackPokemon->getAttack(attackSlot)->getAttackName() };
				BattleEvents::_TextPacket(playerIt, GT_BATTLEATTACKDISABLED, params);
			}

			if(isCharging)
			{
				std::string params2[] = { attackPokemon->getName() };
				BattleEvents::_TextPacket(playerIt, GT_BATTLEATTACKCHARGING, params2);
			}

			BattleEvents::_SleepPacket(playerIt, 2000);

			if(playerIt == attacker) // It's ME!! Mario!
			{
				if(!attackMissed && !isCharging && !isDisabled && AttackDamage > 0)
				{
					if (attackPokemon->hasFlag(StatusAilment_CONFUSION)) {
						BattleEvents::_AnimationPacket(playerIt, 0, ANIMATION_ATTACK, attackPokemon->getAttack(attackSlot)->getAttackID());
					} 
					else {
						BattleEvents::_AnimationPacket(playerIt, 1, ANIMATION_ATTACK, attackPokemon->getAttack(attackSlot)->getAttackID());
					}
					
					if(attackReturn[5] > 1) {
						std::string params2[] = { strsHitAmount.str() };
						BattleEvents::_TextPacket(playerIt, GT_BATTLEATTACKMULTIHIT, params2);
					}

					BattleEvents::_ChangeHPPacket(playerIt, defender->getBattleID(), HP_Perc_Enemy);
					BattleEvents::_ChangeHPPacket(playerIt, attacker->getBattleID(), attackPokemon->getPokemonHP_Left());
				}
				
				BattleEvents::_ChangePPPacket(playerIt, attacker->getPokemonActiveSlot(), attackSlot, attackPokemon->getAttack(attackSlot)->getUsageLeft());				
				BattleEvents::_ChangePokemonStatus(playerIt, attacker->getBattleID(), attackPokemon->getStatus());
				BattleEvents::_ChangePokemonStatus(playerIt, defender->getBattleID(), defendPokemon->getStatus());
			}
			else
			{
				if(!attackMissed && !isCharging && !isDisabled && AttackDamage > 0)
				{
					if(playerIt == defender)
					{
						if (attackPokemon->hasFlag(StatusAilment_CONFUSION)) {
							BattleEvents::_AnimationPacket(playerIt, 1, ANIMATION_ATTACK, attackPokemon->getAttack(attackSlot)->getAttackID());
						} 
						else {
							BattleEvents::_AnimationPacket(playerIt, 0, ANIMATION_ATTACK, attackPokemon->getAttack(attackSlot)->getAttackID());
						}
						
						if(attackReturn[5] > 1) {
							std::string params2[] = { strsHitAmount.str() };
							BattleEvents::_TextPacket(playerIt, GT_BATTLEATTACKMULTIHIT, params2);
						}

						BattleEvents::_ChangeHPPacket(playerIt, defender->getBattleID(), defendPokemon->getPokemonHP_Left());
						BattleEvents::_ChangeHPPacket(playerIt, attacker->getBattleID(), HP_Perc_Attack);
					}
					else
					{
						BattleEvents::_AnimationPacket(playerIt, 1, ANIMATION_ATTACK, attackPokemon->getAttack(attackSlot)->getAttackID());	
						
						if(attackReturn[5] > 1) {
							std::string params2[] = { strsHitAmount.str() };
							BattleEvents::_TextPacket(playerIt, GT_BATTLEATTACKMULTIHIT, params2);
						}

						BattleEvents::_ChangeHPPacket(playerIt, defender->getBattleID(), HP_Perc_Enemy);
						BattleEvents::_ChangeHPPacket(playerIt, attacker->getBattleID(), HP_Perc_Attack);
					}
				}
				
				BattleEvents::_ChangePokemonStatus(playerIt, attacker->getBattleID(), attackPokemon->getStatus());
				BattleEvents::_ChangePokemonStatus(playerIt, defender->getBattleID(), defendPokemon->getStatus());
			}

			if(attackReturn[0] != STAT_NONE) {
				BattleEvents::SendBattleAttackBonus(playerIt, attackReturn[0]);
			}

			if(attackReturn[1] != STAT_NONE && attackReturn[2] != STATMOD_NONE) {
				BattleEvents::SendBattleModifier(playerIt, attackReturn[1], attackReturn[2], attackPokemon->getName(), 1);
			}

			if(attackReturn[3] != STAT_NONE && attackReturn[4] != STATMOD_NONE) {
				BattleEvents::SendBattleModifier(playerIt, attackReturn[3], attackReturn[4], defendPokemon->getName(), 0);
			}
		}
	}

	// Check for after effects
	if(attackPokemon->getPokemonHP_Left() > 0)
	{
		StatusList afterEffectList = attackPokemon->statusTurnList;
		StatusList::iterator iter = afterEffectList.begin(), lim = afterEffectList.end();
		while(iter != lim)
		{
			int atckID = iter->first;
			StatusItem_s statusInfo = iter->second;

			doAttackTurnDamage(atckID, statusInfo, attacker);
			statusInfo.turnCount--;

			if(statusInfo.turnCount == 0) {
				afterEffectList.erase(iter++);
			}
			else {
				++iter;
			}
		}
	}

	// Add post-attack status
	if(!attackMissed && defendPokemon->getPokemonHP_Left() > 0) {
		addAttackTurnDamage(attackID, defender, defendPokemon, attacker->getBattleID(), AttackDamage);
	}

	// Disable Flinch
	if(attackPokemon->hasFlag(StatusAilment_FLINCH)) {
		attackPokemon->removeFlag(StatusAilment_FLINCH);
	}

	if(isCharging) {
		attacker->setAttackCharge(attacker->getAttackCharge() - 1);
	}

	attacker->setPlayerStatus(PS_FINISHED);

	// If attacker used Telport
	if(attackID == 100 && !attackMissed) {
		removePlayer(attacker);
	}
	else {
		checkFaintedPokemon(attacker, defender);
	}
}

bool Battle::checkStatusFlags(BattlePlayer *player)
{
	Pokemon *pokemon = player->getPokemonActive();
	bool showMessage = false;
	bool canAttack = true;
	int attackID = 0;
	std::string params[] = { pokemon->getName(), "" };

	if(pokemon->hasFlag(StatusAilment_CURSE)) 
	{
		showMessage = true;
		params[1] = "Curse";
		attackID = 174;

		int forthHP = (int)floor((double)pokemon->getPokemonHP_Left() / 4.);
		pokemon->decreaseHP(forthHP);
	}

	if(showMessage)
	{
		double att = (double)pokemon->getPokemonHP_Left() / (double)pokemon->getPokemonHP_Total();
		int HP_Perc = (int)floor(att * 100.);

		for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
		{
			BattlePlayer *playerIt;
			if((playerIt = (*it).second) && playerIt->getCreatureType() == PLAYER)
			{
				BattleEvents::_TextPacket(playerIt, GT_POKEMONHURTBY, params);

				if(playerIt->getBattleID() == player->getBattleID())
				{
					BattleEvents::_AnimationPacket(playerIt, 1, ANIMATION_ATTACK, attackID);

					BattleEvents::_ChangeHPPacket(playerIt, player->getBattleID(), pokemon->getPokemonHP_Left());
				}
				else
				{
					BattleEvents::_AnimationPacket(playerIt, 0, ANIMATION_ATTACK, attackID);
					BattleEvents::_ChangeHPPacket(playerIt, player->getBattleID(), HP_Perc);
				}
			}
		}
	}

	return canAttack;
}

void Battle::doAttackTurnDamage(int attackID, StatusItem_s statusInfo, BattlePlayer *attacker)
{
	int totalDamage = statusInfo.totalDamage;
	int damage = 0;
	std::string attackParams[] = { attacker->getPokemonActive()->getName(), "" };

	BattlePlayer *player = NULL;
	int HP_Perc_Defend;

	if(attackID == 128) {
		damage = totalDamage / 16;
		attackParams[1] = "Wrap";
	}	
	else if(attackID == 73) {
		damage = totalDamage / 8;
		attackParams[1] = "Leech Seed";

		player = getPlayer(statusInfo.foeID);
		player->getPokemonActive()->increaseHP(damage);

		int HP_Perc_Defend = (int)floor(((double)player->getPokemonActive()->getPokemonHP_Left() / (double)player->getPokemonActive()->getPokemonHP_Total()) * 100.);
	}
	else if(attackID == 83) {
		damage = totalDamage / 16;
		attackParams[1] = "Fire Spin";
	}	
	else if(attackID == 92) {
		damage = attacker->getPokemonActive()->getPokemonHP_Total() >> 4;
		int extra = 16 - statusInfo.turnCount;
		damage = damage * extra;
		attackParams[1] = "Toxic";
	}
	else if(attackID == 128) {
		damage = totalDamage / 16;
		attackParams[1] = "Clamp";
	}
	else if(attackID == 171) {
		damage = totalDamage / 4;
		attackParams[1] = "Nightmare";
	}
	else if(attackID == 250) {
		damage = totalDamage / 16;
		attackParams[1] = "Whirlpool";
	}
	else if(attackID == 328) {
		damage = totalDamage / 16;
		attackParams[1] = "Sand Tomb";
	}
	else if(attackID == 463) {
		damage = totalDamage / 16;
		attackParams[1] = "Magma Storm";
	}
	else {
		return;
	}

	Pokemon *attackPokemon = attacker->getPokemonActive();
	attackPokemon->decreaseHP(damage);

	int HP_Perc_Attack = (int)floor(((double)attackPokemon->getPokemonHP_Left() / (double)attackPokemon->getPokemonHP_Total()) * 100.);
	for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
	{
		BattlePlayer *playerIt;
		if((playerIt = (*it).second) && playerIt->getCreatureType() == PLAYER) 
		{
			BattleEvents::_TextPacket(playerIt, GT_POKEMONHURTBY, attackParams);

			if(playerIt->getBattleID() == attacker->getBattleID()) {
				BattleEvents::_AnimationPacket(playerIt, 0, ANIMATION_ATTACK, attackID);
				BattleEvents::_ChangeHPPacket(playerIt, attacker->getBattleID(), attackPokemon->getPokemonHP_Left());
			}
			else {
				BattleEvents::_AnimationPacket(playerIt, 1, ANIMATION_ATTACK, attackID);	
				BattleEvents::_ChangeHPPacket(playerIt, attacker->getBattleID(), HP_Perc_Attack);
			}

			if(attackID == 73) 
			{
				if(playerIt->getBattleID() == player->getBattleID()) {
					BattleEvents::_ChangeHPPacket(playerIt, player->getBattleID(), player->getPokemonActive()->getPokemonHP_Left());
				}
				else {
					BattleEvents::_ChangeHPPacket(playerIt, player->getBattleID(), HP_Perc_Attack);
				}
			}
		}
	}
}

void Battle::addAttackTurnDamage(int attackID, BattlePlayer *defender, Pokemon *defendPokemon, int enemyID, int attackDamage)
{
	StatusList afterEffectList = defendPokemon->statusTurnList;
	StatusList::iterator it = afterEffectList.find(attackID);
	if(it != afterEffectList.end())	{
		return;
	}

	if(attackID == 35 || attackID == 463 || attackID == 128 || attackID == 83 || attackID == 73 || attackID == 250 || attackID == 328) // Wrap, Magama Storm, Clamp, Fire Spin, Leech Seed, Whirlpool, Sand Tomb
	{
		StatusItem_s statusItem;
		statusItem.foeID = enemyID;
		statusItem.totalDamage = attackDamage;
		statusItem.turnCount = random_range(2,5);

		defender->setFlag(BPStatus_SWITCHLOCK);
		defendPokemon->statusTurnList.insert(std::make_pair<int, StatusItem_s>(attackID, statusItem));
	}
	else if(attackID == 92) // Toxic
	{
		StatusItem_s statusItem;
		statusItem.foeID = enemyID;
		statusItem.totalDamage = attackDamage;
		statusItem.turnCount = 15;

		defendPokemon->statusTurnList.insert(std::make_pair<int, StatusItem_s>(attackID, statusItem));
	}
	else if(attackID == 171 && defendPokemon->hasFlag(StatusAilment_SLEEP)) 
	{
		StatusItem_s statusItem;
		statusItem.foeID = enemyID;
		statusItem.totalDamage = defendPokemon->getPokemonHP_Total();
		statusItem.turnCount = 999;		

		defendPokemon->statusTurnList.insert(std::make_pair<int, StatusItem_s>(attackID, statusItem));
	}
}

void Battle::moveChangePokemon(BattlePlayer *player)
{
	// Check for status modifiers damage thingy
	if(!preMoveStatusCheck(player) || player->hasFlag(BPStatus_SWITCHLOCK))
	{
		player->setPlayerStatus(PS_FINISHED);
		return;
	}

	// Remove Encore
	if(player->getPokemonActive()->hasFlag(StatusAilment_ENCORE)) {
		player->getPokemonActive()->removeFlag(StatusAilment_ENCORE);
	}

	Pokemon *oldPokemon = player->getPokemonActive();

	// Check if the player has placed any spells which wear off when switching pokemon
	std::map<int, TurnMoveInfo>attackList = oldPokemon->moveHistory;
	for (std::map<int, TurnMoveInfo>::iterator it = attackList.begin(); it != attackList.end(); ++it) 
	{
		TurnMoveInfo tmp = it->second;
		if(getPlayer(tmp.playerID) == NULL) {
			continue;
		}

		if(tmp.attackID == 83 || tmp.attackID == 128 || tmp.attackID == 335 || tmp.attackID == 463) { // FireSpin, Clamp Block, ??, Magma Storm
			getPlayer(tmp.playerID)->removeFlag(BPStatus_SWITCHLOCK);
		}
	}

	player->setPokemon(player->getMoveParameter());
	player->setPlayerStatus(PS_FINISHED);

	if(player->hasFlag(BPStatus_HEALINGWISH)) 
	{
		player->removeFlag(BPStatus_HEALINGWISH);
		player->getPokemonActive()->resetStatModifiers();
		player->getPokemonActive()->setPokemonHP_Left(player->getPokemonActive()->getPokemonHP_Total());
	}

	// Send change to other players
	for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
	{
		BattlePlayer *playerIt;	
		if((playerIt = (*it).second) && playerIt->getCreatureType() == PLAYER)
		{
			std::string params[] = { player->getName(), player->getPokemonActive()->getName() };
			BattleEvents::_TextPacket(playerIt, GT_BATTLEPOKEMONCHANGED, params);

			if(playerIt->getBattleID() == player->getBattleID())
			{
				BattleEvents::_ChangePokemon(player, player->getPokemonActiveSlot());
			}
			else
			{		
				int HP_Left = (int)floor(((double)player->getPokemonActive()->getPokemonHP_Left() / (double)player->getPokemonActive()->getPokemonHP_Total()) * 100.0);

				BattleEvents::_ChangePokemon(playerIt, 
					player->getBattleID(), 
					player->getPokemonActive()->getPokemonRealID(), 
					player->getPokemonActive()->getName(), 
					HP_Left,
					player->getPokemonActive()->getPokemonLvl());
			}
		}
	}
}

void Battle::checkFaintedPokemon(BattlePlayer *attacker, BattlePlayer *defender)
{
	if(defender->getPokemonActive()->getPokemonHP_Left() <= 0) {
		pokemonFainted(attacker, defender);
	}
	else if(attacker->getPokemonActive()->getPokemonHP_Left() <= 0) {
		pokemonFainted(defender, attacker);
	}
}

void Battle::pokemonFainted(BattlePlayer *winner, BattlePlayer *loser)
{
	loser->getPokemonActive()->setStatus(-1); // Fainted
	loser->isFainted = true;

	// Calculate gained exp
	bool isWild = (loser->getType() == POKEMON) ? true : false;
	int wonEXP = StatCalculator::Calculate_EXP(loser->getPokemonActive()->getBaseExp(), loser->getPokemonActive()->getPokemonLvl(), isWild);

	int totalPokemon = 0;
	int pkmnCount = winner->getPokemonTotal();
	for(int i = 0; i < 6; i++)
	{
		if(winner->getPokemon(i) != NULL && winner->getPokemon(i)->doneBattleAttack && winner->getPokemon(i)->getStatus() == -1) {
			totalPokemon++;
		}
	}

	if(totalPokemon == 0) { totalPokemon = 1; }
	int newWonExp = (int)floor((double)(wonEXP / totalPokemon));
	std::stringstream newWonExpStr;
	newWonExpStr << newWonExp;

	BOOST_FOREACH(BattleList::value_type it, battlePlayerList)
	{
		BattlePlayer *playerIt;	

		if((playerIt = it.second) &&  playerIt->getCreatureType() == PLAYER)
		{
			std::string params[] = { loser->getPokemonActive()->getName() };
			BattleEvents::_TextPacket(playerIt, GT_BATTLEPOKEMONFAINTED, params);
			BattleEvents::_SleepPacket(playerIt, 3000);

			/*if(playerIt == winner) // You are the winrar! Now do some cool stuff which need more optimalization
			{			
				BOOST_FOREACH(PokemonList::value_type itPkmn, winner->getPokemonList())
				{
					int i = itPkmn.first;
					Pokemon *pokemon = itPkmn.second;

					if(pokemon && pokemon->doneBattleAttack && pokemon->getStatus() != -1)
					{
						//int TotalExpNextLvl = StatCalculator::Calculate_LevelEXP((winner->getPokemon(i)->getPokemonLvl()+1));
						
						// Increase experience
						int pkmnLvl = pokemon->increaseExp(newWonExp);
						std::stringstream pkmnLvlStr;
						pkmnLvlStr << pkmnLvl;

						// Update experience
						std::string params2[] = { pokemon->getName(), newWonExpStr.str() };
						BattleEvents::_TextPacket(playerIt, GT_BATTLEPOKEMONEXP, params2);
						int expPercent = pokemon->getExpPercent();
						BattleEvents::_ExperiencePacket(playerIt, i, expPercent);

						BattleEvents::_SleepPacket(playerIt, 2000);
						
						if(pkmnLvl > 0) // Pokemon level'd up
						{
							std::string params3[] = { pokemon->getName(), pkmnLvlStr.str() };
							BattleEvents::_TextPacket(playerIt, GT_BATTLEPOKEMONLVLUP, params3);
							BattleEvents::_ChangePokemonLevel(playerIt, true, i, pkmnLvl);
							
							playerIt->getCreature()->getPlayer()->client->sendPokemonData(playerIt->getCreature()->getPokemonParty());
							BattleEvents::_SleepPacket(playerIt, 2000);
							
							BOOST_FOREACH(BattleList::value_type itInt, battlePlayerList)
							{
								BattlePlayer *tmp;	
								if((tmp = itInt.second) && tmp->getCreatureType() == PLAYER && tmp->getBattleID() != winner->getBattleID())
								{								
									BattleEvents::_TextPacket(tmp, GT_BATTLEPOKEMONLVLUP, params3);
									BattleEvents::_ChangePokemonLevel(tmp, false, winner->getBattleID(), pkmnLvl);
									
									double att = (double)pokemon->getPokemonHP_Left() / (double)pokemon->getPokemonHP_Total();
									int HP_Perc_Attack = (int)floor(att * 100.);									
									BattleEvents::_ChangeHPPacket(tmp, winner->getBattleID(), HP_Perc_Attack);
									BattleEvents::_SleepPacket(tmp, 3000);	
								}
							}

							// Check for evolution
							stringstream evolutionQuery;
							evolutionQuery << "SELECT id, evo_param FROM pokemon WHERE evo_parent_id='" << winner->getPokemon(i)->getPokemonDbID() << "' AND evo_method='level'";
							Recordset *evoRst = new Recordset(db, evolutionQuery.str());

							if(evoRst->numOfRows() > 0 && evoRst->moveNext() && evoRst->getInt("evo_param") == pkmnLvl)
							{
								//TODO: What's this...
							}

							delete evoRst;


							// Check for new attack learning
							stringstream attackLearnQuery;
							attackLearnQuery << "SELECT m.id, m.name, m.pp, m.type FROM pokemon_moves `pm` LEFT JOIN moves `m` ON m.id = pm.move_id WHERE"
								<< " pm.method='level' AND pm.pokemon_id='" << pokemon->getPokemonDbID() << "' AND level='" << pokemon->getPokemonLvl() << "'";
							Recordset *rst = new Recordset(db, attackLearnQuery.str().c_str());

							if(rst->numOfRows() > 0 && rst->moveNext())
							{
								winner->setPlayerStatus(PS_CANATTACKLEARN);
								winner->getPokemon(i)->attackLearning = true;
								winner->getPokemon(i)->attackLearningID = rst->getInt("id");
								winner->getPokemon(i)->attackLearningName = rst->getString("name");
							}

							delete rst;
						}

						// Reset
						pokemon->doneBattleAttack = false;
					}
				}

				// TODO: Change this when we have economical system
				// Give moneyz if it's a wild battle
				if(isWildBattle)
				{
					// Get player average pokemon lvl
					int averageLvl = 0;
					for(int i = 0; i < winner->getPokemonTotal(); i++) {
						if(winner->getPokemon(i)) {
							averageLvl += winner->getPokemon(i)->getPokemonLvl();
						}
					}

					averageLvl = (int)floor((double)averageLvl / (double)winner->getPokemonTotal());
					int moneyz = (loser->getPokemonActive()->getPokemonLvl() - averageLvl) + 50;
					if(moneyz <= 0) {
						moneyz = random_range(1, 20);
					}
					winner->getCreature()->getPlayer()->addMoney(moneyz);

					std::stringstream goldz;
					goldz << moneyz;
					std::string params5[] = { goldz.str() };
					BattleEvents::_TextPacket(winner, GT_BATTLE_WINGOLDZ, params5);
					BattleEvents::_SleepPacket(winner, 2000);
				}
			}*/
		}
	}

	Pokemon *attackPokemon = winner->getPokemonActive();
	if(attackPokemon->hasFlag(StatusAilment_DESTINY_BOND))
	{
		Pokemon *defendPokemon = loser->getPokemonActive();
		std::map<int, TurnMoveInfo> attackList = defendPokemon->moveHistory;
		for (std::map<int, TurnMoveInfo>::iterator it = attackList.begin(); it != attackList.end(); ++it) 
		{
			TurnMoveInfo tmp = it->second;
			if(tmp.attackID == 194) // Check if user has done Destiny Bond
			{
				attackPokemon->decreaseHP(attackPokemon->getPokemonHP_Total());

				BOOST_FOREACH(BattleList::value_type itPlayer, battlePlayerList)
				{
					BattlePlayer *playerIt;
					if((playerIt = itPlayer.second) && playerIt->getCreatureType() == PLAYER)
					{
						std::string params[] = { attackPokemon->getName(), "Destiny Bond" };
						BattleEvents::_TextPacket(playerIt, GT_POKEMONHURTBY, params);						
						BattleEvents::_ChangeHPPacket(playerIt, winner->getBattleID(), 0);
					}
				}
				break;
			}
		}
		attackPokemon->removeFlag(StatusAilment_DESTINY_BOND);
	}
}

bool Battle::roundCleanUp()
{
	BattleStatus battleStatus[20];
	freePokemonChange = false;
	BattlePlayer *playerIt;	
	int countPlayersContinue = 0;
	bool isEndOfBattle = false;

	// Loop all players and make a status for each of them
	BOOST_FOREACH(BattleList::value_type itPlayer, battlePlayerList)
	{
		if((playerIt = itPlayer.second))
		{			
			if(playerIt->getPokemonActive()->hasFlag(StatusAilment_ENDURE)) {
				playerIt->getPokemonActive()->removeFlag(StatusAilment_ENDURE);
			}
			
			int playerID = itPlayer.first;

			if(playerIt->isRemoved) {
				battleStatus[playerID] = BATTLESTATUS_ENDLOST;
				isEndOfBattle = true;
				continue;
			}

			if(playerIt->getPlayerStatus() == PS_ISCATCHED) {
				return false;
			}

			battleStatus[playerID] = BATTLESTATUS_CONTINUE;
			countPlayersContinue++;

			// Do we have more pokemon to play with?
			if(playerIt->getPokemonActive()->getPokemonHP_Left() <= 0 || !playerIt->getPokemonActive()->hasUsableAttack()) // Pokemon fainted or no more usable attacks PP
			{
				if(playerIt->checkPokemonNext()) {
					battleStatus[playerID] = BATTLESTATUS_CHANGEPOKEMON;
					freePokemonChange = true;
				}
				else // Can't play any further
				{ 
					isEndOfBattle = true;
					if(!playerIt->getPokemonActive()->hasUsableAttack()) {
						battleStatus[playerID] = BATTLESTATUS_FORFEIT;
					}
					else {
						battleStatus[playerID] = BATTLESTATUS_ENDLOST;
					}
					countPlayersContinue--;
				}
			}
		}
	}

	// Send status to all players
	BOOST_FOREACH(BattleList::value_type itPlayer, battlePlayerList)
	{
		if(playerIt = itPlayer.second)
		{
			int playerID = itPlayer.first;
			if(battleStatus[playerID] == BATTLESTATUS_ENDLOST || battleStatus[playerID] == BATTLESTATUS_FORFEIT) // Oh dear,, I lost
			{
				// Send message to other players this player is out (only if it's a team battle)
				if(battleType != ONE_VS_ONE) 
				{
					BOOST_FOREACH(BattleList::value_type tmp, battlePlayerList)
					{						
						BattlePlayer *tmpIt;
						if((tmpIt = tmp.second) && !tmpIt->isRemoved && tmpIt->getCreatureType() == PLAYER) {
							BattleEvents::SendRemovePlayer(tmpIt, playerIt);
						}
					}
				}

				// Move player to latest PokeCenter
				/*
				if(!playerIt->isRemoved && playerIt->getCreatureType() == PLAYER) 
				{
					if(playerIt->getCreature()->getPlayer()->moveToPokeCenter()) {
						PokeCenter::healActivePokemon(playerIt->getCreature()->getPlayer());
					}
				}*/
			}
			else if(isEndOfBattle) {
				continue;
			}
			// If A pokemon fainted, players can change their pokemon
			else if(freePokemonChange)
			{
				if(playerIt->getCreatureType() != PLAYER) {
					playerIt->setActivePokemon();
				}
				else 
				{
					if(battleStatus[playerID] == BATTLESTATUS_CHANGEPOKEMON) // My pokemon fainted
					{
						BattleEvents::_ControlPacket(playerIt, BATTLECONTROL_CHOOSEPOKEMON);
						BattleEvents::_TextPacket(playerIt, GT_BATTLEPOKEMONFAINTED_OWN);
					}
					else // My pokemon didn't faint to I get to choose
					{
						if(playerIt->getPokemonTotalActive() > 1) // Check if player has more that 1 pokemon who can fight
						{
							BattleEvents::_TextPacket(playerIt, GT_BATTLECHANGEPOKEMON_QUESTION);

							std::map<int, string> answerList;
							answerList.insert(std::make_pair(1, "Yes"));
							answerList.insert(std::make_pair(2, "No"));

							BattleEvents::_DialoguePacket(playerIt, DIALOGUE_OPTIONS, "", answerList);
						}
						else
						{
							playerIt->setMoveOption(MOVE_NONE);
							playerIt->setMoveParameter(0);
							playerIt->setMoveValidated(1);
							playerIt->setPlayerStatus(PS_READY);

							BattleEvents::_TextPacket(playerIt, GT_BATTLEWAITING);

							continue; // otherwise playerIt status will be reset
						}
					}
				}
			}
			else if(battleStatus[playerID] == BATTLESTATUS_CONTINUE && countPlayersContinue >= minPlayers) // Nothing so see here people, move along...
			{
				if(playerIt->getCreatureType() == PLAYER) 
				{
					BattleEvents::_ControlPacket(playerIt, BATTLECONTROL_CHOOSEMOVE);
					std::string params[] = { playerIt->getPokemonActive()->getName() };
					BattleEvents::_TextPacket(playerIt, GT_POKEMONCHOOSEMOVE, params);
				}
			}

			playerIt->resetBattleInfo();
		}
	}

	// Clean up all players who are removed from the battle
	BattleList list = battlePlayerList;
	BOOST_FOREACH(BattleList::value_type it, list)
	{
		if((playerIt = it.second) && playerIt->isRemoved)
		{
			battlePlayerList.erase(it.first);
			deleteBattlePlayer(playerIt);
		}
	}

	// Check if there are enough players to continue the battle
	// Return false if battle has ended
	return (isEndOfBattle) ? false : checkBattleContinue();
}

/**
  * Checks if player has any pokemon who can learn a new move
  * If pokemon found, screen info is send
  */
void Battle::checkPokemonAttackLearn(BattlePlayer *player)
{
	// Loop pokemon
	BOOST_FOREACH(PokemonList::value_type it, player->getPokemonList())
	{
		Pokemon *pokemon;
		int i = it.first;
		if((pokemon = it.second) && pokemon->attackLearning)
		{
			player->setMoveValidated(0);
			player->setPlayerStatus(PS_ATTACKLEARN);

			// Learn the new move, check if there are any slots available
			if(pokemon->getAttackCount() != 4)
			{
				Attack *attack = pokemon->updateAttack(pokemon->getAttackCount(), pokemon->attackLearningID);
				BattleEvents::SendUpdateAttack(player, i, pokemon->getAttackCount(), attack);

				// Send message to player
				std::string params[] = { pokemon->getName(), attack->getAttackName() };
				BattleEvents::_TextPacket(player, GT_POKEMONLEARNATTACK, params);
				BattleEvents::_SleepPacket(player, 2000);

				pokemon->attackLearning = false;
				pokemon->attackLearningID = 0;
				pokemon->attackLearningName = "";

				checkPokemonAttackLearn(player);
			}
			else // All lots are full
			{
				std::string params[] = { pokemon->getName(), pokemon->attackLearningName };
				BattleEvents::_TextPacket(player, GT_POKEMONLEARNATTACK_SLOT, params);	

				std::map<int, string> attackList = pokemon->getAttackList();
				attackList.insert(std::make_pair(5, "Don't learn the attack"));
				BattleEvents::_DialoguePacket(player, DIALOGUE_OPTIONS, "", attackList);
			}	

			return;
		}
	}

	player->resetBattleInfo();
}

/**
  * Player has chosen a slot for the new move
  */
void Battle::assignPokemonAttack(BattlePlayer *player)
{
	BOOST_FOREACH(PokemonList::value_type it, player->getPokemonList())
	{
		Pokemon *pokemon;
		int i = it.first;
		if((pokemon = it.second) && pokemon->attackLearning)
		{
			if(player->getMoveParameter() < 5) 
			{
				Attack *attack = pokemon->updateAttack(player->getMoveParameter(), pokemon->attackLearningID);
				BattleEvents::SendUpdateAttack(player, i, player->getMoveParameter(), attack);

				std::string params[] = { pokemon->getName(), attack->getAttackName() };
				BattleEvents::_TextPacket(player, GT_POKEMONLEARNATTACK, params);
				BattleEvents::_SleepPacket(player, 2000);
			}

			pokemon->attackLearning = false;
			pokemon->attackLearningID = 0;
			pokemon->attackLearningName = "";

			// check if there are more pokemon who need to learn a move
			checkPokemonAttackLearn(player);

			break;
		}
	}
}

std::vector<Creature*> Battle::getPlayerList()
{
	std::vector<Creature*> list;
	for (BattleList::iterator it = battlePlayerList.begin(); it != battlePlayerList.end(); ++it) 
	{
		BattlePlayer *tmp;
		if((tmp = (*it).second) && tmp->getCreatureType() == PLAYER)
		{
			list.push_back(tmp->getCreature());
		}
	}

	return list;
}
