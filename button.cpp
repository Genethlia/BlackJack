#include "button.h"

int Button::findOffsetX(string type){

	int offset = 0;

	switch (type[0]) {
	case 'S':
		if (type[1] == 'T') {
			offset = 50;
		}
		else{
			offset = 65;
		}
		break;
	case 'D':
		offset = 40;
		break;
	case 'H':
		offset = 95;
		break;
	case 'U':
		offset = 220;
		break;
	case 'C':
		offset = 120;
		break;
	case 'G':
		offset = 35;
		break;
	default:
		break;
	}
	return offset;
}

Button::Button(int y, string Text) {
	x = 910;
	this->y = y;
	width = 280;
	height = 100;
	type = Text;
	if (!Text.empty()) {
		offsetX = findOffsetX(type);
	}
}

void Button::Draw(){
	int temp = collision();
	DrawRectangle(x, y, width, height, color[temp]);
	DrawText(type.c_str(), x + offsetX, y + 30, 50, WHITE);
}

bool Button::IsButtonPressed(){
	bool IsMousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
	if (collision() && IsMousePressed) {
		return true;
	}
	return false;
}

bool Button::collision(){
	Vector2 position = GetMousePosition();
	return CheckCollisionPointRec(position, Rectangle(x, y, width, height));
}


pair<Color, Color> BetButton::hoverColor(){
	switch (betAmount) {
	case 0:
		return pair(red, lightGold);
	case 10:
		return pair(yellow, lightYellow);
	case 50:
		return pair(blue, lightBlue);
	case 100:
		return pair(red, lightRed);
	case 200:
		return pair(purple, lightPurple);
	default:
		return pair(WHITE, WHITE);
	}
}

BetButton::BetButton(int x, int betAmount) : Button(x, to_string(betAmount)) {
	this->x = x;
	this->y = GetScreenHeight() / 2-50;
	this->betAmount = betAmount;
	radius = 75;
	color = WHITE;
	Text = GetBetAmount();
}

string BetButton::GetBetAmount()
{
	return to_string(betAmount);
}

void BetButton::Draw(){
	int offsetX = 15;
	int offsetY = 10;
	int size = 30;
	if (collision()) {
		color = hoverColor().first;
	}
	else {
		color = hoverColor().second;
	}
	DrawCircle(x, y, radius, color);
	DrawCircle(x, y, radius*3/4, WHITE);
	DrawCircle(x, y, radius/2, color);
	if (GetBetAmount().size() == 3) offsetX = 20;
	if (GetBetAmount() == "200") offsetX = 25;
	if (GetBetAmount() == "10") offsetX = 12;
	if (GetBetAmount() == "0") offsetX = 38,size=25;
	DrawText(Text.c_str(), x - offsetX, y - offsetY, size, WHITE);
}

bool BetButton::collision(){
	if (CheckCollisionPointCircle(GetMousePosition(), Vector2(x, y),radius)) {
		return true;
	}
	return false;
}

UndoConfirmButton::UndoConfirmButton(float y,string Text) : Button(y, Text) {
	this->y = y;
	x = (GetScreenWidth() - 890) / 2;
	width = 600;
}

AllInButton::AllInButton():BetButton((GetScreenWidth()-300)/2,0){
	radius = 100;
	Text = "ALL IN";
	y = GetScreenHeight() / 2 -300;
}

MainMenuButton::MainMenuButton(int y, string Text):Button(y,Text){
	this->y = y;
	x = 20;
	offsetX = 5;
	width = MeasureText(Text.c_str(), 50) + 40;
	color[0] = lightGreen;
	color[1] = darkGreen;
	placeholder = y / 100 - 3;
}

void MainMenuButton::Draw(){
	bool isHover = collision();

	float targetOffset = isHover ? 25.0f : 0.0f;//If hover, slide to 25, else slide back to 0(fancy way to write an if statement)
	slideOffset = Lerp(slideOffset, targetOffset, 0.2f);

	DrawRectangle(x, y, width, height, color[isHover]);
	DrawText(type.c_str(), x + offsetX + slideOffset, y + 30, 50, WHITE);
	//DrawRectangleLines(x, y, width, height, RED);

	if (isHover) {
		float centerX = x + 15;
		float centerY = y + height / 2+4;
		float radius = 10;
		DrawPoly({ centerX, centerY }, 3, radius, 0, WHITE);
	}
}

HomeButton::HomeButton():Button(5,"") {
	x = 895 - 50;
	width = 50;
	height = 50;
	offsetX = 5;
	color[0] = transparent;
	color[1] = WHITE;
}

OvalButton::OvalButton(int y, bool* state):Button(y,"") {
	x = 950;
	this->y = y;
	ovalWidth = 150;
	ovalHeight = 60;
	knobRadius = ovalHeight / 2 - 5;
	this->state = state;
	slideOffset = 0.0f;
	color[1] = darkGreen;
	color[0] = red;
}

void OvalButton::Draw(){
	bool isOn = state ? *state : false;

	Color temp = color[isOn];
	DrawRectangleRounded({ (float)x,(float)y,(float)ovalWidth,(float)ovalHeight }, 1.0f, 16, temp);

	float targetoffset = isOn ? ovalWidth - ovalHeight : 0;
	slideOffset = Lerp(slideOffset, targetoffset, 0.2f);

	float knobX=x+ovalHeight/2+slideOffset;
	float knobY=y+ovalHeight/2;
	bool tempb = collision();
	DrawCircle(knobX, knobY, knobRadius - 3, tempb ? LIGHTGRAY : GRAY);
}

void OvalButton::Update(){
	if (IsButtonPressed()) {
		if (state) {
			*state = !*state;
		}
	}
}

bool OvalButton::collision(){
	Vector2 mouseposition = GetMousePosition();
	Rectangle rec = { (float)x,(float)y,(float)width,(float)height };

	return CheckCollisionPointRec(mouseposition, rec);
}
