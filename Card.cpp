#include "Card.h"

Card::Card(float x, float y, valRank card,Images* suitTextures,Font* font,Images* gameimages)
  :suitTextures(suitTextures),font(font),gameimages(gameimages) {
	target = { x,y };
	pos = { 800,300 };

	width = 120;
	height = 200;
	this->card = card;

	moving = true;
	facedown = true;
	secret = false;

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
	if ((secret||facedown)&&gameimages->hiddenCardTexture.id!=0) {
		DrawTexture(gameimages->hiddenCardTexture, pos.x, pos.y, WHITE);
		return;
	}

	int PointerOfcolor = GetColorOfRank(card);
	DrawRectangle(pos.x, pos.y, width, height,WHITE);
	DrawTextEx(*font, cardnum(card).c_str(), { pos.x + 5, pos.y + 5 }, 30, 2, color[PointerOfcolor]);
	DrawTextPro(*font, cardnum(card).c_str(), { pos.x+width-5, pos.y+height-5 }, { 0,0 }, 180, 30, 2,color[PointerOfcolor]);
	// ranksObj textures are valid only if created for suit ranks
	if (suitTextures->filiTexture.id != 0) {
		DrawTexture(suitTextures->filiTexture, pos.x+smalloffset, pos.y + 30, WHITE);
		DrawTextureEx(suitTextures->filiTexture, { width + pos.x-smalloffset, height + pos.y-30 },180,1, WHITE);
	}
	if (suitTextures->bigfiliTexture.id != 0) {
		DrawTexture(suitTextures->bigfiliTexture, pos.x+width/2-bigoffset, pos.y+height/2-40, WHITE);
	}
	
}

void Card::Update(){
	if (!moving) return;

	Vector2 dir = { target.x - pos.x,target.y - pos.y };

	float dist = sqrt(dir.x * dir.x + dir.y * dir.y);

	float speed = dist * 12 * GetFrameTime();

	if (dist <= 0.5f) {
		pos = target;
		moving = false;
		facedown = false;
		return;
	}
	else {
		dir.x /= dist;
		dir.y /= dist;

		pos.x += dir.x * speed;
		pos.y += dir.y * speed;
	}

}

bool Card::IsMoving(){
	return moving;
}

void Card::SetFaceDown(bool v){
	secret = v;
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
