#ifndef __ITEM_H_
#define __ITEM_H_

#include "thing.h"

class Item : virtual public Thing
{
public:
	Item() {};
	virtual ~Item() {};

	Item* getItem() { return this; };
	const Item* getItem() const { return this; };

	std::string getDescription(int32_t lookDistance) { return m_name; };

	void setName(std::string name) { m_name = name; };
	std::string getName() { return m_name; };

private:
	std::string m_name;
};


#endif