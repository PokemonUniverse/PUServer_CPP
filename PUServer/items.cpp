#include "items.h"

Items::Items()
{
	isLoaded = false;
}

Items::~Items()
{

}

void Items::clear()
{
	BOOST_FOREACH(ItemsList::value_type i, items)
	{
		delete i.second;
	}

	items.clear();
}

bool Items::reload()
{
	if(isLoaded) {
		clear();
	}

	std::stringstream query;
	query << "SELECT id, berry_id, name, pu_category, cost, fling_power, fling_effect, natural_gift_power, natural_gift_type,"
		  << " effort_hp, effort_at, effort_de, effort_sp, effort_sa, effort_sd, hp_restored, pp_restored, happiness1, happiness2,"
		  << " happiness3, description FROM items";
	boost::scoped_ptr<Recordset> itemsList(new Recordset(&g_db, query.str()));

	if(itemsList->numOfRows() <= 0)
		return true;

	while(itemsList->moveNext())
	{
		int berryID = itemsList->getInt("berry_id");

		Item *item = new Item(itemsList->getInt("id"), itemsList->getString("name"), itemsList->getString("description"), 0, berryID);
		
		item->setCategory(itemsList->getString("pu_category"));
		item->setCost(itemsList->getInt("cost"));
		item->setFlingPower(itemsList->getInt("fling_power"));
		item->setFlingDescription(itemsList->getString("fling_effect"));
		item->setNaturalGiftPower(itemsList->getInt("natural_gift_power"));
		item->setNaturalGiftType(itemsList->getInt("natural_gift_type"));
		item->setEffort(itemsList->getInt("effort_hp"),itemsList->getInt("effort_at"),itemsList->getInt("effort_de"),
						itemsList->getInt("effort_sa"),itemsList->getInt("effort_sd"), itemsList->getInt("effort_sp"));
		item->setPlusHP(itemsList->getInt("hp_restored"));
		item->setPlusPP(itemsList->getInt("pp_restored"));
		item->setHappiness(itemsList->getInt("happiness1"),itemsList->getInt("happiness2"),itemsList->getInt("happiness3"));
		item->setDescription(itemsList->getString("description"));

		items.insert(std::make_pair<int, Item*>(item->getID(), item));
	}

	isLoaded = true;

	return true;
}

Item* Items::getItem( int _id )
{
	ItemsList::iterator it = items.find(_id);
	if(it != items.end())
	{
		return it->second;
	}

	return NULL;
}