#pragma once
#include "Source.h"
#include "button.h"
using namespace std;
class MainMenu {
public:
	MainMenu();
	~MainMenu() = default;
	virtual void Draw();
	virtual void Update();
	virtual void SetTransform(float* scale, float* offsetX, float* offsetY);
	int placeholder;
private:
	MainMenuButton playButton = MainMenuButton(300, "PLAY NEW GAME");
	MainMenuButton settingsButton = MainMenuButton(500, "SETTINGS");
	MainMenuButton exitButton = MainMenuButton(700, "EXIT");
	MainMenuButton statsButton = MainMenuButton(600, "STATS");
	MainMenuButton resumeButton = MainMenuButton(400, "RESUME  GAME");
	double lastTime = 0.0;//For debug output timing
};

class Gamemode :public MainMenu {
public:
	void Draw() override;
	void Update() override;
	void SetTransform(float* scale, float* offsetX, float* offsetY) override;
private:
	MainMenuButton unlimited = MainMenuButton(300, "UNLIMITED");
	MainMenuButton bestof20 = MainMenuButton(500, "BEST OF 20");
	MainMenuButton bestof50 = MainMenuButton(700, "BEST OF 50");
};