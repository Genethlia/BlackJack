#pragma once
#include "Source.h"
#include "button.h"
using namespace std;
class MainMenu {
public:
	MainMenu();
	~MainMenu() = default;
	void Draw();
	void Update();
	int placeholder;
private:
	MainMenuButton playButton = MainMenuButton(300, "PLAY NEW GAME");
	MainMenuButton settingsButton = MainMenuButton(500, "SETTINGS");
	MainMenuButton exitButton = MainMenuButton(700, "EXIT");
	MainMenuButton statsButton = MainMenuButton(600, "STATS");
	MainMenuButton resumeButton = MainMenuButton(400, "RESUME  GAME");
	double lastTime = 0.0;//For debug output timing
};