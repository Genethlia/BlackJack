#include "mainMenu.h"

MainMenu::MainMenu(){
	placeholder = -1;
}

void MainMenu::Draw(){
	DrawText("BLACKJACK", 50, 100, 100, WHITE);
	playButton.Draw();
	resumeButton.Draw();
	settingsButton.Draw();
	exitButton.Draw();
	statsButton.Draw();
}

void MainMenu::Update(){
	if (playButton.IsButtonPressed()) {
		placeholder = playButton.placeholder;
	}
	if (resumeButton.IsButtonPressed()) {
		placeholder = resumeButton.placeholder;
	}
	if (settingsButton.IsButtonPressed()) {
		placeholder = settingsButton.placeholder;
	}
	if (exitButton.IsButtonPressed()) {
		placeholder = exitButton.placeholder;
	}
	if (statsButton.IsButtonPressed()) {
		placeholder = statsButton.placeholder;
	}
}

void MainMenu::SetTransform(float* scale, float* offsetX, float* offsetY){
	playButton.SetTransform(scale, offsetX, offsetY);
	resumeButton.SetTransform(scale, offsetX, offsetY);
	settingsButton.SetTransform(scale, offsetX, offsetY);
	exitButton.SetTransform(scale, offsetX, offsetY);
	statsButton.SetTransform(scale, offsetX, offsetY);
}
