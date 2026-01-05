#pragma once
#include "Source.h"
#include "button.h"
using namespace std;

class Settings {
public:
	Settings();
	void Draw();
	void Update();
	bool ShouldDealerHitOn17;
	bool IsMusicOn;
	bool IsSoundEffectsOn;
	void SaveSettings();
	void LoadSettings();
private:
	OvalButton dealerHitOn17 = OvalButton(50, &ShouldDealerHitOn17, "Dealer hits on soft 17");
	OvalButton musicOn = OvalButton(450, &IsMusicOn, "Music");
	OvalButton soundEffectsOn = OvalButton(850, &IsSoundEffectsOn, "Sound Effects");
};