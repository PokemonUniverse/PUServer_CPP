#ifndef __ENUMS_H_
#define __ENUMS_H_

enum ReturnValue {
	RET_NOERROR = 1,
	RET_NOTPOSSIBLE = 2,
	RET_PLAYERISTELEPORTED = 3,
};

enum colors_t
{
	COLOR_WHITE = 0,
	COLOR_YELLOW = 1,
	COLOR_GRAY = 2,
	COLOR_RED = 3,
	COLOR_GREEN = 4,
	COLOR_ORANGE = 5,
	COLOR_PURPLE = 6,
};

enum tileblocking_t
{
	TILEBLOCK_BLOCK = 1,
	TILEBLOCK_WALK = 2,
	TILEBLOCK_SURF = 3,
	TILEBLOCK_TOP = 4,
	TILEBLOCK_BOTTOM = 5,
	TILEBLOCK_RIGHT = 6,
	TILEBLOCK_LEFT = 7,
	TILEBLOCK_TOPRIGHT = 8,
	TILEBLOCK_BOTTOMRIGHT = 9,
	TILEBLOCK_BOTTOMLEFT = 10,
	TILEBLOCK_TOPLEFT = 11
};

enum Direction {
	DIR_NULL = 0,
	DIR_SOUTH = 1,
	DIR_WEST = 2,
	DIR_NORTH = 3,
	DIR_EAST = 4
};

enum creaturemovement_t
{
	MOVEMENT_WALK = TILEBLOCK_WALK,
	MOVEMENT_SURF = TILEBLOCK_SURF,
};

enum playersex_t {
	PLAYERSEX_FEMALE = 0,
	PLAYERSEX_MALE = 1,
	PLAYERSEX_OLDMALE = 2
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
	SPEAK_RVR_CHANNEL		= 0x09, //Reporting rule violation - Ctrl+R
	SPEAK_RVR_ANSWER		= 0x0A, //Answering report
	SPEAK_RVR_CONTINUE		= 0x0B, //Answering the answer of the report
	SPEAK_BROADCAST     	= 0x0C,	//Broadcast a message - #b
	SPEAK_CHANNEL_R1    	= 0x0D,	//Talk red on chat - #c
	SPEAK_PRIVATE_RED   	= 0x0E,	//Red private - @name@ text
	SPEAK_CHANNEL_O     	= 0x0F,	//Talk orange on text
	//SPEAK_                = 0x10, //?
	SPEAK_CHANNEL_R2    	= 0x11,	//Talk red anonymously on chat - #d
	//SPEAK_                = 0x12, //?
	SPEAK_MONSTER_SAY   	= 0x13,	//Talk orange
	SPEAK_MONSTER_YELL  	= 0x14,	//Yell orange
};

enum Statistic_t
{
	S_NONE = -1,
	S_HP = 0,
	S_ATTACK = 1,
	S_DEFENCE = 2,
	S_SPEED = 3,
	S_SPATTACK = 4, 
	S_SPDEFENCE = 5,
	S_ACCURACY = 6,
	S_EVASION = 7
};

struct Outfit_t{
	Outfit_t(){
		lookHead   = 0;
		lookBody   = 0;
		lookLegs   = 0;
		lookFeet   = 0;
		lookType   = 0;
		lookTypeEx = 0;
		lookAddons = 0;
	}

	uint32_t lookType;
	uint32_t lookTypeEx;
	uint32_t lookHead;
	uint32_t lookBody;
	uint32_t lookLegs;
	uint32_t lookFeet;
	uint32_t lookAddons;
};

enum CombatType_t {
	COMBAT_NONE             = 0,
	COMBAT_FIRST            = COMBAT_NONE,
	COMBAT_PHYSICALDAMAGE	= 1,
	COMBAT_ENERGYDAMAGE		= 2,
	COMBAT_EARTHDAMAGE		= 4,
	COMBAT_FIREDAMAGE		= 8,
	COMBAT_UNDEFINEDDAMAGE	= 16,
	COMBAT_LIFEDRAIN		= 32,
	COMBAT_MANADRAIN		= 64,
	COMBAT_HEALING			= 128,
	COMBAT_DROWNDAMAGE      = 256,
	COMBAT_ICEDAMAGE        = 512,
	COMBAT_HOLYDAMAGE       = 1024,
	COMBAT_DEATHDAMAGE      = 2048,
	COMBAT_LAST             = COMBAT_DEATHDAMAGE
};

const int COMBAT_COUNT = 13;

enum CombatParam_t{
	COMBATPARAM_COMBATTYPE				= 1,
	COMBATPARAM_EFFECT					= 2,
	COMBATPARAM_DISTANCEEFFECT			= 3,
	COMBATPARAM_BLOCKEDBYSHIELD			= 4,
	COMBATPARAM_BLOCKEDBYARMOR			= 5,
	COMBATPARAM_TARGETCASTERORTOPMOST	= 6,
	COMBATPARAM_CREATEITEM				= 7,
	COMBATPARAM_AGGRESSIVE				= 8,
	COMBATPARAM_DISPEL					= 9,
	COMBATPARAM_USECHARGES				= 10,
	COMBATPARAM_HITEFFECT				= 11,
	COMBATPARAM_HITTEXTCOLOR				= 12,
};

enum skills_t {
	SKILL_FIRST = 0,
	SKILL_FIST = SKILL_FIRST,
	SKILL_CLUB = 1,
	SKILL_SWORD = 2,
	SKILL_AXE = 3,
	SKILL_DIST = 4,
	SKILL_SHIELD = 5,
	SKILL_FISH = 6,
	SKILL_LAST = SKILL_FISH
};

enum stats_t {
	STAT_FIRST = 0,
	STAT_MAXHITPOINTS = STAT_FIRST,
	STAT_MAXMANAPOINTS,
	STAT_SOULPOINTS,
	STAT_MAGICPOINTS,
	STAT_LAST = STAT_MAGICPOINTS
};

enum PokemonGenders_t
{
	GENDER_MALE = 1,
	GENDER_FEMALE = 2,
	GENDER_BOTH = GENDER_MALE | GENDER_FEMALE,
	GENDER_NONE = 0
};

#endif