#include <string>
#include <sstream>
#include <utility>

#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

#include "commands.h"
#include "player.h"
#include "npc.h"
#include "game.h"
#include "tools.h"
#include "configmanager.h"
#include "battle.h"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

extern ConfigManager g_config;
extern Game g_game;

extern bool readXMLInteger(xmlNodePtr p, const char *tag, int &value);

#define ipText(a) (unsigned int)a[0] << "." << (unsigned int)a[1] << "." << (unsigned int)a[2] << "." << (unsigned int)a[3]

//table of commands
s_defcommands Commands::defined_commands[] = {
	{"/save",&Commands::saveServer},
	{"/closeserver",&Commands::closeServer},
	{"/openserver",&Commands::openServer},
	{"/refreshmap",&Commands::reloadMap},
	{"/summon",&Commands::summonPokemonBattle},
	{"/goto",&Commands::warpGoTo},
	{"/get",&Commands::warpGET},
	{"/kick",&Commands::kickPlayer},
	{"/repel",&Commands::unlimitedRepel},
	{"/B",&Commands::broadCastMessage},
	{"/tileinfo",&Commands::getTileInfo},
	{"/listpokemon",&Commands::listPlayerParty},
};

Commands::Commands()
{
	loaded = false;

	//setup command map
	for(uint32_t i = 0; i < sizeof(defined_commands) / sizeof(defined_commands[0]); i++)
	{
		Command* cmd = new Command;
		cmd->loaded = false;
		cmd->accesslevel = 1;
		cmd->f = defined_commands[i].f;
		std::string key = defined_commands[i].name;
		commandMap[key] = cmd;
	}
}

std::string parseParams(tokenizer::iterator &it, tokenizer::iterator end)
{
	std::string tmp;
	if(it == end){
		return "";
	}
	else{
		tmp = *it;
		++it;
		if(tmp[0] == '"'){
			tmp.erase(0,1);
			while(it != end && tmp[tmp.length() - 1] != '"'){
				tmp += " " + *it;
				++it;
			}

			if(tmp.length() > 0 && tmp[tmp.length() - 1] == '"'){
				tmp.erase(tmp.length() - 1);
			}
		}
		return tmp;
	}
}

bool Commands::loadXml(const std::string& _datadir)
{
	datadir = _datadir;
	std::string filename = datadir + "/commands.xml";

	xmlDocPtr doc = xmlParseFile(filename.c_str());
	if(doc){
		loaded = true;
		xmlNodePtr root, p;
		root = xmlDocGetRootElement(doc);

		if(xmlStrcmp(root->name,(const xmlChar*)"commands") != 0){
			xmlFreeDoc(doc);
			return false;
		}

		std::string strCmd;

		p = root->children;

		while (p){
			if(xmlStrcmp(p->name, (const xmlChar*)"command") == 0){
				if(readXMLString(p, "cmd", strCmd)){
					CommandMap::iterator it = commandMap.find(strCmd);
					int alevel;
					if(it != commandMap.end()){
						if(readXMLInteger(p,"access",alevel))
						{
							if(!it->second->loaded) {
								it->second->accesslevel = alevel;
								it->second->loaded = true;
							}
							else {
								std::cout << "Duplicated command " << strCmd << std::endl;
							}
						}
						else{
							std::cout << "missing access tag for " << strCmd << std::endl;
						}
					}
					else{
						//error
						std::cout << "Unknown command " << strCmd << std::endl;
					}
				}
				else{
					std::cout << "missing cmd." << std::endl;
				}
			}
			p = p->next;
		}
		xmlFreeDoc(doc);
	}

	//
	for(CommandMap::iterator it = commandMap.begin(); it != commandMap.end(); ++it){
		if(it->second->loaded == false){
			std::cout << "Warning: Missing access level for command " << it->first << std::endl;
		}
		//register command tag in game
		g_game.addCommandTag(it->first.substr(0,1));
	}


	return this->loaded;
}

bool Commands::reload()
{
	loaded = false;
	for(CommandMap::iterator it = commandMap.begin(); it != commandMap.end(); ++it){
		it->second->accesslevel = 1;
		it->second->loaded = false;
	}
	g_game.resetCommandTag();

	loadXml(datadir);
	return true;
}

bool Commands::exeCommand(Creature* creature, const std::string& cmd)
{
	std::string str_command;
	std::string str_param;

	std::string::size_type loc = cmd.find( ' ', 0 );
	if(loc != std::string::npos && loc >= 0){
		str_command = std::string(cmd, 0, loc);
		str_param = std::string(cmd, (loc+1), cmd.size()-loc-1);
	}
	else {
		str_command = cmd;
		str_param = std::string("");
	}

	//find command
	CommandMap::iterator it = commandMap.find(str_command);
	if(it == commandMap.end()){
		return false;
	}

	Player* player = creature->getPlayer();
	//check access for this command
	if(player && player->getAccessLevel() < it->second->accesslevel){
		if(player->getAccessLevel() > 0){
			player->sendCreatureSay(NULL, SPEAK_SAY, "You can not execute this command.");
		}

		return false;
	}

	//execute command
	if(player) {
		player->sendCreatureSay(NULL, SPEAK_SAY, cmd.c_str());
	}
	CommandFunc cfunc = it->second->f;
	(this->*cfunc)(creature, str_command, str_param);

	return true;
}

bool Commands::closeServer(Creature* creature, const std::string& cmd, const std::string& param)
{
	// Save the rest of everything (including players that stayed online)
	g_game.saveServer(true, false);

	Player* player = creature->getPlayer();
	// Notify player (If he's still online)
	if(player){
		player->sendCreatureSay(NULL, SPEAK_SAY, "Server is now closed.");
	}

	return true;
}

bool Commands::saveServer(Creature* creature, const std::string& cmd, const std::string& param)
{
	// Save most everything
	g_game.saveServer(false, false);

	// Notify player
	if(Player* player = creature->getPlayer()){
		player->sendCreatureSay(NULL, SPEAK_SAY, "Server has been saved.");
	}

	return true;
}

bool Commands::openServer(Creature* creature, const std::string& cmd, const std::string& param)
{
	g_game.setGameState(GAME_STATE_NORMAL);

	Player* player = creature->getPlayer();
	if(player){
		player->sendCreatureSay(NULL, SPEAK_SAY, "Server is now open.");
	}
	return true;
}

bool Commands::reloadMap(Creature* creature, const std::string& cmd, const std::string& param)
{
	Player* player = creature->getPlayer();
	if(!player){
		return false;
	}

	player->sendCreatureSay(NULL, SPEAK_SAY, "Reloading map, might take some time...");
	
	g_game.reloadMap();

	player->sendCreatureSay(NULL, SPEAK_SAY, "Relaoding map complete.");

	
	return true;
}

bool Commands::warpGoTo(Creature* creature, const std::string& cmd, const std::string& param)
{
	Player *player = creature->getPlayer();
	
	std::string tmp = param;
	std::string::size_type pos = tmp.find_first_of(' ', 0);	
	Position warpPosition;
	if(pos == std::string::npos)
	{
		Player *warpPlayer = g_game.getPlayerByName(param);
		if(!warpPlayer)
		{
			player->sendCreatureSay(NULL, SPEAK_SAY, "Can not find this player to warp to.");
			return false;
		}

		warpPosition = warpPlayer->getPosition();
	}
	else
	{
		int coordX = atoi(tmp.substr(0, pos).c_str());
		int coordY = atoi(tmp.substr(pos + 1, tmp.size()).c_str());
		if(coordX == 0 || coordY == 0) 
		{
			player->sendCreatureSay(NULL, SPEAK_SAY, "Faulty warp command");
			return false;
		}

		warpPosition.x = coordX;
		warpPosition.y = coordY;
		warpPosition.z = creature->getPosition().z;
	}
	
	if(g_game.internalTeleport(creature, warpPosition) == RET_PLAYERISTELEPORTED)
	{
		Tile *toCylinder = g_game.getTile(warpPosition);
		creature->getPlayer()->sendCancelWalk(toCylinder);	

		creature->getPlayer()->sendMap(DIR_NULL);
		return true;
	}

	player->sendCreatureSay(NULL, SPEAK_SAY, "Could not warp...");
	return false;
}

bool Commands::warpGET(Creature* creature, const std::string& cmd, const std::string& param)
{
	Creature* paramCreature = g_game.getCreatureByName(param);
	if(paramCreature && paramCreature->getPlayer())
	{
		Position destPos = paramCreature->getPosition();
		if(g_game.internalTeleport(paramCreature, creature->getPosition()) == RET_PLAYERISTELEPORTED)
		{
			Tile *toCylinder = g_game.getTile(destPos);
			paramCreature->getPlayer()->sendCancelWalk(toCylinder);

			paramCreature->getPlayer()->sendMap(DIR_NULL);
			return true;
		}
	}	

	creature->getPlayer()->sendCreatureSay(NULL, SPEAK_SAY, "Could not find this player.");
	return false;
}

bool Commands::summonPokemonBattle( Creature* creature, const std::string& cmd, const std::string& param )
{
	std::string tmp = param;
	std::string::size_type pos = tmp.find_first_of(' ', 0);	
	int pokemonID = atoi(tmp.substr(0, pos).c_str());
	int pokemonLvl = atoi(tmp.substr(pos + 1, tmp.size()).c_str());
	if(pokemonID == 0 || pokemonLvl == 0) {
		return false;
	}

	Battle *newBattle = new Battle(ONE_VS_ONE);
	bool ret1 = newBattle->addPlayer(creature, 0);
	bool ret2 = newBattle->addPlayer(pokemonID, pokemonLvl, 1);
	
	if(ret1 && ret2) {
		newBattle->startBattle();
	}
	else {
		creature->getPlayer()->sendCreatureSay(NULL, SPEAK_SAY, "Error while creating battle players.");
	}

	return true;
}

bool Commands::kickPlayer( Creature* creature, const std::string& cmd, const std::string& param )
{
	Player* playerKick = g_game.getPlayerByName(param);
	if(playerKick)
	{
		Player* player = creature->getPlayer();
		if(player && player->getAccessLevel() < playerKick->getAccessLevel()){
			player->sendCreatureSay(NULL, SPEAK_SAY, "You cannot kick this player.");
			return true;
		}

		playerKick->kickPlayer();
		return true;
	}
	return false;
}

bool Commands::broadCastMessage( Creature* creature, const std::string& cmd, const std::string& param )
{
	Player* player = creature->getPlayer();
	if(!player)
		return false;

	g_game.internalBroadcastMessage(player, param);
	return true;
}

bool Commands::getTileInfo( Creature* creature, const std::string& cmd, const std::string& param )
{
	Tile *tile = g_game.getTile(creature->getPosition());

	if(tile)
	{
		int l1 = (tile->getObject(0) != NULL) ? tile->getObject(0)->getTileID() : 0;
		int l2 = (tile->getObject(1) != NULL) ? tile->getObject(1)->getTileID() : 0;
		int l3 = (tile->getObject(2) != NULL) ? tile->getObject(2)->getTileID() : 0;

		std::string tmp = boost::str(boost::format("X: %d | Y: %d | Z: %d | L1: %d | L2: %d | L3: %d") % tile->getX() % tile->getY() % tile->getZ() % l1 % l2 % l3);

		creature->getPlayer()->sendCreatureSay(NULL, SPEAK_SAY, tmp);
	}

	return true;
}

bool Commands::unlimitedRepel( Creature* creature, const std::string& cmd, const std::string& param )
{
	if(param == "on")
	{
		if(!creature->getPlayer()->hasFlag(PlayerFlag_AdminRepel)) {
			creature->getPlayer()->addFlag(PlayerFlag_AdminRepel);

			creature->getPlayer()->sendCreatureSay(NULL, SPEAK_SAY, "Unlimited Repel: ON");
		}
	}
	else if(param == "off")
	{
		if(creature->getPlayer()->hasFlag(PlayerFlag_AdminRepel)) {
			creature->getPlayer()->removeFlag(PlayerFlag_AdminRepel);

			creature->getPlayer()->sendCreatureSay(NULL, SPEAK_SAY, "Unlimited Repel: OFF");
		}
	}

	return false;
}

bool Commands::listPlayerParty( Creature* creature, const std::string& cmd, const std::string& param )
{
	PokemonList list = creature->getPokemonParty();
	//list.rehash();
	BOOST_FOREACH(PokemonList::value_type it, list)
	{
		std::string pkmnStr;
		if(it.second) {
			pkmnStr = boost::str(boost::format("%d - %s") % it.first % it.second->getName());
		}
		else {
			pkmnStr = boost::str(boost::format("%d - EMPTY") % it.first);
		}

		creature->getPlayer()->sendCreatureSay(NULL, SPEAK_SAY, pkmnStr);
	}

	return true;
}