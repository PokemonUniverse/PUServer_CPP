#ifndef __CHAT_H_
#define __CHAT_H_

#include <map>
#include <list>

#include "enums.h"
#include "definitions.h"


class Player;
class Party;

typedef std::map<uint32_t, Player*> UsersMap;

enum ChannelID {
	CHANNEL_LOCAL		= 0x00,
	CHANNEL_WORLD		= 0x01,
	CHANNEL_TRADE		= 0x02,
	CHANNEL_BATTLE		= 0x03,
	CHANNEL_GUILD		= 0x04,
	CHANNEL_PARTY		= 0x05,
	CHANNEL_PRIVATE		= 0xFFFF,
};

class ChatChannel
{
public:
	ChatChannel(uint16_t channelId, std::string channelName);
	virtual ~ChatChannel(){}

	bool addUser(Player* player);
	bool removeUser(Player* player, bool sendCloseChannel = false);

	bool talk(Player* fromPlayer, SpeakClasses type, const std::string& text, uint32_t time = 0);
	bool sendInfo(SpeakClasses type, const std::string& text, uint32_t time = 0);

	const std::string& getName(){ return m_name; }
	const uint16_t getId(){ return m_id; }
	const UsersMap& getUsers(){ return m_users; }

	virtual const uint32_t getOwner(){ return 0; }

protected:
	UsersMap m_users;
	std::string m_name;
	uint16_t m_id;
};

class PrivateChatChannel : public ChatChannel
{
public:
	PrivateChatChannel(uint16_t channelId, std::string channelName);
	virtual ~PrivateChatChannel(){};

	virtual const uint32_t getOwner(){return m_owner;}
	void setOwner(uint32_t id){m_owner = id;}

	bool isInvited(const Player* player);

	void invitePlayer(Player* player, Player* invitePlayer);
	void excludePlayer(Player* player, Player* excludePlayer);

	bool addInvited(Player* player);
	bool removeInvited(Player* player);

	void closeChannel();

protected:
	typedef std::map<uint32_t, Player*> InvitedMap;

	InvitedMap m_invites;
	uint32_t m_owner;
};

typedef std::list<ChatChannel*> ChannelList;

class Chat
{
public:
	Chat();
	~Chat();
	ChatChannel* createChannel(Player* player, uint16_t channelId);
	bool deleteChannel(Player* player, uint16_t channelId);
	bool deleteChannel(Party* party);

	bool addUserToChannel(Player* player, uint16_t channelId);
	bool removeUserFromChannel(Player* player, uint16_t channelId);
	void removeUserFromAllChannels(Player* player);

	uint16_t getFreePrivateChannelId();

	bool talkToChannel(Player* player, SpeakClasses type, const std::string& text, unsigned short channelId);

	std::string getChannelName(Player* player, uint16_t channelId);
	ChannelList getChannelList(Player* player);

	ChatChannel* getChannel(Party* party);
	ChatChannel* getChannel(Player* player, uint16_t channelId);
	ChatChannel* getChannelById(uint16_t channelId);
	PrivateChatChannel* getPrivateChannel(Player* player);

private:

	typedef std::map<uint16_t, ChatChannel*> NormalChannelMap;
	typedef std::map<uint32_t, ChatChannel*> GuildChannelMap;
	//typedef std::map<Party*, PrivateChatChannel*> PartyChannelMap;
	NormalChannelMap m_normalChannels;
	GuildChannelMap m_guildChannels;
	//PartyChannelMap m_partyChannels;

	typedef std::map<uint16_t, PrivateChatChannel*> PrivateChannelMap;
	PrivateChannelMap m_privateChannels;

	ChatChannel* dummyPrivate;
};

#endif