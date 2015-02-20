#ifndef __quest_h_
#define __quest_h_

#include <iostream>
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include "database.h"

extern Database g_db;

class Quest
{
public:
	Quest(int _id, std::string _name, std::string _description)
		: id(_id),
		  name(_name),
		  description(_description) {}

	~Quest() {}

	int getID() { return id; }
	std::string getName() { return name; }
	std::string getDescription() { return description; }

private:
	int id;
	std::string name, description;
};

typedef boost::shared_ptr<Quest> QuestPtr;
typedef boost::unordered_map<int, QuestPtr> QuestMap;

class Quests
{
public:
	Quests();
	~Quests();
	static Quests *getInstance() { return instance; }

	bool load();

	Quest *getQuestByID(int id);

	std::string getNameByID(int id);
	std::string getDescriptionByID(int id);

private:
	static Quests *instance;
	QuestMap questMap;
};

#endif