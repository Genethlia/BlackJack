#pragma once
#include <string>
#include <raylib.h>
#include "color.h"
using namespace std;

class Button {
private:
	int x, y;
	int width, height;
	string Text;
	char type[10];
	char firstLetter,secondLetter;
public:
	Button(int y, string Text);
	void Draw();
	bool IsButtonPressed();
};
