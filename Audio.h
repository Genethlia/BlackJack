#pragma once
#include <raylib.h>

class Audio {
public:
	Audio();
	~Audio();
	void LoadAll();
	void UpdateMusic(bool musicEnabled);

	Sound playerWin;
	Sound playerLose;
	Sound playerPush;
	Sound playerBlackjack;
	Sound playerSurrender;
	Sound dealerBlackjack;
	Sound cardFlip;

	Music backgroundMusic;
private:
	void UnloadAll();
	bool musicstarted;
};
