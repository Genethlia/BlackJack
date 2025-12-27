#pragma once
#include <raylib.h>
#include "button.h"
using namespace std;
class MainMenu {
public:
	MainMenu();
	~MainMenu() = default;
	void Draw();
	void Update();
private:
	MainMenuButton playnewButton = MainMenuButton(400, "PLAY NEW GAME");
	MainMenuButton settingsButton = MainMenuButton(500, "SETTINGS");
	MainMenuButton exitButton = MainMenuButton(700, "EXIT");
	MainMenuButton statsButton = MainMenuButton(600, "STATS");
	MainMenuButton playsavedButton = MainMenuButton(300, "PLAY SAVED GAME");
};