#include "creature.h"
#include "pokemon.h"
#include "game.h"

extern Game g_game;

Pokemon::Pokemon(Database *database, int _uniqueID, bool isPlayerPokemon) : Creature()
{
	db = database;
	
	pokemonAttacks[0] = NULL;
	pokemonAttacks[1] = NULL;
	pokemonAttacks[2] = NULL;
	pokemonAttacks[3] = NULL;

	if(isPlayerPokemon) {
		loadPlayerPokemon(_uniqueID);
	}
	else
		loadNpcPokemon(_uniqueID, 0);

	setBattle(false);
	isInBattle = false;
}

Pokemon::Pokemon(Database *database, int _pokemonID, int _level) : Creature()
{
	db = database;
	loadNpcPokemon(_pokemonID, _level);
}

Pokemon::~Pokemon()
{

}

void Pokemon::loadPlayerPokemon(int uniqueID)
{
	// Select user pokemon info
	std::stringstream pkmnQuery;
	pkmnQuery << "SELECT u.uniqueID, u.pokemonID, u.level, u.pkmn_hp, u.pkmn_exp, u.pkmn_status, u.happiness, u.gender, um.uniqueMoveID, um.moveID, um.moveUsage,"
			  << " u.iv_hp, u.iv_at, u.iv_de, u.iv_sa, u.iv_sd, u.iv_sp,"
			  << " p.id, p.real_pokemon_id, p.name, p.base_exp, p.type1, p.type2, p.stat_at, p.stat_de, p.stat_sa, p.stat_sd, p.stat_sp, p.stat_hp, p.weight"
			  << " FROM user_pokemon AS `u`"
			  << " INNER JOIN pokemon AS `p` ON p.id = u.pokemonID"
			  << " LEFT JOIN user_pokemon_moves `um` ON um.uniqueID = u.uniqueID"
			  << " WHERE u.uniqueID='" << uniqueID << "'";

	Recordset *rst = new Recordset(db, pkmnQuery.str().c_str());

	if(!rst->moveNext())
		delete this;

	// Pokemon IV's
	iv_at = rst->getInt("iv_at");
	iv_de = rst->getInt("iv_de");
	iv_sa = rst->getInt("iv_sa");
	iv_sd = rst->getInt("iv_sd");
	iv_sp = rst->getInt("iv_sp");
	iv_hp = rst->getInt("iv_hp");

	// Pokemon Info
	pokemonID = uniqueID;
	pokemonDbID = rst->getInt("id");
	pokemonRealID = rst->getInt("real_pokemon_id");
	name = rst->getString("name");
	pokemonLvl = rst->getInt("level");
	pokemonType1 = rst->getString("type1");
	pokemonType2 = rst->getString("type2");
	pokemonStatus = rst->getInt("pkmn_status");
	weight = rst->getInt("weight");
	gender = rst->getInt("gender");

	base_at = rst->getInt("stat_at");
	base_de = rst->getInt("stat_de");
	base_sa = rst->getInt("stat_sa");
	base_sd = rst->getInt("stat_sd");
	base_sp = rst->getInt("stat_sp");
	base_hp = rst->getInt("stat_hp");

	stat_at = StatCalculator::Calculate_Stats(base_at, iv_at, 0, pokemonLvl);
	stat_de = StatCalculator::Calculate_Stats(base_de, iv_de, 0, pokemonLvl);
	stat_sa = StatCalculator::Calculate_Stats(base_sa, iv_sa, 0, pokemonLvl);
	stat_sd = StatCalculator::Calculate_Stats(base_sd, iv_sd, 0, pokemonLvl);
	stat_sp = StatCalculator::Calculate_Stats(base_sp, iv_sp, 0, pokemonLvl);

	pokemonHP_Total = StatCalculator::Calculate_HP(base_hp, iv_hp, 0, pokemonLvl);
	pokemonHP_Left = pokemonHP_Total - rst->getInt("pkmn_hp");
	baseExp = rst->getInt("base_exp");
	currentExp = rst->getInt("pkmn_exp");

	capture_rate = NULL;

	int moveID = rst->getInt("moveID");
	int uniqueMoveID = rst->getInt("uniqueMoveID");
	int moveUsage = rst->getInt("moveUsage");

	// First Attack
	pokemonAttacks[0] = new Attack(db, moveID, uniqueMoveID, moveUsage);
	
	int move_i = 1;
	while(rst->moveNext())
	{
		pokemonAttacks[move_i] = new Attack(db, rst->getInt("moveID"), rst->getInt("uniqueMoveID"), rst->getInt("moveUsage"));	
		move_i++;
	}

	loadFlavorText(pokemonDbID);

	attackCount = move_i;
	holdingItemID = 0;
	lastAttackID = 0;
	doneBattleAttack = false;
	attackLearning = false;
	isWild = false;
	StatusAilment = 0;
	mod_ac = 0;
	mod_at = 0;
	mod_de = 0;
	mod_sa = 0;
	mod_sd = 0;
	mod_sp = 0;

	delete rst;	
}

void Pokemon::loadNpcPokemon(int pokemonID, int level)
{
	// Select wild pokemon info
	std::stringstream pkmnQuery;
	pkmnQuery << "SELECT DISTINCT pm.move_id, pm.level, p.base_exp, p.id, p.name, p.type1, p.type2, p.stat_at, p.stat_de, p.stat_sa, p.stat_sd, p.stat_sp, p.stat_hp, p.capture_rate, p.base_happiness, p.weight"
			  << " FROM pokemon AS `p`"
			  << " LEFT JOIN pokemon_moves AS `pm` ON pm.pokemon_id = p.id"
			  << " WHERE p.id='" << pokemonID << "' AND pm.level <= '" << level << "' AND pm.method='level'"
			  << " ORDER BY RAND() DESC LIMIT 0,4";

	Recordset *rst = new Recordset(db, pkmnQuery.str().c_str());

	if(!rst->moveNext())
		return;

	// Calculate pokemon IV's
	iv_at = StatCalculator::Calculate_IV();
	iv_de = StatCalculator::Calculate_IV();
	iv_sa = StatCalculator::Calculate_IV();
	iv_sd = StatCalculator::Calculate_IV();

	iv_sp = StatCalculator::Calculate_IV();
	iv_hp = StatCalculator::Calculate_IV();

	// Pokemon Info
	pokemonID = rst->getInt("id");
	pokemonDbID = rst->getInt("id");
	pokemonRealID = pokemonID;
	name = rst->getString("name");
	pokemonLvl = level;
	pokemonType1 = rst->getString("type1");
	pokemonType2 = rst->getString("type2");
	weight = rst->getInt("weight");
	gender = random_range(0,1);

	baseExp = rst->getInt("base_exp");
	capture_rate = rst->getInt("capture_rate");
	
	base_at = rst->getInt("stat_at");
	base_de = rst->getInt("stat_de");
	base_sa = rst->getInt("stat_sa");
	base_sd = rst->getInt("stat_sd");
	base_sp = rst->getInt("stat_sp");
	base_hp = rst->getInt("stat_hp");

	stat_at = StatCalculator::Calculate_Stats(base_at, iv_at, 0, pokemonLvl);
	stat_de = StatCalculator::Calculate_Stats(base_de, iv_de, 0, pokemonLvl);
	stat_sa = StatCalculator::Calculate_Stats(base_sa, iv_sa, 0, pokemonLvl);
	stat_sd = StatCalculator::Calculate_Stats(base_sd, iv_sd, 0, pokemonLvl);
	stat_sp = StatCalculator::Calculate_Stats(base_sp, iv_sp, 0, pokemonLvl);
	//currentExp = StatCalculator::Calculate_EXP(baseExp, pokemonLvl, true);
	currentExp = StatCalculator::Calculate_LevelEXP(99) + 8000;

	if(pokemonID == 129) { 
		pokemonHP_Total = 1;
	} else {
		pokemonHP_Total = StatCalculator::Calculate_HP(base_hp, iv_hp, 0, level);
	}
	pokemonHP_Left = pokemonHP_Total;

	// First Attack
	//pokemonAttacks[0] = new Attack(db, rst->getInt("move_id"), 0, 0);
	if (pokemonID == 129) {
		pokemonAttacks[0] = new Attack(db, 150, 0, 0); // Splash
	} else {
		pokemonAttacks[0] = new Attack(db, 109, 0, 0); // Confuse Ray
	}
	
	
	int move_i = 1;/*
	while(rst->moveNext())
	{
		pokemonAttacks[move_i] = new Attack(db, rst->getInt("move_id"), 0, 0);	
		move_i++;
	}*/

	loadFlavorText(pokemonDbID);

	attackCount = move_i;
	holdingItemID = 0;
	lastAttackID = 0;
	doneBattleAttack = false;
	attackLearning = false;
	isWild = true;
	pokemonStatus = 0;
	StatusAilment = 0;
	mod_ac = 0;
	mod_at = 0;
	mod_de = 0;
	mod_sa = 0;
	mod_sd = 0;
	mod_sp = 0;

	delete rst;
}

void Pokemon::increaseHP(int _addHP)
{
	pokemonHP_Left += _addHP;

	if(pokemonHP_Left > pokemonHP_Total)
		pokemonHP_Left = pokemonHP_Total;
}

void Pokemon::decreaseHP(int _delHP)
{
	pokemonHP_Left -= _delHP;

	if(pokemonHP_Left < 0)
		pokemonHP_Left = 0;

	if(hasFlag(StatusAilment_ENDURE)) {
		pokemonHP_Left = 1;
	}
}

void Pokemon::setHappiness(int _happiness)
{
	happiness += _happiness;

	if(happiness < 0)
		happiness = 0;
	else if(happiness > 255)
		happiness = 255;
}

int Pokemon::increaseExp(int ExPoints)
{	
	if(isWild)
		return 0;

	currentExp += ExPoints;
	int TotalNextLvl = 0;
	int newPokemonLvl = 0;
	
	for(;;)
	{
		// Points needed for next level
		TotalNextLvl = StatCalculator::Calculate_LevelEXP((getPokemonLvl()+1));

		if(currentExp >= TotalNextLvl)
		{
			pokemonLvl++;
			newPokemonLvl = pokemonLvl;
			
			levelUp(pokemonLvl);
		}
		else
			break;
	}
	
	return newPokemonLvl;
}

void Pokemon::addMoveHistory(int TurnID, TurnMoveInfo moveInfo)
{
	moveHistory.insert(pair<int, TurnMoveInfo>(TurnID, moveInfo));
}

void Pokemon::useAttack(int slotID)
{
	if(getAttack(slotID) == NULL)
		return;

	getAttack(slotID)->useAttack();
}

Attack* Pokemon::updateAttack(int slotID, int newAttackID)
{
	Attack *oldAttack = getAttack(slotID);
	int uniqueID = 0;

	if(oldAttack == NULL) // New attack
	{
		std::stringstream pkmnAttack;
		pkmnAttack << "INSERT INTO user_pokemon_moves (uniqueID, moveID) "
			       << "VALUES ('" << pokemonID << "','" << newAttackID << "')";
		db->runQuery(pkmnAttack.str());
		uniqueID = db->getLastID();
	}
	else // Update old attack
	{
		uniqueID = oldAttack->getUniqueID();
		std::stringstream pkmnAttack;
		pkmnAttack << "UPDATE user_pokemon_moves SET "
			       << "moveID='" << newAttackID << "',"
				   << "moveEV='0',"
				   << "moveUsage='0'"
				   << "WHERE uniqueMoveID='" << uniqueID << "'";

		db->runQuery(pkmnAttack.str());
	}

	delete pokemonAttacks[slotID];
	pokemonAttacks[slotID] = new Attack(db, newAttackID, uniqueID, 0);

	return pokemonAttacks[slotID];
}

int Pokemon::getLastTurnID()
{
	if(moveHistory.size() <= 0)
		return 0;

	std::map<int, TurnMoveInfo>::iterator it = moveHistory.end();
	--it;

	TurnMoveInfo moveInfo = it->second;	
	return moveInfo.turnID;
}

TurnMoveInfo Pokemon::getLastTurnInfo()
{
	TurnMoveInfo dummy;
	if(moveHistory.size() <= 0)
		return dummy;

	std::map<int, TurnMoveInfo>::iterator it = moveHistory.end();
	--it;

	return (TurnMoveInfo)it->second;
}

int Pokemon::getLastAttackID()
{
	if(moveHistory.size() <= 0)
		return 0;

	std::map<int, TurnMoveInfo>::iterator it = moveHistory.end();
	--it;

	TurnMoveInfo moveInfo = it->second;	
	return moveInfo.attackID;
}

void Pokemon::savePokemonData(bool saveAttackData)
{
	if(isWild)
		return;

	// Save pokemon data
	std::stringstream pkmnSaveQuery;
	pkmnSaveQuery << "UPDATE user_pokemon SET "
				  << "level='" << pokemonLvl << "',"
				  << " pkmn_hp='" << (pokemonHP_Total - pokemonHP_Left) << "',"
				  << " pkmn_exp='" << currentExp << "',"
				  << " pkmn_status='" << pokemonStatus << "'"
				  << " WHERE uniqueID='" << pokemonID << "'";

	db->runQuery(pkmnSaveQuery.str());
	
	if(!saveAttackData)
		return;

	// Save attack data
	for(int i = 0; i < attackCount; i++)
	{
		if(getAttack(i))
		{
			int attackUsage = (getAttack(i)->getUsageTotal() - getAttack(i)->getUsageLeft());

			std::stringstream pkmnSaveQuery;
			pkmnSaveQuery << "UPDATE user_pokemon_moves SET "
						  << "moveUsage='" << attackUsage << "'"
						  << " WHERE uniqueMoveID='" << getAttack(i)->getUniqueID() << "'";	

			db->runQuery(pkmnSaveQuery.str());
		}
	}
}

void Pokemon::heal()
{
	pokemonHP_Left = pokemonHP_Total;
	statusCount = 0;
	pokemonStatus = 0;
	StatusAilment = 0;

	for(int i = 0; i < attackCount; i++)
	{
		if(getAttack(i))
			getAttack(i)->resetAttack();
	}
}

std::map<int, std::string> Pokemon::getAttackList()
{
	std::map<int, std::string> attackList;
	for(int i = 0; i < attackCount; i++)
	{
		if(getAttack(i)) {
			attackList.insert(std::make_pair(i, getAttack(i)->getAttackName()));
		}
	}

	return attackList;
}

bool Pokemon::hasUsableAttack()
{
	for(int i = 0; i < attackCount; i++)
	{
		if(getAttack(i) && getAttack(i)->getUsageLeft() > 0) {
			return true;
		}
	}

	return false;
}

void Pokemon::levelUp( int level )
{
	stat_at = StatCalculator::Calculate_Stats(base_at, iv_at, 0, level);
	stat_de = StatCalculator::Calculate_Stats(base_de, iv_de, 0, level);
	stat_sa = StatCalculator::Calculate_Stats(base_sa, iv_sa, 0, level);
	stat_sd = StatCalculator::Calculate_Stats(base_sd, iv_sd, 0, level);
	stat_sp = StatCalculator::Calculate_Stats(base_sp, iv_sp, 0, level);	
	pokemonHP_Total = StatCalculator::Calculate_HP(base_hp, iv_hp, 0, level);
}

int Pokemon::getExpPercent()
{
	double TotalExpNextLvl = StatCalculator::Calculate_LevelEXP((getPokemonLvl()+1));
	double ExpToLevel = TotalExpNextLvl - StatCalculator::Calculate_LevelEXP((getPokemonLvl()));
	double CurrentExpToLevel = getCurrentExp()-StatCalculator::Calculate_LevelEXP((getPokemonLvl()));
	double expPercent = (CurrentExpToLevel/ExpToLevel) * 100.;
	
	int expTotal = (int)floor(expPercent);
	if(expTotal > 100) {
		expTotal = 100;
	}
	
	return expTotal;
}

void Pokemon::loadFlavorText( int pokemonID )
{
	std::stringstream query;
	query << "SELECT text FROM flavor_text WHERE pokemon_id=" << pokemonID << " LIMIT 0,1";
	boost::scoped_ptr<Recordset> flavor(new Recordset(db, query.str()));

	if(flavor->moveNext()) 
	{
		flavorText = flavor->getString("text");
	}
}
