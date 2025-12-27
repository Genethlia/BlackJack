#pragma once
#include <string>
#include <raylib.h>
#include "color.h"
using namespace std;

class Button {
protected:
    int x, width, offsetX;
    Color color[2] = {darkGreen,tableGreen};;
private:
    int height;
    string Text; // temporary string to hold the button text
    string type; // array to hold the text letters
    int findOffsetX(string type); // function to find the offset for text positioning
public:
    int y;
    Button(int y, string Text);
    virtual void Draw(); // draw the button
    bool IsButtonPressed(); // check if button is pressed
    virtual bool collision(); // check if mouse is over the button
};

class BetButton:public Button {
protected:
    string Text;
    int radius;
    Color color;
    pair<Color,Color> hoverColor();
public:
    int betAmount;
	BetButton(int x,int betAmount);
	string GetBetAmount();//get the bet amount of the button
	void Draw() override;
	bool collision() override;
};

class UndoConfirmButton :public Button {
public:
    UndoConfirmButton(float y,string Text);
private:
    float y;
};

class AllInButton :public BetButton {
public:
    AllInButton();
};

class MainMenuButton :public Button {
public:
	MainMenuButton(int y, string Text);
private:
    int y;
};