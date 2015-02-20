#ifndef INC_QUEST_H_
#define INC_QUEST_H_

#include "definitions.h"

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

class Quest;
typedef boost::unordered_map<uint32_t, Quest*> QuestMap;

class Quest
{
public:
	Quest(int _id, std::string _name, std::string _description)
		: m_id(_id),
		m_name(_name),
		m_description(_description) {}

	~Quest() {}

	uint32_t getID() { return m_id; }
	std::string getName() { return m_name; }
	std::string getDescription() { return m_description; }

private:
	uint32_t m_id;
	std::string m_name;
	std::string m_description;
};

class Quests
{
public:
	Quests() {};
	~Quests();

	static Quests* instance() {
		static Quests instance;
		return &instance;
	}

	bool load();

	std::string getNameByID(uint32_t id);
	std::string getDescriptionByID(uint32_t id);
	Quest* getQuestByID(uint32_t id);

private:
	QuestMap questMap;
};
#endif