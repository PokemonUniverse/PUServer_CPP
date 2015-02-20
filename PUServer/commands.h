#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <string>
#include <map>
#include "creature.h"

struct Command;
struct s_defcommands;

class Commands{
public:
	//Commands():game(NULL),loaded(false){};
	Commands();

	bool loadXml(const std::string& _datadir);	
	bool isLoaded(){return loaded;}
	bool reload();

	bool exeCommand(Creature* creature, const std::string& cmd);


protected:
	bool loaded;
	std::string datadir;

	//commands
	bool closeServer(Creature* creature, const std::string& cmd, const std::string& param);
	bool saveServer(Creature* creature, const std::string& cmd, const std::string& param);
	bool openServer(Creature* creature, const std::string& cmd, const std::string& param);
	bool reloadMap(Creature* creature, const std::string& cmd, const std::string& param);
	bool warpGoTo(Creature* creature, const std::string& cmd, const std::string& param);
	bool warpGET(Creature* creature, const std::string& cmd, const std::string& param);
	bool summonPokemonBattle(Creature* creature, const std::string& cmd, const std::string& param);
	bool kickPlayer(Creature* creature, const std::string& cmd, const std::string& param);
	bool broadCastMessage(Creature* creature, const std::string& cmd, const std::string& param);
	bool getTileInfo(Creature* creature, const std::string& cmd, const std::string& param);
	bool unlimitedRepel( Creature* creature, const std::string& cmd, const std::string& param );

	bool listPlayerParty( Creature* creature, const std::string& cmd, const std::string& param );

	//table of commands
	static s_defcommands defined_commands[];

	typedef std::map<std::string,Command*> CommandMap;
	CommandMap commandMap;
};

typedef  bool (Commands::*CommandFunc)(Creature*,const std::string&,const std::string&);

struct Command{
	CommandFunc f;
	int32_t accesslevel;
	bool loaded;
};

struct s_defcommands{
	const char *name;
	CommandFunc f;
};

#endif