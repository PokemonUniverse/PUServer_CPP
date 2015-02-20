#ifndef __NPC_H_
#define __NPC_H_

#include "definitions.h"
#include "creature.h"

#include "position.h"
#include "game.h"

class Npc : public Creature
{
public:
	Npc(uint32_t _id, const std::string& _name);
	~Npc() {};

	virtual Npc* getNpc() { return this; };
	virtual const Npc* getNpc() const { return this; };

	virtual const std::string& getNameDescription() const { return m_name; };
	virtual uint32_t idRange() { return 0x80000000; };

private:
	std::string m_datadir;
	std::string m_script;

	playerList_t m_interactingPlayers;

	int m_walkTicks;
	int m_lastWalk;
	int m_talkRadius;
	int m_idleTime;
	int m_idleInterval;
	int m_lastIdleInterval;
	int m_baseSpeed;
	int m_focusCreature;

	Position m_basePosition;
	uint32_t m_walkRadius;

	bool m_isInside;
	bool m_loaded;
	bool m_threadRunning;

	//static NpcScriptInterface* m_scriptInterface;
	//NpcEventsHandler* m_npcEventHandler;

	//typedef std::list<uint32_t> QueueList;
	//QueueList m_queueList;
	
	//friend class NpcManager;
	//friend class NpcScriptInterface;
};

#endif