#include "Audio.h"

Audio::Audio(){
	musicstarted = false;
}

Audio::~Audio(){
	UnloadAll();
}

void Audio::UpdateMusic(bool musicEnabled){
	if (musicEnabled) {
		if (!musicstarted) {
			musicstarted = true;
			PlayMusicStream(backgroundMusic);
		}
		UpdateMusicStream(backgroundMusic);
	}
	else {
		if (musicstarted) {
			musicstarted = false;
			StopMusicStream(backgroundMusic);
		}
	}
}

void Audio::LoadAll(){
	playerBlackjack = LoadSound("Sounds/Player_BlackJack.wav");
	playerLose = LoadSound("Sounds/Player_Lose.wav");
	playerWin = LoadSound("Sounds/Player_Win.wav");
	playerPush = LoadSound("Sounds/Player_Push.wav");
	playerSurrender = LoadSound("Sounds/Player_Surrender.wav");
	dealerBlackjack = LoadSound("Sounds/Dealer_BlackJack.wav");
	cardFlip = LoadSound("Sounds/CardFlip.wav");
	backgroundMusic = LoadMusicStream("Sounds/BackgroundMusic.mp3");
	backgroundMusic.looping = true;
	SetMusicVolume(backgroundMusic, 0.5f);
}

void Audio::UnloadAll(){
	if (playerBlackjack.frameCount > 0) UnloadSound(playerBlackjack);
	if (playerLose.frameCount > 0) UnloadSound(playerLose);
	if (playerWin.frameCount > 0)UnloadSound(playerWin);
	if (playerPush.frameCount > 0) UnloadSound(playerPush);
	if (playerSurrender.frameCount > 0) UnloadSound(playerSurrender);
	if (dealerBlackjack.frameCount > 0) UnloadSound(dealerBlackjack);
	if (cardFlip.frameCount > 0) UnloadSound(cardFlip);

	UnloadMusicStream(backgroundMusic);
}
