#include "items.h"

Item::Item()
{
	id = 0;
	name = "";
	description = "";
	count = 0;
}

Item::Item(int _id, std::string _name, std::string _description, int _count, int _berry_id)
{
	id = _id;
	name = _name;
	description = _description;
	count = _count;

	if(_berry_id > 0)
	{
		std::string query = boost::str(boost::format("SELECT id, size, firmness, max_harvest, growth_time, soil_dryness, spicy, dry, sweet, bitter, sour, smoothness FROM berries WHERE id=%d") % _berry_id);
		boost::scoped_ptr<Recordset> berryInfo(new Recordset(&g_db, query));

		if(berryInfo->numOfRows() > 0 && berryInfo->moveNext()) 
		{
			berry_id = _berry_id;

			setSize(berryInfo->getInt("size"));
			setFirmness(berryInfo->getString("firmness"));
			setMaxHarvest(berryInfo->getInt("max_harvest"));
			setGrowthTime(berryInfo->getInt("growth_time"));
			setSoilDryness(berryInfo->getInt("soil_dryness"));
			setSpicy(berryInfo->getInt("spicy"));
			setDry(berryInfo->getInt("dry"));
			setSweet(berryInfo->getInt("sweet"));
			setBitter(berryInfo->getInt("bitter"));
			setSour(berryInfo->getInt("sour"));
			setSmoothness(berryInfo->getInt("smoothness"));
		}
	}
}

Item* Item::clone()
{
	Item *item = new Item(*this);

	return item;
}

void Item::setCategory(std::string _category)
{
	category = _category;
	categoryID = getCategoryByName(_category);
}

int Item::getCategoryByName(std::string _name)
{
	if(_name == "powerups")
		return ITEM_POWERUPS;
	else if(_name == "berries")
		return ITEM_BERRIES;
	else if(_name == "equipment")
		return ITEM_EQUIPMENT;
	else if(_name == "pokeball")
		return ITEM_POKEBALL;
	else if(_name == "held")
		return ITEM_HELD;
	else if(_name == "quest")
		return ITEM_QUEST;
	else
		return ITEM_MISC;
}

std::string Item::getCategoryByID(int _id)
{
	switch(_id)
	{
	case ITEM_POWERUPS: return "powerups"; break;
	case ITEM_BERRIES: return "berries"; break;
	case ITEM_EQUIPMENT: return "equipment"; break;
	case ITEM_POKEBALL: return "pokeball"; break;
	case ITEM_HELD: return "held"; break;
	case ITEM_QUEST: return "quest"; break;
	default: return "misc"; break;
	}
}