#ifndef __GAMETEXT_H_
#define __GAMETEXT_H_

#include <string>
#include <vector>
#include <boost/unordered_map.hpp>

#include "enums.h"

enum GAMETEXT_t
{
	GT_WILDENCOUNTER = 1,
	GT_POKEMONCHOOSEMOVE = 2,
	GT_POKEMONATTACK = 3,
	GT_POKEMONATTACKON = 4,
	GT_BATTLEWAITING = 5,
	GT_BATTLERUN = 6,
	GT_BATTLERUNFAIL = 7,
	GT_BATTLERUNSUCCES = 8,
	GT_BATTLECHANGEPOKEMON = 9,
	GT_BATTLEATTACKMISSED = 10,
	GT_BATTLEATTACKCRITICAL = 11,
	GT_BATTLEPOKEMONFAINTED = 12,
	GT_BATTLEPOKEMONEXP = 13,
	GT_BATTLEPOKEMONLVLUP = 14,
	GT_BATTLEPOKEMONCHANGED = 15,
	GT_BATTLENOTEFFECTIVE = 16,
	GT_BATTLENOTVERYEFFECTIVE = 17,
	GT_BATTLEVERYEFFECTIVE = 18,
	GT_BATTLESUPEREFFECTIVE = 19,
	GT_BATTLESTATFELL = 20,
	GT_BATTLESTATHARSHLYFELL = 21,
	GT_BATTLESTATROSE = 22,
	GT_BATTLESTATSHARPLYROSE = 23,
	GT_BATTLESTATMOD_ON = 24,
	GT_BATTLESTATMOD_OFF = 25,
	GT_BATTLEPOKEMONFAINTED_OWN = 26,
	GT_POKEMONLEARNATTACK_SLOT = 27,
	GT_POKEMONLEARNATTACK = 28,
	GT_POKEMONFLINCHED = 29,
	GT_BATTLEATTACKCHARGING = 30,
	GT_BATTLEATTACKMULTIHIT = 31,
	GT_BATTLEPLAYEROUT = 32,
	GT_BATTLEPLAYERFORFEIT = 33,
	GT_POKEMONHURTBY = 34,
	GT_BATTLEATTACKDISABLED = 35,
	GT_BATTLECHOOSEITEM = 36,
	GT_BATTLEUSEITEMONPOKEMON = 37,
	GT_BATTLEITEMONPOKEMON = 38,
	GT_BATTLEITEMEFFECT_RESTORE = 39,
	GT_BATTLETHROWPOKEBALL = 40,
	GT_BATTLEPOKEMON_CAUGHT = 41,
	GT_BATTLEPOKEMON_ESCAPED = 42,
	GT_BATTLEPLAYERTURN = 43,
	GT_PLAYERENCOUNTER = 44,
	GT_BATTLECHANGEPOKEMON_QUESTION = 45,
	GT_BATTLE_WINGOLDZ = 46,

	GT_BATTLE_CONFUSED = 666,
	GT_BATTLE_CONFUSED_HURT = 667,
	GT_BATTLE_BUTNOTHINGHAPPENED = 668
};

struct TextPiece
{
	Colors color;
	int var;
	std::string text;

	TextPiece( Colors _color, int _var, std::string _text ) : color(_color), var(_var), text(_text) {}
};

typedef std::vector<TextPiece> TextPieceList;

class GameText
{
public:
	GameText() {};
	~GameText() {};

	bool loadText(std::string xmlfile);

	TextPieceList getText(int textId);

private:
	void addText(int textId, TextPieceList pieces);

	typedef boost::unordered_map<int, TextPieceList> TextList;	
	TextList textArray; 
};

#endif