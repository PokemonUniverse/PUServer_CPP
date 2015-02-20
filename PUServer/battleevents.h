#ifndef __b_events_h_
#define __b_events_h_

#include <iostream>
#include <sstream>

#include "packet.h"
#include "battleplayer.h"
#include "player.h"
#include "gametext.h"

class BattlePlayer;
class Player;

extern GameText g_gameText;

using namespace std;

enum BattleEvent
{
	BATTLEEVENT_STOPBATTLE = 999,
	BATTLEEVENT_SLEEP = 0,
	BATTLEEVENT_TEXTID = 1,
	BATTLEEVENT_TEXT = 2,
	BATTLEEVENT_CHANGEHP = 3,
	BATTLEEVENT_ANIMATION = 4,
	BATTLEEVENT_ALLOWCONTROL = 5,
	BATTLEEVENT_CHANGEPOKEMON_SELF = 6,
	BATTLEEVENT_CHANGEPOKEMON = 7,
	BATTLEEVENT_CHANGESELECTION = 8,
	BATTLEEVENT_CHANGEPP = 9,
	BATTLEEVENT_CHANGESTATUS = 10,
	BATTLEEVENT_CHANGELEVELSELF = 11,
	BATTLEEVENT_CHANGELEVEL = 12,
	BATTLEEVENT_CHANGEATTACK = 13,
	BATTLEEVENT_CHANGESCREEN = 14,
	BATTLEEVENT_DIALOGUE = 15,
	BATTLEEVENT_REMOVEPLAYER = 16,
	BATTLEEVENT_CHANGEEXP = 17,
	BATTLEEVENT_CHANGEMAXHP = 18,
};

enum BattleControlState	
{	
	BATTLECONTROL_CHOOSEMOVE = 1,
	BATTLECONTROL_CHOOSEPOKEMON = 2,
	BATTLECONTROL_CHOOSEPOKEMON_ITEM = 3,
	BATTLECONTROL_CHOOSEATTACK_ITEM = 4
};	

enum BattleSelectType	
{	
	BATTLESELECT_MOVE = 0,
	BATTLESELECT_POKEMON = 1,
	BATTLESELECT_RUN = 2,
	BATTLESELECT_ITEM = 3
};

enum BattleScreen
{
	BATTLESCREEN_MOVES = 0,
	BATTLESCREEN_YESNO = 1,
	BATTLESCREEN_ATTACKS = 2
};

enum BattleAnswer
{
	BATTLEANSWER_NONE = 0,
	BATTLEANSWER_YES = 1,
	BATTLEANSWER_NO = 2
};

enum AnimationTypes
{
	ANIMATION_ATTACK = 0,
	ANIMATION_ITEM = 1,
	ANIMATION_STATS = 2
};

enum Animations_Attacks
{
	ANIMATION_ATTACKS_HIT = 0
};

enum Animations_Item
{
	ANIMATION_ITEM_POKEBALL = 0
};


enum Animations_Stats
{
	ANIMATION_STATS_UP = 0,
	ANIMATION_STATS_DOWN = 1
};


class BattleEvents
{
public:
	static void SendBattleIntro(BattlePlayer *bPlayer, string opponentPokemon, string ownPokemon, bool isWildBattle=true, std::string playerName="")
	{
		// Intro packet
		if(isWildBattle) {
			std::string params[] = { opponentPokemon };		
			_TextPacket(bPlayer, GT_WILDENCOUNTER, params);
		}
		else {
			std::string params[] = { playerName, opponentPokemon };
			_TextPacket(bPlayer, GT_PLAYERENCOUNTER, params);
		}

		// Sleep
		_SleepPacket(bPlayer, 2000);

		// Set text
		std::string params2[] = { ownPokemon };
		_TextPacket(bPlayer, GT_POKEMONCHOOSEMOVE, params2);
	}

	static void SendBattleMoveAccept(BattlePlayer *bPlayer, int slotID)
	{
		// Send new text
		_TextPacket(bPlayer, GT_BATTLEWAITING);
	}

	static void SendBattleChangePokemon(BattlePlayer *bPlayer, int moveType)
	{
		// Send new text
		_TextPacket(bPlayer, GT_BATTLECHANGEPOKEMON);
	}

	static void SendBattlePokemonChangeAccept(BattlePlayer *bPlayer, int pokemonSlot)
	{
		// Send new text
		_TextPacket(bPlayer, GT_BATTLEWAITING);	
	}

	static void SendBattleRun(BattlePlayer *bPlayer)
	{
		// Send new text
		_TextPacket(bPlayer, GT_BATTLEWAITING);	
	}

	static void SendBattleAttack_RUN(BattlePlayer *bPlayer, bool canRun, string pokemonName)
	{
		// Send text
		_TextPacket(bPlayer, GT_BATTLERUN);	

		// Sleep for 4 sec
		_SleepPacket(bPlayer, 4000);

		if(!canRun)
		{
			// Send text
			_TextPacket(bPlayer, GT_BATTLERUNFAIL);	

			// Sleep for 2 sec
			_SleepPacket(bPlayer, 2000);
		}
		else
		{
			_TextPacket(bPlayer, GT_BATTLERUNSUCCES);		

			// Sleep for 2 sec
			_SleepPacket(bPlayer, 2000);
		}
	}
	
	static void SendBattleAttackBonus(BattlePlayer *bPlayer, int bonus)
	{
		if(bonus == BONUS_CRITICALHIT)
			_TextPacket(bPlayer, GT_BATTLEATTACKCRITICAL);
		else if(bonus == BONUS_NOTEFFECTIVE)
			_TextPacket(bPlayer, GT_BATTLENOTEFFECTIVE);
		else if(bonus == BONUS_NOTVERYEFFECTIVE)
			_TextPacket(bPlayer, GT_BATTLENOTVERYEFFECTIVE);
		else if(bonus == BONUS_VERYEFFECTIVE)
			_TextPacket(bPlayer, GT_BATTLEVERYEFFECTIVE);
		else if(bonus == BONUS_SUPEREFFECTIVE)
			_TextPacket(bPlayer, GT_BATTLESUPEREFFECTIVE);

		_SleepPacket(bPlayer, 2000);
	}

	static void SendBattleModifier(BattlePlayer *bPlayer, int modifierType, int modifier, string pokemonName, int position = 0)
	{
		string strModifier;

		switch (modifierType)
		{
		case STAT_DEFENSE:
			strModifier = "defense";
			break;
		case STAT_ATTACK:
			strModifier = "attack";
			break;
		case STAT_SPEED:
			strModifier = "speed";
			break;
		case STAT_SPECIALATTACK:
			strModifier = "special attack";
			break;
		case STAT_SPECIALDEFENSE:
			strModifier = "special defense";
			break;
		case STAT_ACCURACY:
			strModifier = "accuracy";
			break;
		case STAT_BURN:
			strModifier = "burned";
			break;
		case STAT_CONFUSED:			
			strModifier = "confused";
			break;
		case STAT_FLINCH:
			strModifier = "flinched";
			break;
		case STAT_FREEZE:
			strModifier = "frozen";
			break;
		case STAT_LEECH:
			strModifier = "leeched";
			break;
		case STAT_PARALIZED:
			strModifier = "paralized";
			break;
		case STAT_POISON:
			strModifier = "poisoned";
			break;
		case STAT_SLEEP:
			strModifier = "sleeping";
			break;
		case STAT_TARGETING:
			strModifier = "targeting";
			break;
		case STAT_TERRAIN:
			strModifier = "terrain";
			break;
		case STAT_TOXIC:
			strModifier = "toxic";
			break;
		case STAT_TRIATTACK:
			strModifier = "traiattack";
			break;
		case STAT_TRAP:
			strModifier = "trapped";
			break;
		default:
			strModifier = "unknown";
			break;
		}
		
		std::string params[] = { pokemonName, strModifier };

		if(modifierType >= 1 && modifierType <= 6)
		{	
			if(modifier == STATMOD_FELL) {
				_TextPacket(bPlayer, GT_BATTLESTATFELL, params);
				_AnimationPacket(bPlayer, position, ANIMATION_STATS, ANIMATION_STATS_DOWN);
			}
			else if(modifier == STATMOD_HARSHLYFELL) {
				_TextPacket(bPlayer, GT_BATTLESTATHARSHLYFELL, params);
				_AnimationPacket(bPlayer, position, ANIMATION_STATS, ANIMATION_STATS_DOWN);
			}
			else if(modifier == STATMOD_ROSE) {
				_TextPacket(bPlayer, GT_BATTLESTATROSE, params);
				_AnimationPacket(bPlayer, position, ANIMATION_STATS, ANIMATION_STATS_UP);
			}
			else if(modifier == STATMOD_SHARPLYROSE) {
				_TextPacket(bPlayer, GT_BATTLESTATSHARPLYROSE, params);
				_AnimationPacket(bPlayer, position, ANIMATION_STATS, ANIMATION_STATS_UP);
			}

			//_SleepPacket(bPlayer, 2000);
		}
		else if(modifierType == 0 && modifier > 0)
		{
			_TextPacket(bPlayer, GT_BATTLESTATMOD_ON, params);

			_SleepPacket(bPlayer, 2000);
		}
	}

	static void SendUpdateAttack(BattlePlayer *bPlayer, int pokemon, int slot, Attack *attack)
	{
		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_CHANGEATTACK);
		packet.AddInt(pokemon);
		packet.AddInt(slot);
		packet.AddString(attack->getAttackName());
		packet.AddString(attack->getAttackDesc());
		packet.AddString(attack->getAttackType());
		packet.AddInt(attack->getUsageLeft());
		packet.AddInt(attack->getUsageTotal());

		packet.AddInt(attack->getAttackPower());
		packet.AddInt(attack->getAttackAccuracy());
		packet.AddString(attack->getAttackClass());
		packet.AddString(attack->getTarget());
		packet.AddString(attack->getContact());

		bPlayer->sendDataPacket(packet);
	}

	static void SendRemovePlayer(BattlePlayer *bPlayer, BattlePlayer *removed, bool forfeit = false)
	{
		std::string params[] = { removed->getName() };
		if(forfeit) {
			_TextPacket(bPlayer, GT_BATTLEPLAYERFORFEIT, params);
		}
		else {
			_TextPacket(bPlayer, GT_BATTLEPLAYEROUT, params);
		}
		_SleepPacket(bPlayer, 1500);
		_RemovePlayer(bPlayer, removed->getBattleID());
	}

	//////////////////////////////////////////////////////////////////////////
	
	static void _SleepPacket(BattlePlayer *bPlayer, int amount)
	{
		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_SLEEP);
		packet.AddInt(amount);
		
		bPlayer->sendDataPacket(packet);
	}

	static void _EndBattlePacket(BattlePlayer *bPlayer)
	{
		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_STOPBATTLE);
		
		bPlayer->sendDataPacket(packet);
	}

	static void _AnimationPacket(BattlePlayer *bPlayer, int defenderPosition, int animationType, int animationID)
	{
		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_ANIMATION);
		packet.AddInt(defenderPosition);
		packet.AddInt(animationType);
		packet.AddInt(animationID);
		
		bPlayer->sendDataPacket(packet);
	}

	static void _ChangePokemon(BattlePlayer *bPlayer, int pokemonSlot)
	{
		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_CHANGEPOKEMON_SELF);
		packet.AddInt(pokemonSlot);
		
		bPlayer->sendDataPacket(packet);
	}

	static void _ChangePokemon(BattlePlayer *bPlayer, int playerID, int pokemonID, string pokemonName, int pokemonHP, int pokemonLevel)
	{
		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_CHANGEPOKEMON);
		packet.AddInt(playerID);
		packet.AddInt(pokemonID);
		packet.AddString(pokemonName);
		packet.AddInt(pokemonHP);
		packet.AddInt(pokemonLevel);
		
		bPlayer->sendDataPacket(packet);
	}

	static void _ChangeHPPacket(BattlePlayer *bPlayer, int id, int hp)
	{
		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_CHANGEHP);
		packet.AddInt(id);
		packet.AddInt(hp);
		
		bPlayer->sendDataPacket(packet);
	}

	static void _ChangePPPacket(BattlePlayer *bPlayer, int pokemonID, int moveSlotID, int newPP)
	{
		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_CHANGEPP);
		packet.AddInt(pokemonID);
		packet.AddInt(moveSlotID);
		packet.AddInt(newPP);	
		
		bPlayer->sendDataPacket(packet);
	}

	static void _ChangePokemonStatus(BattlePlayer *bPlayer, int playerID, int pokemonStatus)
	{
		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_CHANGESTATUS);
		packet.AddInt(playerID);
		packet.AddInt(pokemonStatus);	
		
		bPlayer->sendDataPacket(packet);
	}

	static void _ChangePokemonLevel(BattlePlayer *bPlayer, bool isSelf, int pID, int newLevel)
	{
		Packet packet;
		packet.AddByte(0xDE);
		
		if(isSelf)
			packet.AddInt(BATTLEEVENT_CHANGELEVELSELF);
		else
			packet.AddInt(BATTLEEVENT_CHANGELEVEL);

		packet.AddInt(pID);
		packet.AddInt(newLevel);	
		
		bPlayer->sendDataPacket(packet);
	}

	static void _TextPacket(BattlePlayer *bPlayer, int textID)
	{
		TextPieceList list = g_gameText.getText(textID);

		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_TEXT);
		packet.AddInt(1);
		packet.AddByte(list.front().color);
		packet.AddString(list.front().text);
		
		bPlayer->sendDataPacket(packet);
	}

	static void _TextPacket(BattlePlayer *bPlayer, int textID, std::string params[])
	{
		TextPieceList list = g_gameText.getText(textID);

		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_TEXT);
		packet.AddInt(list.size());
		
		int i = 0;
		for(TextPieceList::iterator it = list.begin(); it != list.end(); ++it)
		{
			packet.AddByte(it->color);
			if(it->var == 1) {
				packet.AddString(params[i++]);
			}
			else {
				packet.AddString(it->text);
			}
		}
		
		bPlayer->sendDataPacket(packet);
	}

	static void _TextPacket(Player *player, int textID, std::string params[])
	{
		TextPieceList list = g_gameText.getText(textID);

		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_TEXT);
		packet.AddInt(list.size());

		int i = 0;
		for(TextPieceList::iterator it = list.begin(); it != list.end(); ++it)
		{
			packet.AddByte(it->color);
			if(it->var == 1) {
				packet.AddString(params[i++]);
			}
			else {
				packet.AddString(it->text);
			}
		}
		
		player->sendPacket(packet);
	}


	static void _RemovePlayer(BattlePlayer *bPlayer, int removeID)
	{
		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_REMOVEPLAYER);
		packet.AddInt(removeID);

		bPlayer->sendDataPacket(packet);
	}

	static void _ControlPacket(BattlePlayer *bPlayer, int controlEvent)
	{
		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_ALLOWCONTROL);
		packet.AddInt(controlEvent);

		bPlayer->sendDataPacket(packet);
	}

	static void _DialoguePacket(BattlePlayer *bPlayer, DIALOGUE_TYPES type, std::string question, std::map<int, std::string> answers, int npc_id = 0)
	{
		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_DIALOGUE);
		packet.AddByte(type);

		if(type == DIALOGEU_NPC) {
			packet.AddInt(npc_id);
		}

		if(type != DIALOGUE_OPTIONS) {
			packet.AddString(question.c_str());
		}

		packet.AddByte(answers.size());

		for(std::map<int, std::string>::iterator it = answers.begin(); it != answers.end(); ++it)
		{
			packet.AddInt((*it).first);
			packet.AddString((*it).second.c_str());
		}

		bPlayer->sendDataPacket(packet);
	}

	static void _ExperiencePacket(BattlePlayer *bPlayer, int pokemonID, int exp)
	{
		Packet packet;
		packet.AddByte(0xDE);
		packet.AddInt(BATTLEEVENT_CHANGEEXP);
		packet.AddInt(pokemonID);
		packet.AddInt(exp);

		bPlayer->sendDataPacket(packet);
	}
};

#endif