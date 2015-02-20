#ifndef __ITEMS_H__
#define __ITEMS_H__

#include <boost/unordered_map.hpp>

#include "definitions.h"

#include "item.h"

class Items
{
public:
	static Items* instance()
	{
		static Items instance;
		return &instance;
	}
	
	~Items() {};

	bool load();
	bool reload();

	Item* getItem(uint32_t idx);

private:
	Items() : m_isLoaded(false) {};

	void clear();

	typedef boost::unordered_map<uint32_t, Item*> itemMap_t;
	itemMap_t m_items;

	bool m_isLoaded;
};

#endif
