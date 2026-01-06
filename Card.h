#pragma once
#include "Source.h"
#include "Images.h"
#include "deck.h"

using namespace std;

class Card {
public:
	Card(float x, float y, valRank card,Images* suitTextures, Font* font,Images* gameimages,vector<valRank>* cards,Audio* audio,bool* soundenabled);
	~Card() = default;
	void Draw();
	void Update();
	bool IsMoving();
	void SetFaceDown(bool v);
	void GoImmediatelyToTarget();
	void StartFlip();
	Vector2 pos;
	valRank card;
	Vector2 target;
	bool secret;
private:
	bool moving;
	bool facedown;
	bool firstsecret;
	bool flipping;
	float flipProgress;
	float width, height;
	string cardnum(valRank card);//Returns the string representation of the card value
	int GetColorOfRank(valRank card);//Returns pointer to color array based on card suit
	Color color[2];//0 for black,1 for red
	int bigoffset;//Offset for centering big rank image
	int smalloffset;//Offset for centering small rank image

	Images* suitTextures;
	Images* gameimages;
	Font* font;
	vector<valRank>* cards;
	Audio* gameaudio;
	bool* soundenabled;
};