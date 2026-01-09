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

void Gamemode::Draw(){
	DrawText("SELECT GAMEMODE", 20, 20, 80, WHITE);
	unlimited.Draw();
	bestof20.Draw();
	bestof50.Draw();
}

void Gamemode::Update(){
	if (unlimited.IsButtonPressed()) {
		placeholder = 0;
	}
	if (bestof20.IsButtonPressed()) {
		placeholder = 1;
	}
	if (bestof50.IsButtonPressed()) {
		placeholder = 2;
	}
}

void Gamemode::SetTransform(float* scale, float* offsetX, float* offsetY){
	unlimited.SetTransform(scale, offsetX, offsetY);
	bestof20.SetTransform(scale, offsetX, offsetY);
	bestof50.SetTransform(scale, offsetX, offsetY);
}
