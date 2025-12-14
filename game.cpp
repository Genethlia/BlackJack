#include "game.h"

Game::Game(){
	deck = Deck();
	LastUpdateTime = 0;
	cardsDrown = 0;
	Drown = false;
	money = 1000;
}

void Game::Draw() {
	DrawBackground();
	DrawButtons();
	DrawCards();
	DrawScore();
}

void Game::DrawStartingCards(double TimePassed){
	int CurrentTime = GetTime();
	if (CurrentTime - LastUpdateTime > TimePassed) {
		valRank xard = deck.DrawCard();
		//valRank xard = { 1,1 };//testing split
		if (cardsDrown == 0) {
			Card card(50, 650, xard);
			playerCards.push_back(card);
			playerHand.push_back(xard);
		}
		else if(cardsDrown==1){
			Card card(50, 50, xard);
			cpuCards.push_back(card);
			cpuHand.push_back(xard);
		}
		else if (cardsDrown == 2) {
			Card card(210, 650, xard);
			playerCards.push_back(card);
			playerHand.push_back(xard);
		}
		else if (cardsDrown == 3) {
			Card card(210, 50, xard);
			cpuCards.push_back(card);
			cpuHand.push_back(xard);
			Drown = true;
		}
		cardsDrown++;
		LastUpdateTime = GetTime();
	}
}

void Game::DrawCards(){
	for (int i = 0; i < static_cast<int>(playerCards.size()); ++i) {
		playerCards[i].Draw();
	}
	for (int i = 0; i < static_cast<int>(cpuCards.size()); ++i) {
		cpuCards[i].Draw();
	}
}

void Game::DrawScore(){
	int playerScore = GetScore(playerHand);
	int cpuScore = GetScore(cpuHand);
	string playerScoreText = "Player Score: " + to_string(playerScore);
	string cpuScoreText = "Dealer Score: " + to_string(cpuScore);
	DrawText(playerScoreText.c_str(), 720, 830, 32, WHITE);
	DrawText(cpuScoreText.c_str(), 720, 750, 32, WHITE);
}

void Game::DrawButtons(){
	if (Drown) {
		hit.Draw();
		stand.Draw();
		//Double.Draw();
		if (playerHand[0].value == playerHand[1].value) {
			Split.Draw();
		}
	}
}

void Game::DrawBackground(){
	DrawTexture(matImage.matTexture, 0, 0, WHITE);
	DrawRectangle(705, 0, 295, 900, lightGreen);
	DrawRectangle(700, 0, 5, 900, BLACK);
	DrawRectangleLinesEx({ 0,0,1000,900 }, 5, BLACK);
}

int Game::GetScore(vector<valRank> hand){
	int aces = 0;
	int score = 0;
	for (int i = 0; i < hand.size(); i++) {
		if (hand[i].value == 1) {
			aces++;
			score += 1;
		}
		else if (hand[i].value > 10) {
			score += 10;
		}
		else {
			score += hand[i].value;
		}
	}
	while (aces > 0&&score+10<=21) {
		score += 10;
		aces--;
	}
	return score;
}
