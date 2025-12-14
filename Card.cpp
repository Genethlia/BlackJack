#include "Card.h"

Card::Card(float x, float y, valRank card)
  : ranks(card.rank) {
	this->x = x;
	this->y = y;

	width = 120;
	height = 200;
	this->card = card;

	color[0] = { 0,0,0,255 };
	color[1] = { 255,0,0,255 };

	font = LoadFontEx("Fonts/monogram", 64, 0, 0);
	ranksObj = new Images(card.rank);

	switch (card.rank) {
	case 1:
		offset = 35;
		break;
	case 2:
		offset = 27;
		break;
	case 3:
		offset = 40;
		break;
	default:
		offset = 30;
		break;
	}

}

Card::~Card(){
	UnloadFont(font);
}

void Card::Draw(){
	int PointerOfcolor = GetColorOfRank(card);
	DrawRectangle(x, y, width, height,WHITE);
	DrawTextEx(font, cardnum(card).c_str(), { x + 5, y + 5 }, 30, 2, color[PointerOfcolor]);
	DrawTextPro(font, cardnum(card).c_str(), { x+width-5, y+height-5 }, { 0,0 }, 180, 30, 2,color[PointerOfcolor]);
	DrawTexture(ranksObj->filiTexture, x, y + 30, WHITE);
	DrawTextureEx(ranksObj->filiTexture, { width + x, height + y-30 },180,1, WHITE);
	DrawTexture(ranksObj->bigfiliTexture, x+width/2-offset, y+height/2-40, WHITE);
	
}

string Card::cardnum(valRank card){
	if (card.value == 1) {
		return "A";
	}
	if (card.value == 11) {
		return "J";
	}
	if (card.value == 12) {
		return "Q";
	}
	if (card.value == 13) {
		return "K";
	}

	return to_string(card.value);
}

int  Card::GetColorOfRank(valRank card) {
	if (card.rank == 1 || card.rank == 3) {
		return 0;
	}
	return 1;
}
