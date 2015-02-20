#include "holditem.h"

HoldItemData* HoldItem::m_default = new HoldItemData();

void HoldItemData::addExclusiveItem( std::string name, std::string pokemon )
{
	itemSet_t list;

	exclusiveList_t::iterator it = m_exclusive.find(pokemon);
	if(it == m_exclusive.end())
	{	
		list.push_back(name);
		m_exclusive.insert(std::make_pair<std::string, itemSet_t>(pokemon, list));
	}
	else
	{
		list = it->second;
		list.push_back(name);
	}
}

void HoldItemData::removeExclusiveItem( std::string name, std::string pokemon )
{
	exclusiveList_t::iterator it = m_exclusive.find(pokemon);
	if(it == m_exclusive.end())	{	
		return;
	}

	/*
	itemSet_t list = it->second;
	itemSet_t::iterator listit = list.begin();
	for(; listit != list.end(); ++listit) {
		std::string tmp = listit>;
		if(tmp == name) {
			listit = list.erase(listit);
		}
	}

	it->second = list;*/

}

itemSet_t HoldItemData::getItemSet( std::string pokemon )
{
	exclusiveList_t::iterator it = m_exclusive.find(pokemon);
	if(it == m_exclusive.end())	{
		return m_items;
	}

	itemSet_t items = m_items;
	itemSet_t list = it->second;
	foreach(std::string itm, list) {
		items.push_back(itm);
	}

	return items;
}

bool HoldItemData::canUseitem( std::string pokemon, std::string item )
{
	foreach(std::string itm, m_items) {
		if(itm == item) return true;
	}

	exclusiveList_t::iterator it = m_exclusive.find(pokemon);
	if(it == m_exclusive.end()) {
		return false;
	}

	foreach(std::string itm, it->second) {
		if(itm == item) return true;
	}

	return false;
}

/************************************************************************/
/* HoldItem                                                             */
/************************************************************************/
HoldItem::HoldItem( std::string name, pokemonList_t pokemon ) : IntrinsicAbility(false, name)
{
	foreach(Pokemon* p, pokemon) {
		m_default->addExclusiveItem(name, p->getName());
	}
}

void HoldItem::registerAbility()
{
	IntrinsicAbility::registerAbility();
	m_default->addItem(getName());
}