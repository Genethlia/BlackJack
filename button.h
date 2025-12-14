#pragma once
#include <string>
#include <raylib.h>
#include "color.h"
using namespace std;

class Button {
private:
	int x, y;
	int width, height;
	string Text;//temporary string to hold the button text
	char type[10];//array to hold the text letters
	int offsetX;
	int findOffsetX(char type[10]);//function to find the offset for text positioning
public:
	Button(int y, string Text);
	void Draw();//draw the button
	bool IsButtonPressed();//check if button is pressed
	bool collision();//check if mouse is over the button
};
