#include "quest.h"

#include "database.h"
#include "logger.h"

Quests::~Quests()
{
	questMap.clear();
}

bool Quests::load()
{
	Database* db = Database::getFactory();
	DBQuery selectQuests;
	DBResult* result;

	selectQuests << "SELECT id, name, description FROM quests";
	if((result = db->storeQuery(selectQuests.str())) == NULL) 
	{
		Logger::consoleOutput(COUTPUT_ERROR, "Error fetching quests from database.");
		return false;
	}

	uint32_t id;
	std::string name, description;
	while(result->next())
	{
		id = result->getDataInt("id");
		name = result->getDataString("name");
		description = result->getDataString("description");

		Quest* quest = new Quest(id, name, description);
		questMap.insert(std::pair<uint32_t, Quest*>(id, quest));
	}

	return true;
}

std::string Quests::getNameByID(uint32_t id)
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

std::string Quests::getDescriptionByID(uint32_t id)
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

Quest* Quests::getQuestByID(uint32_t id)
{
	QuestMap::iterator it = questMap.find(id);
	if(it != questMap.end())
	{
		if(it->second)
		{
			return it->second;
		}
	}

	return NULL;
}