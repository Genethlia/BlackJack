#include "game.h"

Game::Game(){
	deck = Deck();
	LastUpdateTime = 0;
	dealerLastUpdateTime = 0;
	cardsDealtCount = 0;
	money = 1000;
	bet = 0;
	cpuHiddenCard = {0, 0}; // Initialize to default value
	resultColor = RED;
	splitHand = false;
	dealToSplitHand = false;

	for (int i = 0; i < 4; i++) suitImages[i].LoadSuit(i);
	gameImage.LoadMatAndHiddenCard();
	mainFont = LoadFontEx("Fonts/ComSuns.ttf", 64, 0, 0);

	roundOver = false;

}

void Game::Draw() {
		DrawBackground();
		if (state == GameState::betting) DrawBetButtons();
		else { DrawButtons(); };
		DrawCards();
		DrawScore();
		DrawResultText();
		DrawMoneyBets();
	}

void Game::UpdateDealing(double TimePassed){
		if (HasEnoughTimePassed(LastUpdateTime, TimePassed)) {
			//valRank xard = deck.DrawCard();
			vector<valRank> temp;
			valRank xard = { 1,1 };//testing split
			switch(cardsDealtCount){
			case 0:
				playerCards.emplace_back(50,player_Y,xard,&suitImages[xard.rank],&mainFont);
				playerHand.push_back(xard);
				break;
			case 1:
				//xard = { 10,1 };
				cpuCards.emplace_back(50, dealer_Y, xard, &suitImages[xard.rank], &mainFont);
				cpuHand.push_back(xard);
				break;
			case 2:
				playerCards.emplace_back(210, player_Y, xard, &suitImages[xard.rank], &mainFont);
				playerHand.push_back(xard);
				break;
			case 3:
				//xard = { 1,2 };
				temp.push_back(xard);
				cpuHiddenCard = xard;
				if (GetScore(cpuHand) + GetScore(temp) == 21) {
					cpuCards.emplace_back(210, dealer_Y, xard, &suitImages[xard.rank], &mainFont);
					cpuHand.push_back(xard);
					state = GameState::roundEnd;
				}
				else {
					state = GameState::playerTurn;
				}
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
	DrawText(playerScoreText.c_str(), 920, 810, 32, WHITE);
	DrawText(cpuScoreText.c_str(), 920, 730, 32, WHITE);
}

void Game::DrawButtons(){
		hit.Draw();
		stand.Draw();
		if (bet <= money&&state==GameState::playerTurn) {
			Double.Draw();
			Split.y = 550;
		}
		else {
			Split.y = 400;
		}
		if (playerHand.size()==2 && GetScoreOfCard(playerHand[0].value) == GetScoreOfCard(playerHand[1].value)&&state==GameState::playerTurn) {
			Split.Draw();
		}
}

void Game::DrawBackground() {
	DrawTexture(gameImage.matTexture, 0, 0, WHITE);
	if (state == GameState::playerTurn) DrawUpsideCard();
	DrawRectangle(screenWidth - 295, 0, 295, 950, lightGreen);
	DrawRectangle(screenWidth - 300, 0, 5, 950, BLACK);
	DrawRectangleLinesEx({ 0,0,screenWidth,screenHeight }, 5, BLACK);
}

void Game::DrawUpsideCard(){
	if (state==GameState::playerTurn) {
			DrawTexture(gameImage.hiddenCardTexture, 180, 27, WHITE);
	}
}

void Game::DrawBetButtons(){
	for (int i = 0; i < 4; i++) {
		betButtons[i].Draw();
	}
	undoButton.Draw();
	confirmButton.Draw();
	allInButton.Draw();
}

void Game::DrawMoneyBets(){
	string mon = (to_string(money) + "$");
	float spacing = MeasureText(mon.c_str(), 32);
	DrawTextEx(mainFont, mon.c_str(), { screenWidth - spacing,5 }, 32, 2, WHITE);
	string be = ("Bet: " + to_string(bet) + "$");
	spacing = 290;
	DrawTextEx(mainFont, be.c_str(), { screenWidth-spacing,5 }, 32, 2, WHITE);
}

int Game::GetScore(const vector<valRank>& hand){
	int aces = 0;
	int score = 0;
	for (int i = 0; i < hand.size(); i++) {
		if (hand[i].value == 1) {
			aces++;
			score += 1;
		}
		else {
			score += GetScoreOfCard(hand[i].value);
		}
	}
	while (aces > 0&&score+10<=21) {
		score += 10;
		aces--;
	}
	return score;
}

int Game::GetScoreOfCard(int cardValue)
{
	if (cardValue >= 10) {
		return 10;
	}
	return cardValue;
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
		else if (stand.IsButtonPressed()||GetScore(playerHand)>=21) {
			if (!splitHand || dealToSplitHand) {
				state = GameState::dealerTurn;
				Card card(210, dealer_Y, cpuHiddenCard, &suitImages[cpuHiddenCard.rank], &mainFont);
				cpuCards.push_back(card);
				cpuHand.push_back(cpuHiddenCard);
				dealerLastUpdateTime = GetTime();
			}
			else {
				dealToSplitHand = true;
			}
		}
		else if (Double.IsButtonPressed()&&bet<=money) {
			money -= bet;
			bet *= 2;
			HitPlayer();
			if (!splitHand || dealToSplitHand) {
				state = GameState::dealerTurn;
				dealerLastUpdateTime = GetTime();
			}
			else {
				dealToSplitHand = true;
			}
		}
		else if (Split.IsButtonPressed() && playerHand.size() == 2 && playerHand[0].value == playerHand[1].value&&!splitHand) {
			splitHand = true;
			SplitFunc();
			//state = GameState::dealerTurn;
			//dealerLastUpdateTime = GetTime();
		}
	}
}

void Game::cpuGet17(){
	if (GetScore(cpuHand) >= 17||GetScore(playerHand)>21||(GetScore(playerHand)==21&&playerHand.size()==2)) {
		state = GameState::roundEnd;
		return;
	}
	if (HasEnoughTimePassed(dealerLastUpdateTime, 0.8)) {
		valRank card = deck.DrawCard();
		cpuHand.push_back(card);
		cpuCards.emplace_back(50 + cpuCards.size() * cardSpacing, dealer_Y, card,&suitImages[card.rank],&mainFont);
	}
}

void Game::UpdateResults(){
	if (roundOver) return;
	if (GetScore(playerHand) > 21) {
		resultText = "YOU LOST";
		resultColor = RED;
	}
	else if(GetScore(playerHand)==21&&playerHand.size()==2){
		resultText = "BLACKJACK";
		resultColor = GOLD;
		money += bet * 5 / 2;
	}
	else {
		if (GetScore(cpuHand)>21) {
			resultText = "YOU WON";
			resultColor = GREEN;
			money += bet * 2;
		}
		else if (GetScore(playerHand) > GetScore(cpuHand)) {
			resultText = "YOU WON";
			resultColor = GREEN;
			money += bet * 2;
		}
		else if (GetScore(playerHand) == GetScore(cpuHand)) {
			resultText = "PUSH";
			resultColor = YELLOW;
			money += bet;
		}
		else {
			resultText = "YOU LOST";
			resultColor = RED;
		}
	}
	roundOver = true;
}

void Game::UpdateBettingButtons()
{
	for (int i = 0; i < 4; i++) {
		if (betButtons[i].IsButtonPressed()) {
			int betAmount = betButtons[i].betAmount;
			if (money >= betAmount) {
				bet += betAmount;
				money -= betAmount;
				lastBet.push(betAmount);
			}
			else if(money!=0){
				bet += money;
				lastBet.push(money);
				money = 0;
			}
		}
	}
	if (undoButton.IsButtonPressed()) {
		if (!lastBet.empty()) {
			bet -= lastBet.top();
			money += lastBet.top();
			lastBet.pop();
		}
	}
	if (confirmButton.IsButtonPressed()) {
		if (bet != 0) {
			state = GameState::dealing;
			LastUpdateTime = GetTime();
		}
	}
	if (allInButton.IsButtonPressed()) {
		if (money != 0) {
			bet += money;
			lastBet.push(money);
			money = 0;
		}
	}
}

void Game::DrawResultText(){
	if (!resultText.empty()) {
		string PlayAgain = "Press any key to play again";
		int x = (screenWidth-300) / 2;
		int y = (screenHeight-250) / 2;
		int fontSize = 128;
		int TextWidth = MeasureText(resultText.c_str(), fontSize);
		int positionX = x - TextWidth / 2;
		int otherpositionX = x - TextWidth / 2 + 80;
		if (resultText == "PUSH") positionX = 270,otherpositionX=220;
		DrawText(resultText.c_str(),positionX, y, fontSize, resultColor);
		DrawText(PlayAgain.c_str(),otherpositionX , y + 200, 32, WHITE);
	}
}

void Game::ResetRound(){
	if (GetKeyPressed()) {
		state = GameState::betting;
		bet = 0;
		while (!lastBet.empty()) {
			lastBet.pop();
		}
		LastUpdateTime = 0;
		dealerLastUpdateTime = 0;
		cardsDealtCount = 0;
		cpuHiddenCard = { 0, 0 };
		roundOver = false;
		resultText.clear();
		cpuHand.clear();
		cpuCards.clear();
		playerHand.clear();
		playerCards.clear();
	}
}

void Game::SplitFunc(){
	int cardHeight = playerCards[0].height;
	int heightOfSplitCards = player_Y - cardHeight - 20;
	playerHandSplit.push_back(playerHand[1]);
	playerCardsSplit.emplace_back(50, heightOfSplitCards, playerHandSplit[0], &suitImages[playerCards[1].card.rank], &mainFont);
	playerCards.pop_back();
	playerHand.pop_back();
}


void Game::Update(){
	switch (state) {
	case GameState::betting:
		UpdateBettingButtons();
		break;
	case GameState::dealing:
		UpdateDealing(0.8);
		break;
	case GameState::playerTurn:
		UpdateButtons();
		break;
	case GameState::dealerTurn:
		cpuGet17();
		break;
	case GameState::roundEnd:
		UpdateResults();
		ResetRound();
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
