#include "ioplayer.h"

#include "database.h"
#include "player.h"
#include "game.h"
#include "outfit.h"
#include "towns.h"

extern Game g_game;

bool IOPlayer::loadPlayer( Player* player, const std::string& name, bool preload /*= false*/ )
{
	player->setName(name);

	Database* db = Database::getFactory();
	DBResult* res;
	
	// Load default player data
	DBQuery query;
	query	<< "SELECT p.`idplayer`, p.`idaccount`, p.`name`, p.`position`, p.`movement`, p.`money`, p.`idlocation`, "
			<< "po.`head`, po.`nek`, po.`upper`, po.`lower`, po.`feet`, pc.`position` AS `pc_position` "
			<< "FROM `player` AS p "
			<< "JOIN `player_outfit` AS po ON po.`idplayer`=p.`idplayer` "
			<< "JOIN `pokecenter` AS pc ON pc.`idpokecenter`=p.`idpokecenter` "
			<< "WHERE p.`name`='%1%'";

	std::string user_query = boost::str(boost::format(query.str()) % name);
	if((res = db->storeQuery(user_query.c_str())) == NULL) {
		db->freeResult(res);
		return false;
	}
	
	player->setID(res->getDataInt("idplayer"));
	player->setName(res->getDataString("name"));
	player->setMovement((creaturemovement_t)res->getDataInt("movement"));

	Position position = Position(res->getDataString("position"));
	player->setTile(g_game.getTile(position));
	
	Town* town = Towns::getInstance()->getTown(res->getDataInt("idlocation"));
	player->setTown(town);

	Position pc_position = Position(res->getDataString("pc_position"));
	player->setPokeCenter(pc_position);

	player->setMoney(res->getDataInt("money"));

	// Outfit
	Outfit outfit;
	outfit.setKey(OUTFIT_HEAD, res->getDataInt("head"));
	outfit.setKey(OUTFIT_NEK, res->getDataInt("nek"));
	outfit.setKey(OUTFIT_UPPER, res->getDataInt("upper"));
	outfit.setKey(OUTFIT_LOWER, res->getDataInt("lower"));
	outfit.setKey(OUTFIT_FEET, res->getDataInt("feet"));
	player->setOutfit(outfit);

	db->freeResult(res);

	return true;
}

bool IOPlayer::savePlayer( Player* player )
{
	return true;
}