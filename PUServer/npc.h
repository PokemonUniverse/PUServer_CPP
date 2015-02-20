#ifndef __npc_h_
#define __npc_h_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#include "global.h"
#include "game.h"
#include "luascript.h"
#include "tools.h"
#include "filesystem.h"
#include "configmanager.h"
#include "database.h"
#include "tile.h"

using namespace std;

class Creature;
class Npc;
class Tile;
class NpcResponse;
struct NpcState;
class Player;

// (External) Classes
extern ConfigManager g_config;
extern Database g_db;

// Definitions
typedef std::vector<Npc*> NpcList;
typedef std::vector<Player*>PlayerList;

enum ShopEvent_t{
	SHOPEVENT_SELL,
	SHOPEVENT_BUY,
	SHOPEVENT_CLOSE
};

// Real Classes
class NpcManager
{
public:
	NpcManager();
	~NpcManager();
	bool reload();

	void killThreads();

protected:
	NpcList listNpc;
	NpcList loadedNpc;

	bool firstLoad;
};


class NpcScriptInterface : public LuaScriptInterface
{
public:
	NpcScriptInterface();
	virtual ~NpcScriptInterface();

	bool loadNpcLib(std::string file);

	static void pushState(lua_State *L, NpcState* state);
	static void popState(lua_State *L, NpcState* &state);

protected:

	virtual void registerFunctions();

	static int luaActionSay(lua_State *L);
	static int luaActionDialogue(lua_State *L);
	static int luaActionEndDialogue(lua_State* L);
	static int luaActionHideDialogue(lua_State* L);
	static int luaActionHealParty(lua_State* L);
	static int luaActionGetQuestProgress(lua_State* L);
	static int luaActionSetQuestProgress(lua_State* L);
	static int luaActionCheckPokemon(lua_State* L);
	static int luaActionGetMoney(lua_State* L);
	static int luaActionAddMoney(lua_State* L);
	static int luaActionRemoveMoney(lua_State* L);
	
	static int luaActionAddItem(lua_State* L);
	static int luaActionCheckItem(lua_State* L);
	static int luaActionRemoveItem(lua_State* L);
	static int luaOpenShopWindow(lua_State* L);
	static int luaCloseShopWindow(lua_State* L);

	static int luaActionStartBattle(lua_State *L);

private:
	virtual bool initState();
	virtual bool closeState();

	bool m_libLoaded;
};

class NpcEventsHandler
{
public:
	NpcEventsHandler(Npc* npc);
	virtual ~NpcEventsHandler();

	virtual void onAnswer(const Creature* creature, int answer){};
	virtual void onBuy(const Creature* creature, int callback){};
	virtual void onShopWindowClose(const Creature* creature){};

	bool isLoaded();

protected:
	Npc* m_npc;
	bool m_loaded;
};

class NpcScript : public NpcEventsHandler
{
public:
	NpcScript(std::string file, Npc* npc);
	virtual ~NpcScript();

	void onAnswer(const Creature* creature, int answer);
	void onBuy(const Creature* creature, int callback);
	void onShopWindowClose(const Creature* creature);

private:
	NpcScriptInterface* m_scriptInterface;

	int32_t m_onAnswer;
	int32_t m_onBuy;
	int32_t m_onShopWindowClose;
};

struct ScriptVars{
	ScriptVars()
	{
		n1 = -1;
		n2 = -1;
		n3 = -1;
		b1 = false;
		b2 = false;
		b3 = false;
		s1 = "";
		s2 = "";
		s3 = "";
	}

	int32_t n1;
	int32_t n2;
	int32_t n3;
	bool b1;
	bool b2;
	bool b3;
	std::string s1;
	std::string s2;
	std::string s3;
};

class Npc : public Creature
{
public:	
	~Npc();

	virtual Npc* getNpc() {return this;};
	virtual const Npc* getNpc() const {return this;};

	bool load();
	bool loadPokemon(Database *_db);
	bool loadItems(Database *_db);
	bool addItem( int _itemId);
	bool reload();

	bool isLoaded() { return loaded; }
	bool isRunning() { return threadRunning; }

	void setTalkRadius(int _talkRadius) { talkRadius = _talkRadius; }
	void setWalkInterval(int _walk) { walkTicks = _walk; }

	PlayerTypes getCreatureType() const { return NPC; };

	void runThread();
	void stopThread();

	void doSay(std::string msg, Player* focus = NULL, bool publicize = false) {}
	void doTurn(Direction dir) {}
	void doMove(Direction dir) {}
	void doMoveTo(Position pos) {}

	void setBasePosition(Position basePos) { basePosition = basePos; }

	void onCreatureMove(const Creature *creature, Cylinder *fromCylinder, Cylinder *toCylinder);
	void onPlayerTrade(Player* player, ShopEvent_t type, int callback, int itemId, int count, bool ignoreCapacity);
	void onPlayerCloseShopWindow(Player *player);

	void startDialogue(Player *player);
	void setAnswer(Player *player, int answer);

	void addVisibleCreature(const Creature *creature);	
	void removeInteractingPlayer(Player *player);
	void setScript(std::string script) { m_script = script; }

	NpcScriptInterface* getScriptInterface();

private:
	bool canWalk(Direction dir);

protected:
	Npc(int _id, std::string _name);	
	void reset();

	void onPlayerEnter(Player* player);
	void onPlayerLeave(Player* player);

	boost::thread npcThread;
	void MainThread();

	std::string m_datadir;
	std::string m_script;

	PlayerList interactingPlayers;

	int walkTicks;
	int lastWalk;
	int talkRadius;
	int walkRadius;
	int idleTime;
	int idleInterval;
	int lastIdleInterval;
	int baseSpeed;
	int focusCreature;

	Position basePosition;

	bool isInside;
	bool loaded;
	bool threadRunning;
	bool isMarket;

	NpcEventsHandler* m_npcEventHandler;

	typedef std::list<uint32_t> QueueList;
	QueueList queueList;

	static NpcScriptInterface* m_scriptInterface;
	friend class NpcManager;
	friend class NpcScriptInterface;
};

#endif