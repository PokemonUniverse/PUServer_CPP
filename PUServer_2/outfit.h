#ifndef INC_OUTFIT_H_
#define INC_OUTFIT_H_

#include "definitions.h"

enum outfitPart_t
{
	OUTFIT_HEAD = 0,
	OUTFIT_NEK = 1,
	OUTFIT_UPPER = 2,
	OUTFIT_LOWER = 3,
	OUTFIT_FEET = 4
};

class Outfit
{
	int outfitData[5];

public:
	Outfit() {};

	Outfit(int _head, int _nek, int _upper, int _lower, int _feet) {
		outfitData[OUTFIT_HEAD] = _head;
		outfitData[OUTFIT_NEK] = _nek;
		outfitData[OUTFIT_UPPER] = _upper;
		outfitData[OUTFIT_LOWER] = _lower;
		outfitData[OUTFIT_FEET] = _feet;
	};
	~Outfit() {};

	void update(Outfit outfit) {
		outfitData[OUTFIT_HEAD] = outfit.getKey(OUTFIT_HEAD);
		outfitData[OUTFIT_NEK] = outfit.getKey(OUTFIT_NEK);
		outfitData[OUTFIT_UPPER] = outfit.getKey(OUTFIT_UPPER);
		outfitData[OUTFIT_LOWER] = outfit.getKey(OUTFIT_LOWER);
		outfitData[OUTFIT_FEET] = outfit.getKey(OUTFIT_FEET);
	};
	
	void setKey(outfitPart_t outfitpart, int key) {
		outfitData[outfitpart] = key;
	};
	int getKey(outfitPart_t outfitpart) {
		return outfitData[outfitpart];
	};

	void updateStyle(outfitPart_t outfitpart, int style) {
		outfitData[outfitpart] = (style << 24) | getColour(outfitpart);
	};
	int getStyle(outfitPart_t outfitpart) {
		int key = outfitData[outfitpart];
		return ((char)(key >> 24));
	};

	void updateColour(outfitPart_t outfitpart, int colour) {
		outfitData[outfitpart] = (getStyle(outfitpart) << 24) | colour;
	};
	int getColour(outfitPart_t outfitpart) {
		int key = outfitData[outfitpart];
		return (key & 0xFFFFFF);
	};
};

#endif