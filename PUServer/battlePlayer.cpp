#include "battlePlayer.h"
#include "player.h"

BattlePlayer::BattlePlayer(Creature *creature)
{
	myPlayer = creature;
	playerType = PLAYER;
	realPlayerType = creature->getCreatureType();
	isRemoved = false;
	setPlayerStatus(PS_WAITING);
	attackCharge = 0;
	battleFlags = 0;
	myPlayer->setBattle(true);

	isFainted = false;
}

BattlePlayer::BattlePlayer(Database *db, int pokemonId, int level)
{
	myPokemon = new Pokemon(db, pokemonId, level);
	playerType = POKEMON;
	realPlayerType = POKEMON;
	isRemoved = false;
	attackCharge = 0;
	battleFlags = 0;
	setPlayerStatus(PS_WAITING);

	isFainted = false;
}

BattlePlayer::~BattlePlayer()
{
	if(playerType == POKEMON) {
		delete myPokemon;
	}
}

std::string BattlePlayer::getName()
{
	if(playerType == PLAYER) {
		return myPlayer->getName();
	}

	return myPokemon->getName();
}

Creature *BattlePlayer::getCreature()
{
	if(playerType == PLAYER) {
		return myPlayer;
	}

	return NULL;
}

void BattlePlayer::sendDataPacket(Packet packet)
{
	if(playerType == PLAYER && myPlayer->getPlayer() != NULL) {
		myPlayer->getPlayer()->sendPacket(packet);
	}
}

Pokemon *BattlePlayer::getPokemon(int slot)
{
	if(playerType == PLAYER) {
		return myPlayer->getPokemon(slot);
	}

	return myPokemon;
}

void BattlePlayer::setPokemon(int slot)
{
	if(playerType == PLAYER) {
		myPlayer->setActivePokemon(slot);
	}
}

Pokemon *BattlePlayer::getPokemonActive()
{
	if(playerType == PLAYER) {
		return myPlayer->getActivePokemon();
	}

	return myPokemon;
}

bool BattlePlayer::checkPokemonNext()
{
	if(playerType == PLAYER) {
		return myPlayer->checkNextPokemon();
	}
	
	return false;
}

int BattlePlayer::getPokemonActiveSlot()
{
	if(playerType == PLAYER) {
		return myPlayer->getActivePokemonSlot();
	}

	return 0;
}

int BattlePlayer::getPokemonTotal()
{
	if(playerType == PLAYER) {
		return myPlayer->getPokemonCount();
	}

	return 1;
}

int BattlePlayer::getPokemonTotalActive()
{
	if(playerType == PLAYER) {
		return myPlayer->getPokemonCountActive();
	}

	return 1;
}

BattlePlayerStatus_t BattlePlayer::getPlayerStatus()
{
	if(playerType == PLAYER) {
		return myPlayer->getPlayerStatus();
	}

	return playerStatus;
}

void BattlePlayer::setPlayerStatus(BattlePlayerStatus_t newStatus)
{
	if(playerType == PLAYER) {
		myPlayer->setPlayerStatus(newStatus);
	}
	else {
		playerStatus = newStatus;
	}
}

MoveType_t BattlePlayer::getMoveOption()
{
	if(playerType == PLAYER) {
		return myPlayer->getMoveOption();
	}

	return moveOption;
}
void BattlePlayer::setMoveOption(MoveType_t option)
{
	if(playerType == PLAYER) {
		myPlayer->setMoveOption(option);
	}
	else {
		moveOption = option;
	}
}

long BattlePlayer::getMoveParameter()
{
	if(playerType == PLAYER) {
		return myPlayer->getMoveParameter();
	}

	return moveParameter;
}
void BattlePlayer::setMoveParameter(long param)
{
	if(playerType == PLAYER) {
		myPlayer->setMoveParameter(param);
	}
	else {
		moveParameter = param;
	}
}

void BattlePlayer::setMoveValidated(int param)
{
	if(playerType == PLAYER) {
		myPlayer->setMoveValidated(param);
	}
	else {
		moveValidated = param;
	}
}

void BattlePlayer::resetBattleInfo()
{
	if(playerType == PLAYER)
	{
		myPlayer->resetBattleInfo();
	}
	else
	{
		moveOption = MOVE_NONE;
		moveParameter = -1;
		moveValidated = 0;
		playerStatus = PS_WAITING;
	}
}

void BattlePlayer::setActivePokemon()
{
	if(realPlayerType == NPC) {
		myPlayer->setActivePokemon();
	}
}

void BattlePlayer::setAttackCharge( int amount )
{
	if(playerType == PLAYER) {
		myPlayer->setBattleAttackCharge(amount);
	}
	else {
		attackCharge = amount;
	}
}

int BattlePlayer::getAttackCharge()
{
	if(playerType == PLAYER) {
		return myPlayer->getBattleAttackCharge();
	}

	return attackCharge;
}

PokemonList BattlePlayer::getPokemonList()
{
	return myPlayer->getPokemonParty();
}