#include "button.h"
Button::Button(int y, string Text){
	x = 710;
	this->y = y;
	width = 280;
	height = 100;
	int j = Text.size();
	for (int i = 0; i < j; i++) {
		type[i] = Text[i];
	}
	firstLetter = type[0];
	secondLetter = type[1];
}

void Button::Draw(){
	DrawRectangle(x, y, width, height, BLACK);
	switch (firstLetter) {
	case 'S':
		if (secondLetter=='T') {
			DrawText(type, x + 50, y + 30, 50, WHITE);
		}
		else {
			DrawText(type, x + 65, y + 30, 50, WHITE);
		}
		break;
	case 'D':
		DrawText(type, x + 40, y + 30, 50, WHITE);
		break;
	case 'H':
		DrawText(type, x + 95, y + 30, 50, WHITE);
		break;
	default:
		break;
	}
	
}

bool Button::IsButtonPressed(){
	Vector2 position = GetMousePosition();
	bool IsMousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
	bool colision = CheckCollisionPointRec(position, Rectangle( x,y,width,height ));
	if (colision && IsMousePressed) {
		return true;
	}
	return false;
}


