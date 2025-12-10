#include "game.h"

Game::Game(){
	deck = Deck();
	LastUpdateTime = 0;
	cardsDrown = 0;
	Drown = false;
	money = 1000;
}

void Game::Draw(){
	DrawRectangle(705, 0, 295, 900, WHITE);
	DrawRectangle(700, 0, 5, 900, BLACK);
	DrawRectangleLinesEx({ 0,0,1000,900 }, 5, BLACK);
	hit.Draw();
	stand.Draw();
	DrawCards(playerCards);
	DrawCards(cpuCards);
}

void Game::Update(double TimePassed){
	int CurrentTime = GetTime();
	if (CurrentTime - LastUpdateTime > TimePassed) {
		valRank xard = deck.DrawCard();
		if (cardsDrown == 0) {
			Card card(50, 650, xard);
			playerCards.push_back(card);
		}
		else if(cardsDrown==1){
			Card card(200, 650, xard);
			playerCards.push_back(card);
		}
		else if (cardsDrown == 2) {
			Card card(50, 50, xard);
			cpuCards.push_back(card);
		}
		else if (cardsDrown == 3) {
			Card card(200, 50, xard);
			cpuCards.push_back(card);
			Drown = true;
		}
		cardsDrown++;
		LastUpdateTime = GetTime();
	}
}

void Game::DrawCards(vector<Card> cards){
	for (int i = 0; i < static_cast<int>(cards.size()); ++i) {
		cards[i].Draw();
	}
}
