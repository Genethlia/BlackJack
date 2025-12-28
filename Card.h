#pragma once
#include "Source.h"
#include "Images.h"
#include "deck.h"

using namespace std;

class Card {
public:
	Card(float x, float y, valRank card,Images* suitTextures, Font* font);
	~Card() = default;
	void Draw();
	valRank card;
private:
	float x, y;
	float width, height;
	string cardnum(valRank card);//Returns the string representation of the card value
	int GetColorOfRank(valRank card);//Returns pointer to color array based on card suit
	Color color[2];//0 for black,1 for red
	int bigoffset;//Offset for centering big rank image
	int smalloffset;//Offset for centering small rank image

	Images* suitTextures;
	Font* font;
};