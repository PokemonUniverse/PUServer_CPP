#ifndef __GAMETEXT_H_
#define __GAMETEXT_H_

#include <boost/unordered_map.hpp>
#include <vector>

#include "enums.h"

typedef std::vector<std::string> TextVarHolder;

class PU_Textpart
{
public:
	PU_Textpart(std::string _text, colors_t _color, bool _isVariable = false)
		: text(_text),
		color(_color),
		var(_isVariable) {};

	~PU_Textpart() {};

	std::string getText() { return text; };
	void setText(std::string _text) { text = _text; };
	colors_t getColor() { return color; };
	bool isVariable() { return var; };

private:
	std::string text;
	colors_t color;
	bool var;
};

class PU_Text
{
public:
	PU_Text() {};
	~PU_Text() {};

	void add(std::string _text, long _color);
	PU_Textpart getPart(int _index);

	void addToLast(std::string _text);

	int getSize();
	int getVariables();

	std::string parseText();
	std::string parseText(TextVarHolder vars);

private:
	std::vector<PU_Textpart> parts;
};

class GameText
{
public:
	GameText();
	~GameText();

	void loadFromXML(std::string file);

	PU_Text getText(uint32_t _index);

private:
	typedef boost::unordered_map<uint32_t, PU_Text> GameTextList;
	GameTextList gameTextList;
};

#endif