#include "game.h"

Game::Game(){
	deck = Deck();
	LastUpdateTime = 0;
	dealerLastUpdateTime = 0;
	cardsDealtCount = 0;
	money = 1000;
	cpuHiddenCard = {0, 0}; // Initialize to default value

	for (int i = 0; i < 4; i++) suitImages[i].LoadSuit(i);
	gameImage.LoadMatAndHiddenCard();
	mainFont = LoadFontEx("Fonts/ComSuns.ttf", 64, 0, 0);
}

void Game::Draw() {
	DrawBackground();
	DrawButtons();
	DrawCards();
	DrawScore();
}

void Game::UpdateDealing(double TimePassed){
		if (HasEnoughTimePassed(LastUpdateTime, TimePassed)) {
			valRank xard = deck.DrawCard();
			//valRank xard = { 1,1 };//testing split
			switch(cardsDealtCount){
			case 0:
				playerCards.emplace_back(50,player_Y,xard,&suitImages[xard.rank],&mainFont);
				playerHand.push_back(xard);
				break;
			case 1:
				cpuCards.emplace_back(50, dealer_Y, xard, &suitImages[xard.rank], &mainFont);
				cpuHand.push_back(xard);
				break;
			case 2:
				playerCards.emplace_back(210, player_Y, xard, &suitImages[xard.rank], &mainFont);
				playerHand.push_back(xard);
				break;
			case 3:
				cpuHiddenCard = xard;
				state = GameState::playerTurn;
				return;
			default:
				break;
			}
			cardsDealtCount++;
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
		hit.Draw();
		stand.Draw();
		//Double.Draw();
		if (playerHand.size()>=2 && playerHand[0].value == playerHand[1].value) {
			Split.Draw();
		}
}

void Game::DrawBackground(){
	DrawTexture(gameImage.matTexture, 0, 0, WHITE);
	if (state==GameState::playerTurn) DrawUpsideCard();
	DrawRectangle(705, 0, 295, 900, lightGreen);
	DrawRectangle(700, 0, 5, 900, BLACK);
	DrawRectangleLinesEx({ 0,0,1000,900 }, 5, BLACK);
}

void Game::DrawUpsideCard(){
	if (state==GameState::playerTurn) {
			DrawTexture(gameImage.hiddenCardTexture, 180, 27, WHITE);
	}
}

int Game::GetScore(const vector<valRank>& hand){
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

void Game::HitPlayer(){
	valRank vard = deck.DrawCard();
	Card card(50 + playerCards.size() * cardSpacing, player_Y, vard,&suitImages[vard.rank],&mainFont);
	playerCards.push_back(card);
	playerHand.push_back(vard);
}

void Game::UpdateButtons(){
	if (state==GameState::playerTurn) {
		if (hit.IsButtonPressed() && GetScore(playerHand) < 21) {
			HitPlayer();
		}
		if (stand.IsButtonPressed()||GetScore(playerHand)>=21) {
			state = GameState::dealerTurn;
			Card card(210, dealer_Y, cpuHiddenCard,&suitImages[cpuHiddenCard.rank],&mainFont);
			cpuCards.push_back(card);
			cpuHand.push_back(cpuHiddenCard);
			dealerLastUpdateTime = GetTime();
		}
	}
}

void Game::cpuGet17(){
	if (GetScore(cpuHand) >= 17) {
		return;
	}
	if (HasEnoughTimePassed(dealerLastUpdateTime, 0.8)) {
		valRank card = deck.DrawCard();
		cpuHand.push_back(card);
		cpuCards.emplace_back(50 + cpuCards.size() * cardSpacing, dealer_Y, card,&suitImages[card.rank],&mainFont);
	}
}

void Game::Update(){
	switch (state) {
	case GameState::dealing:
		UpdateDealing(0.8);
		break;
	case GameState::playerTurn:
		UpdateButtons();
		break;
	case GameState::dealerTurn:
		cpuGet17();
		break;
	default:
		break;
	}
}

bool Game::HasEnoughTimePassed(double& lastUpdateTime,double TimePassed){
	bool HasEnoughTimePassed=false;
	double CurrentTime = GetTime();
	if (CurrentTime - lastUpdateTime > TimePassed) {
		HasEnoughTimePassed = true;
		lastUpdateTime = CurrentTime;
	}
	return HasEnoughTimePassed;
}
