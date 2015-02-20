#ifndef __luascript_h_
#define __luascript_h_

#include <deque>
#include <string>
#include <map>
#include <list>
#include <vector>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "creature.h"
#include "position.h"
#include "definitions.h"

class Creature;
class Player;
class Npc;
class LuaScriptInterface;

enum{
	EVENT_ID_LOADING = 1,
	EVENT_ID_USER = 1000,
};

enum LUA_RET_CODE{
	LUA_NO_ERROR = 0,
	LUA_ERROR = -1,
	LUA_TRUE = 1,
	LUA_FALSE = 0,
	LUA_NULL = 0,
};

enum LuaVariantType_t{
	VARIANT_NONE = 0,
	VARIANT_NUMBER,
	VARIANT_POSITION,
	VARIANT_TARGETPOSITION,
	VARIANT_STRING,
};

enum ErrorCode_t{
	LUA_ERROR_PLAYER_NOT_FOUND,
	LUA_ERROR_CREATURE_NOT_FOUND,
	LUA_ERROR_ITEM_NOT_FOUND,
	LUA_ERROR_THING_NOT_FOUND,
	LUA_ERROR_TILE_NOT_FOUND,
	LUA_ERROR_HOUSE_NOT_FOUND,
	LUA_ERROR_COMBAT_NOT_FOUND,
	LUA_ERROR_CONDITION_NOT_FOUND,
	LUA_ERROR_AREA_NOT_FOUND,
	LUA_ERROR_CONTAINER_NOT_FOUND,
	LUA_ERROR_VARIANT_NOT_FOUND,
	LUA_ERROR_VARIANT_UNKNOWN,
	LUA_ERROR_SPELL_NOT_FOUND
};

struct LuaVariant{
	LuaVariant()
	{
		type = VARIANT_NONE;
		text = "";
		pos.x = 0;
		pos.y = 0;
		pos.z = 0;
		number = 0;
	}

	LuaVariantType_t type;
	std::string text;
	Position pos;
	uint32_t number;
};

class ScriptEnviroment
{
public:
	ScriptEnviroment();
	~ScriptEnviroment();

	void resetEnv();
	void resetCallback() {m_callbackId = 0;}

	void setScriptId(int32_t scriptId, LuaScriptInterface* scriptInterface)
	{
		m_scriptId = scriptId; 
		m_interface = scriptInterface;
	}

	bool setCallbackId(int32_t callbackId, LuaScriptInterface* scriptInterface);
	void setEventDesc(const std::string& desc) { m_eventdesc = desc; }

	std::string getEventDesc() {return m_eventdesc;}
	int32_t getScriptId() {return m_scriptId;}
	int32_t getCallbackId() {return m_callbackId;}
	LuaScriptInterface* getScriptInterface() {return m_interface;}

	void setTimerEvent() {m_timerEvent = true;}
	void resetTimerEvent() {m_timerEvent = false;}

	void getEventInfo(int32_t& scriptId, std::string& desc, LuaScriptInterface*& scriptInterface, int32_t& callbackId, bool& timerEvent);

	void addGlobalStorageValue(const uint32_t key, const int32_t value);
	bool getGlobalStorageValue(const uint32_t key, int32_t& value) const;

	void setRealPos(const Position& realPos) { m_realPos = realPos; }
	Position getRealPos() { return m_realPos; }

	void setNpc(Npc* npc) {m_curNpc = npc;}
	Npc* getNpc() const {return m_curNpc;}

	Player* getPlayerByUID(uint32_t uid);
	Creature* getCreatureByUID(uint32_t uid);

	static bool loadGameState();
	static bool saveGameState();

	uint32_t addThing(Creature* thing);

private:
	typedef std::map<uint32_t, Creature*> ThingMap;
	typedef std::vector<const LuaVariant*> VariantVector;
	typedef std::map<uint32_t, int32_t> StorageMap;

	//script file id
	int32_t m_scriptId;
	int32_t m_callbackId;
	bool m_timerEvent;
	LuaScriptInterface* m_interface;
	//script event desc
	std::string m_eventdesc;

	static StorageMap m_globalStorageMap;

	Position m_realPos;

	//item/creature map
	int32_t m_lastUID;
	ThingMap m_localMap;


	//for npc scripts
	Npc* m_curNpc;
};

#define reportErrorFunc(a)  reportError(__FUNCTION__, a)

class LuaScriptInterface
{
public:
	LuaScriptInterface(std::string interfaceName);
	virtual ~LuaScriptInterface();

	int32_t loadFile(const std::string& file, Npc* npc = NULL);
	int32_t loadBuffer(const std::string& text, Npc* npc /* = NULL*/);
	const std::string& getFileById(int32_t scriptId);

	int32_t getEvent(const std::string& eventName);

	virtual bool initState();
	bool reInitState();

	static ScriptEnviroment* getScriptEnv(){
		assert(m_scriptEnvIndex >= 0 && m_scriptEnvIndex < 16);
		return &m_scriptEnv[m_scriptEnvIndex];
	}

	static bool reserveScriptEnv(){
		++m_scriptEnvIndex;
		if(m_scriptEnvIndex < 15){
			return true;
		}
		else{
			--m_scriptEnvIndex;
			return false;
		}
	}

	static void releaseScriptEnv(){
		if(m_scriptEnvIndex >= 0){
			m_scriptEnv[m_scriptEnvIndex].resetEnv();
			--m_scriptEnvIndex;
		}
	}

	static void reportError(const char* function, const std::string& error_desc);

	std::string getInterfaceName() { return m_interfaceName;}
	const std::string& getLastLuaError() const { return m_lastLuaError; }
	void dumpLuaStack();

	lua_State* getLuaState() { return m_luaState; }

	bool pushFunction(int32_t functionId);

	static int luaErrorHandler(lua_State* L);
	int32_t callFunction(uint32_t nParams);

	//push/pop common structures
	static void pushVariant(lua_State *L, const LuaVariant& var);
	static void pushPosition(lua_State *L, const Position& pos);
	static void pushCallback(lua_State *L, int32_t callback);

	static LuaVariant popVariant(lua_State *L);
	static void popPosition(lua_State *L, Position& pos);
	static uint32_t popNumber(lua_State *L);
	static double popFloatNumber(lua_State *L);
	static const char* popString(lua_State *L);
	static int32_t popCallback(lua_State *L);

	static int32_t getField(lua_State *L, const char *key);
	static uint32_t getFieldU32(lua_State *L, const char *key);
	static void setField(lua_State *L, const char* index, int32_t val);
	static void setField(lua_State *L, const char* index, const std::string& val);
	static std::string getFieldString(lua_State *L, const char *key);
	static void setFieldBool(lua_State *L, const char* index, bool val);
	static bool getFieldBool(lua_State *L, const char *key);

protected:
	virtual bool closeState();

	virtual void registerFunctions();

	static std::string getErrorDesc(ErrorCode_t code);

	// Lua Function go here
	// DO
	static int luaDoCreatureSay(lua_State *L);

	// GET
	static int luaGetCreatureName(lua_State *L);
	static int luaGetCreaturePosition(lua_State *L);

	lua_State* m_luaState;
	std::string m_lastLuaError;

private:
	static ScriptEnviroment m_scriptEnv[16];
	static int32_t m_scriptEnvIndex;

	int32_t m_runningEventId;
	std::string m_loadingFile;

	//script file cache
	typedef std::map<int32_t , std::string> ScriptsCache;
	ScriptsCache m_cacheFiles;

	//events information
	struct LuaTimerEventDesc{
		int32_t scriptId;
		int function;
		std::list<int> parameters;
	};
	uint32_t m_lastEventTimerId;

	typedef std::map<uint32_t, LuaTimerEventDesc> LuaTimerEvents;
	LuaTimerEvents m_timerEvents;

	void executeTimerEvent(uint32_t eventIndex);

	std::string m_interfaceName;
};

#endif
