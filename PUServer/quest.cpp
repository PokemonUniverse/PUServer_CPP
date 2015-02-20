#include "quest.h"

Quests *Quests::instance = NULL;

Quests::Quests()
{
	instance = this;
}

Quests::~Quests()
{
	questMap.clear();
}

bool Quests::load()
{
	std::string query = "SELECT id, name, description FROM quests";
	boost::scoped_ptr<Recordset> quests(new Recordset(&g_db, query));

	if(quests->numOfRows() <= 0)
		return true;

	int id;
	std::string name, description;
	while(quests->moveNext())
	{
		id = quests->getInt("id");
		name = quests->getString("name");
		description = quests->getString("description");
		
		QuestPtr quest(new Quest(id, name, description));
		std::pair<int, QuestPtr> newQuest(id, quest);
		questMap.insert(newQuest);
	}

	return true;
}

std::string Quests::getNameByID(int id)
{
	QuestMap::iterator it = questMap.find(id);
	if(it != questMap.end())
	{
		if(it->second)
		{
			return it->second->getName();
		}
	}
	return "";
}

std::string Quests::getDescriptionByID(int id)
{
	QuestMap::iterator it = questMap.find(id);
	if(it != questMap.end())
	{
		if(it->second)
		{
			return it->second->getDescription();
		}
	}
	return "";
}

Quest *Quests::getQuestByID(int id)
{
	QuestMap::iterator it = questMap.find(id);
	if(it != questMap.end())
	{
		if(it->second)
		{
			return it->second.get();
		}
	}
	return NULL;
}