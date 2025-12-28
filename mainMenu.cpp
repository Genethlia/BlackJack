#include "mainMenu.h"

MainMenu::MainMenu(){

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
	
}
