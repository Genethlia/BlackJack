#include "Card.h"

Card::Card(float x, float y, valRank card,Images* suitTextures,Font* font)
  :suitTextures(suitTextures),font(font) {
	this->x = x;
	this->y = y;

	width = 120;
	height = 200;
	this->card = card;

	color[0] = { 0,0,0,255 };
	color[1] = { 255,0,0,255 };


	smalloffset = 0;
	switch (card.rank) {
	case 1:
		bigoffset = 35;
		break;
	case 2:
		bigoffset = 27;
		smalloffset = 4;
		break;
	case 3:
		bigoffset = 40;
		break;
	default:
		bigoffset = 30;
		break;
	}

}


void Card::Draw(){
	int PointerOfcolor = GetColorOfRank(card);
	DrawRectangle(x, y, width, height,WHITE);
	DrawTextEx(*font, cardnum(card).c_str(), { x + 5, y + 5 }, 30, 2, color[PointerOfcolor]);
	DrawTextPro(*font, cardnum(card).c_str(), { x+width-5, y+height-5 }, { 0,0 }, 180, 30, 2,color[PointerOfcolor]);
	// ranksObj textures are valid only if created for suit ranks
	if (suitTextures->filiTexture.id != 0) {
		DrawTexture(suitTextures->filiTexture, x+smalloffset, y + 30, WHITE);
		DrawTextureEx(suitTextures->filiTexture, { width + x-smalloffset, height + y-30 },180,1, WHITE);
	}
	if (suitTextures->bigfiliTexture.id != 0) {
		DrawTexture(suitTextures->bigfiliTexture, x+width/2-bigoffset, y+height/2-40, WHITE);
	}
	
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
