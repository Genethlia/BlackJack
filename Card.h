#pragma once
#include <raylib.h>
#include "images.h"
#include "deck.h"
#include <string>
using namespace std;

class Card {
public:
	Card(float x, float y, valRank card);
	~Card();
	void Draw();
	Font font;
	valRank card;
	Images ranks;// <-- Keep original Images object
	Images* ranksObj; // <-- Add pointer to Images for ranksObj
private:
	float x, y;
	float width, height;
	string cardnum(valRank card);//Returns the string representation of the card value
	int GetColorOfRank(valRank card);//Returns pointer to color array based on card suit
	Color color[2];//0 for black,1 for red
	int offset;//Offset for centering big rank image
};