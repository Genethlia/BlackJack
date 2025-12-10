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
	Images ranks;
	Images* ranksObj; // <-- Add pointer to Images for ranksObj
private:
	float x, y;
	float width, height;
	string cardnum(valRank card);
	int GetColorOfRank(valRank card);
	Color color[2];
	int offset;
};