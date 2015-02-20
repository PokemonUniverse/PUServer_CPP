#ifndef __enums_h_
#define __enums_h_

enum BlockingVolumes
{
	VOLUME_BLOCK = 1,
	VOLUME_WALK = 2,
	VOLUME_SURF = 3,
	VOLUME_BLOCKTOP = 4,
	VOLUME_BLOCKBOTTOM = 5,
	VOLUME_BLOCKRIGHT = 6,
	VOLUME_BLOCKLEFT = 7,
	VOLUME_BLOCKCORNER_TR = 8, // top right
	VOLUME_BLOCKCORNER_BR = 9, // bottom right
	VOLUME_BLOCKCORNER_BL = 10, // bottom left
	VOLUME_BLOCKCORNER_TL = 11 // Top left	
};

enum Direction
{
	DIR_NULL = 0,
	DIR_SOUTH = 1,
	DIR_WEST = 2,
	DIR_NORTH = 3,
	DIR_EAST = 4
};

enum playersex_t
{
	PLAYERSEX_FEMALE = 0,
	PLAYERSEX_MALE = 1,
};

enum BattleChallenge_t
{
	CHALLENGED = 0,
	CHALLENGING = 1
};

enum StopCodes
{
	FINISHED = 0,
	DISCONNECT = 1,
	RUN = 2
};

enum BattlePlayerStatus_t
{
	PS_WAITING = 0,	
	PS_FINISHED = 1,
	PS_POKEMONCHANGEASK = 2,
	PS_POKEMONCHANGE = 3,
	PS_CANATTACKLEARN = 4,
	PS_ATTACKLEARN = 5,
	PS_ATTACKASSIGN = 6,
	PS_DISCONNECTED = 7,
	PS_READY = 8,
	PS_ITEMCHOOSEPOKEMON = 9,
	PS_ISCATCHED = 10,
};

enum PlayerTypes
{
	CREATURE = -1,
	PLAYER = 0,
	NPC = 1,
	POKEMON = 2,
	SELF = 3
};

enum MoveType_t
{
	MOVE_NONE = -1,
	MOVE_ATTACK = 0,
	MOVE_POKEMON = 4,
	MOVE_BAG = 5, // Items
	MOVE_RUN = 6, // Forfeit from battle (only wild)
	MOVE_POKEMONCHANGE = 7, // Change pokemon
	MOVE_FAINT = 8,
	MOVE_ANSWER = 9,
	MOVE_MISSED = 10,
	MOVE_STATUSCHANGE = 11,
	MOVE_ITEM_POKEBALL = 12,
	MOVE_ITEM_POWERUP = 13,
	MOVE_FINISH = 99
};

enum BattleStatus
{
	BATTLESTATUS_CONTINUE = 0,
	BATTLESTATUS_ENDWON = 1,
	BATTLESTATUS_ENDLOST = 2,
	BATTLESTATUS_CHANGEPOKEMON = 3,
	BATTLESTATUS_FORFEIT = 4
};

enum MoveBonus
{
	BONUS_NONE = 0,
	BONUS_NOTEFFECTIVE = 1,
	BONUS_NOTVERYEFFECTIVE = 2,
	BONUS_VERYEFFECTIVE = 3,
	BONUS_SUPEREFFECTIVE = 4,
	BONUS_CRITICALHIT = 5
};

enum Stat
{
	STAT_FAINTED = -1,
	STAT_NONE = 0,
	STAT_DEFENSE = 1,
	STAT_ATTACK = 2,
	STAT_SPEED = 3,
	STAT_SPECIALATTACK = 4,
	STAT_SPECIALDEFENSE = 5,
	STAT_ACCURACY = 6,
	STAT_BURN = 7,
	STAT_CONFUSED = 8,
	STAT_FLINCH = 9,
	STAT_FREEZE = 10,
	STAT_LEECH = 11,
	STAT_PARALIZED = 12,
	STAT_POISON = 13,
	STAT_SLEEP = 14,
	STAT_TARGETING = 15,
	STAT_TERRAIN = 16,
	STAT_TOXIC = 17,
	STAT_TRIATTACK = 18,
	STAT_TRAP = 19,
	STAT_ATTRACTION = 20
};

enum StatModifier
{
	STATMOD_NONE = 0,
	STATMOD_HARSHLYFELL = 1,
	STATMOD_FELL = 2,
	STATMOD_ROSE = 3,
	STATMOD_SHARPLYROSE = 4
};

enum SpeakClasses {
	SPEAK_SAY				= 0x01,	//normal talk
	SPEAK_WHISPER			= 0x02,	//whispering - #w text
	SPEAK_YELL				= 0x03,	//yelling - #y text
	SPEAK_PRIVATE_PN		= 0x04, //Player-to-NPC speaking(NPCs channel)
	SPEAK_PRIVATE_NP		= 0x05, //NPC-to-Player speaking
	SPEAK_PRIVATE			= 0x06, //Players speaking privately to players
	SPEAK_CHANNEL_Y     	= 0x07,	//Yellow message in chat
	SPEAK_CHANNEL_W         = 0x08, //White message in chat
	SPEAK_RVR_CHANNEL		= 0x09, //Reporting rule violation
	SPEAK_RVR_ANSWER		= 0x0A, //Answering report
	SPEAK_RVR_CONTINUE		= 0x0B, //Answering the answer of the report
	SPEAK_BROADCAST     	= 0x0C,	//Broadcast a message - #b
	SPEAK_CHANNEL_R1    	= 0x0D,	//Talk red on chat - #c
	SPEAK_PRIVATE_RED   	= 0x0E,	//Red private - @name@ text
	SPEAK_CHANNEL_O     	= 0x0F	//Talk orange on text
};

enum MessageClasses {
	MSG_STATUS_CONSOLE_RED		= 0x11, //Red message in the console
	MSG_STATUS_CONSOLE_ORANGE2	= 0x13, //Orange message in the console
	MSG_STATUS_CONSOLE_ORANGE	= 0x14, //Orange message in the console
	MSG_STATUS_WARNING			= 0x15, //Red message in game window and in the console
	MSG_EVENT_ADVANCE			= 0x16, //White message in game window and in the console
	MSG_EVENT_DEFAULT			= 0x17, //White message at the bottom of the game window and in the console
	MSG_STATUS_DEFAULT			= 0x18, //White message at the bottom of the game window and in the console
	MSG_INFO_DESCR				= 0x19, //Green message in game window and in the console
	MSG_STATUS_SMALL			= 0x1A, //White message at the bottom of the game window"
	MSG_STATUS_CONSOLE_BLUE		= 0x1B //Blue message in the console
};

enum DIALOGUE_TYPES {
	DIALOGUE_QUESTION	= 0x00,
	DIALOGEU_NPC		= 0x01,
	DIALOGUE_CLOSE		= 0x02,
	DIALOGUE_OPTIONS	= 0x03,
	DIALOGUE_NPCTEXT	= 0x04
};

enum Colors
{
	COLOR_WHITE = 0,
	COLOR_YELLOW = 1,
	COLOR_GRAY = 2,
	COLOR_RED = 3,
	COLOR_GREEN = 4,
	COLOR_ORANGE = 5,
	COLOR_PURPLE = 6,
};

enum PlayerFlags {
	//Add the flag's numbers to get the groupFlags number you need
	PlayerFlag_CannotBeAttacked,
	PlayerFlag_CanAlwaysLogin,
	PlayerFlag_CanBroadcast,
	PlayerFlag_AdminRepel,

	//add new flags here
	PlayerFlag_LastFlag
};

enum SoundType_t {
	SOUND_PLAY = 0,
	SOUND_FADEIN = 1,
	SOUND_LIST = 2
};
#endif