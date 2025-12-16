#include "button.h"

int Button::findOffsetX(char type[10]){

	int offset = 0;

	switch (type[0]) {
	case 'S':
		if (type[1] == 'T') {
			offset = 50;
		}
		else {
			offset = 65;
		}
		break;
	case 'D':
		offset = 40;
		break;
	case 'H':
		offset = 95;
		break;
	default:
		break;
	}
	return offset;
}

Button::Button(int y, string Text){
	x = 910;
	this->y = y;
	width = 280;
	height = 100;
	int j = Text.size();
	for (int i = 0; i < j; i++) {
		type[i] = Text[i];
	}
	offsetX = findOffsetX(type);
}

void Button::Draw(){
	Color color;
		if (collision()) {
		color = tableGreen;
	}
	else{
		color = darkGreen;
	}
	DrawRectangle(x, y, width, height, color);
	DrawText(type, x + offsetX, y + 30, 50, WHITE);
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


