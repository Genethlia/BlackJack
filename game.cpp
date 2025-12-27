#include "game.h"

Game::Game(){
	deck = Deck();
	lastDealTime = 0;
	LastUpdateTime = 0;
	dealerLastUpdateTime = 0;
	dealPauseTime = 0;
	cardsDealtCount = 0;
	money = 0;
	LoadMoney();
	mainBet = 0;
	splitBet = 0;
	cpuHiddenCard = {0, 0}; // Initialize to default value
	resultColor = RED;
	splitHand = false;
	dealToSplitHand = false;

	for (int i = 0; i < 4; i++) suitImages[i].LoadSuit(i);
	gameImage.LoadMatAndHiddenCard();
	mainFont = LoadFontEx("Fonts/ComSuns.ttf", 64, 0, 0);

	roundOver = false;

	resultText = "";

}

void Game::Draw() {
	if (state == GameState::MainMenu) {
		mainMenu.Draw();
	}
	else {
		DrawBackground();
		if (state == GameState::betting) DrawBetButtons();
		else { DrawButtons(); };
		if (state != GameState::roundEnd && state != GameState::betting) DrawCards();
		DrawScore();
		if (state == GameState::roundEnd)	DrawResultText();
		DrawMoneyBets();
		DrawPopUpMessage();
	}
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
				if (GetScore(cpuHand) + GetScoreOfCard(xard.value) == 21) {
					cpuCards.emplace_back(210, dealer_Y, xard, &suitImages[xard.rank], &mainFont);
					cpuHand.push_back(xard);
					dealPauseTime = GetTime();
					ShowPopUp("DEALER HIT A BLACKJACK", RED, 3);
					state = GameState::dealerPause;
				}
				else {
					ShowPopUp("PLAYER'S TURN", WHITE, 3);
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
	for (int i = 0; i < static_cast<int>(playerCardsSplit.size()); ++i) {
		playerCardsSplit[i].Draw();
	}
}

void Game::DrawScore(){
	string Text;
	int size = 32;
	if (!splitHand) {
		Text = "Player Score: ";
	}
	else {
		size = 24;
		Text = "First Hand Score: ";
		int splitScore = GetScore(playerHandSplit);
		string splitScoreText = "Second Hand Score: " + to_string(splitScore);
		DrawText(splitScoreText.c_str(), 910, 880, size, WHITE);
	}
	int playerScore = GetScore(playerHand);
	int cpuScore = GetScore(cpuHand);
	string playerScoreText = Text + to_string(playerScore);
	string cpuScoreText = "Dealer Score: " + to_string(cpuScore);
	DrawText(playerScoreText.c_str(), 910, 810, size, WHITE);
	DrawText(cpuScoreText.c_str(), 910, 730, 32, WHITE);
}

void Game::DrawButtons(){
		hit.Draw();
		stand.Draw();
		if (mainBet <= money && state == GameState::playerTurn) {
			Double.Draw();
			if (playerHand.size() == 2 && GetScoreOfCard(playerHand[0].value) == GetScoreOfCard(playerHand[1].value)) {
				Split.Draw();
			}
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
	string be = ("Bet: " + to_string(mainBet) + "$");
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
	if (cardValue == 1) {
		return 11;
	}

	return cardValue;
}


void Game::UpdateButtons(vector<Card>& playerCards, vector<valRank>& playerHand,int y){
	if (!dealQueue.empty()) return;

	if (state==GameState::playerTurn) {
		if (hit.IsButtonPressed() && GetScore(playerHand) < 21) {
			QueueHit(playerCards,playerHand,y);
		}
		else if ((stand.IsButtonPressed()||GetScore(playerHand)>=21)&&playerHand.size()>=2) {
			if (!splitHand || dealToSplitHand) {
				state = GameState::dealerTurn;
				Card card(210, dealer_Y, cpuHiddenCard, &suitImages[cpuHiddenCard.rank], &mainFont);
				cpuCards.push_back(card);
				cpuHand.push_back(cpuHiddenCard);
				dealerLastUpdateTime = GetTime();
			}
			else {
				ShowPopUp("DEALING TO SPLIT HAND", WHITE, 3);
				dealToSplitHand = true;
			}
		}
		else if (stand.IsButtonPressed() && playerHand.size() < 2) {
			ShowPopUp("YOU MUST HAVE AT LEAST 2 CARDS TO STAND", RED, 3);
		}
		else if (Double.IsButtonPressed()&&mainBet<=money) {
			money -= mainBet;
			mainBet *= 2;
			QueueHit(playerCards,playerHand,y);
			if (!splitHand || dealToSplitHand) {
				state = GameState::dealerTurn;
				dealerLastUpdateTime = GetTime();
			}
			else {
				ShowPopUp("DEALING TO SPLIT HAND", WHITE, 3);
				dealToSplitHand = true;
			}
		}
		else if (Split.IsButtonPressed() && playerHand.size() == 2 && playerHand[0].value == playerHand[1].value&&!splitHand&&mainBet<=money) {
			splitHand = true;
			SplitFunc();
		}
	}
}

void Game::cpuGet17(){
	bool ShouldTheDealerStand;
	if (!splitHand) {
		ShouldTheDealerStand = GetScore(cpuHand) >= 17 || GetScore(playerHand) > 21 || (GetScore(playerHand) == 21 && playerHand.size() == 2);
	}
	else {
		ShouldTheDealerStand = GetScore(cpuHand) >= 17 || ((GetScore(playerHand) > 21 || GetScore(playerHand) == 21 && playerHand.size() == 2) && (GetScore(playerHandSplit) > 21 || GetScore(playerHandSplit) == 21 && playerHandSplit.size() == 2));
		}
	if (ShouldTheDealerStand) {
		if (GetScore(cpuHand) <= 21) {
			ShowPopUp("DEALER STANDS", WHITE, 3);
		}
		else {
			ShowPopUp("DEALER BUSTED", WHITE, 3);
		}
		dealPauseTime = GetTime();
		state = GameState::dealerPause;
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

	mainResult = ResolveHand(playerHand, mainBet);
	if (splitHand) {
		splitResult = ResolveHand(playerHandSplit, splitBet);
		resultText = "SPLIT RESULTS";
		resultColor = WHITE;
	}
	else {
		resultText = GetresultText(mainResult);
		resultColor = GetresultColor(mainResult);
	}
	roundOver = true;
}

void Game::UpdateBettingButtons()
{
	for (int i = 0; i < 4; i++) {
		if (betButtons[i].IsButtonPressed()) {
			int betAmount = betButtons[i].betAmount;
			if (money >= betAmount) {
				mainBet += betAmount;
				money -= betAmount;
				lastBet.push(betAmount);
			}
			else if(money!=0){
				mainBet += money;
				lastBet.push(money);
				money = 0;
			}
		}
	}
	if (undoButton.IsButtonPressed()) {
		if (!lastBet.empty()) {
			mainBet -= lastBet.top();
			money += lastBet.top();
			lastBet.pop();
		}
	}
	if (confirmButton.IsButtonPressed()) {
		if (mainBet != 0) {
			state = GameState::dealing;
			LastUpdateTime = GetTime();
		}
	}
	if (allInButton.IsButtonPressed()) {
		if (money != 0) {
			mainBet += money;
			lastBet.push(money);
			money = 0;
		}
	}
}

void Game::DrawResultText(){
		const char* PlayAgain = "Press any key to play again";
		int x = (screenWidth-300) / 2;
		int y = (screenHeight-350) / 2;
		int fontSize = 128;
		int TextWidth = MeasureText(resultText, fontSize);
		int positionX = x - TextWidth / 2;
		int otherpositionX = x - TextWidth / 2 + 80;
		if (resultText == "PUSH") positionX = 270,otherpositionX=220;
		if (resultText == "SPLIT RESULTS") fontSize = 80,positionX=100;
		DrawText(resultText,positionX, y, fontSize, resultColor);
		if (splitHand) {
			DrawText(TextFormat("HAND 1 %s", GetresultText(mainResult)),300, 500, fontSize/2,GetresultColor(mainResult));

			DrawText(TextFormat("HAND 2 %s", GetresultText(splitResult)),300, 600, fontSize/2,GetresultColor(splitResult));

			DrawText("Press any key to play again", 200, 800, 32, BLACK);
			return;
		}
		else {
			DrawText(PlayAgain, 210, y + 200, 32, BLACK);
		}
}

void Game::ResetRound(){
	if (GetKeyPressed()) {
		state = GameState::betting;
		mainBet = 0;
		splitBet = 0;
		while (!lastBet.empty()) {
			lastBet.pop();
		}
		LastUpdateTime = 0;
		dealerLastUpdateTime = 0;
		cardsDealtCount = 0;
		lastDealTime = 0;
		dealPauseTime = 0;
		cpuHiddenCard = { 0, 0 };
		roundOver = false;
		splitHand = false;
		dealToSplitHand = false;
		resultText="";
		cpuHand.clear();
		cpuCards.clear();
		playerHand.clear();
		playerCards.clear();
		playerCardsSplit.clear();
		playerHandSplit.clear();
		ResultStates mainResult = ResultStates::None;
		ResultStates splitResult = ResultStates::None;
		SaveMoney();
	}
}

void Game::SplitFunc(){
	playerHandSplit.push_back(playerHand[1]);
	playerCardsSplit.emplace_back(50, YOfSplitCards, playerHandSplit[0], &suitImages[playerCards[1].card.rank], &mainFont);
	playerCards.pop_back();
	playerHand.pop_back();

	splitBet = mainBet;
	money -= splitBet;

	lastDealTime = GetTime();
}

void Game::QueueHit(vector<Card>& cards, vector<valRank>& hand, int y){
	dealQueue.push({ &cards,&hand,y });
}

void Game::ProcessDealQueue(double delay){
	if (dealQueue.empty()) return;

	if (HasEnoughTimePassed(lastDealTime, delay)) {
		auto& d = dealQueue.front();
		valRank v = deck.DrawCard();

		d.cards->emplace_back(50 + d.cards->size() * cardSpacing, d.y, v, &suitImages[v.rank], &mainFont);
		d.hand->emplace_back(v);

		dealQueue.pop();
	}
}

void Game::ShowPopUp(string text, Color color, double duration){
	popUp.message = text;
	popUp.color = color;
	popUp.displayTime = duration;
	popUp.startTime = GetTime();
	popUp.active = true;
}

void Game::DrawPopUpMessage(){
	if (!popUp.active) return;

	double now = GetTime();

	if (now - popUp.startTime > popUp.displayTime) {
		popUp.active = false;
		return;
	}

	int fontSize = 32;
	int padding = 20;
	int textWidth = MeasureText(popUp.message.c_str(), fontSize);
	int textHeight = fontSize;

	int x = (screenWidth-300) / 2 - (textWidth + 2 * padding) / 2;
	int y = screenHeight / 2 - (textHeight + 2 * padding) / 2;

	float alpha = 1.0f - (GetTime() - popUp.startTime) / popUp.displayTime;
	DrawRectangle(x, y,textWidth + 2 * padding,textHeight + 2 * padding,Fade(BLACK, 0.6f*alpha));
	DrawRectangleLines(x, y, textWidth + 2 * padding, textHeight + 2 * padding, Fade(WHITE, alpha));
	DrawText(popUp.message.c_str(), x + padding, y + padding, fontSize, Fade(popUp.color, alpha));
}

void Game::DealerPauseUpdate(double duration){
	if (GetTime() - dealPauseTime > duration) {
		state = GameState::roundEnd;
	}
	return;
}

void Game::SaveMoney(){
	ofstream fin("save.txt");
	if (fin.is_open()) {
		fin << money;
		fin.close();
	}
	else {
		ShowPopUp("Failed To Save Game", RED, 3);
	}
}

void Game::LoadMoney(){
	ifstream fout("save.txt");
	if (fout.is_open()) {
		fout >> money;
		fout.close();
	}
	else if(money==0){
		money = 1000;
	}
}

Color Game::GetresultColor(ResultStates r)
{
	switch (r) {
	case ResultStates::Win:	return GREEN;
	case ResultStates::Lose:return RED;
	case ResultStates::Push:return YELLOW;
	case ResultStates::Blackjack:return GOLD;
	default:
		break;
	}
	return WHITE;
}

const char* Game::GetresultText(ResultStates r)
{
	switch (r) {
	case ResultStates::Win:
		if (splitHand) {
			return "WON";
		}
		return "YOU WON";
	case ResultStates::Lose:
		if (!splitHand) {
			return "YOU LOST";
		}
		return "LOST";
	case ResultStates::Push:return "PUSH";
	case ResultStates::Blackjack:return "BLACKJACK";
	default:
		break;
	}
	return "";
}

ResultStates Game::ResolveHand(const vector<valRank>& hand, int betAmount){
	int playerScore = GetScore(hand);
	int dealerScore = GetScore(cpuHand);
	if (playerScore > 21) return ResultStates::Lose;
	if (dealerScore > 21) {
		money += betAmount * 2;
		return ResultStates::Win;
	}
	else if (!splitHand &&playerScore == 21 && hand.size() == 2) {
		money += betAmount * 5 / 2;
		return ResultStates::Blackjack;
	}
	else if (playerScore > dealerScore && playerScore <= 21) {
		money += betAmount * 2;
		return ResultStates::Win;
	}
	else if (playerScore == dealerScore) {
		money += betAmount;
		return ResultStates::Push;
	}
	return ResultStates::Lose;
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
		if (!dealToSplitHand) {
			UpdateButtons(playerCards, playerHand,player_Y);
		}
		else {
			UpdateButtons(playerCardsSplit, playerHandSplit,YOfSplitCards);
		}
		ProcessDealQueue(0.8);
		break;
	case GameState::dealerTurn:
		cpuGet17();
		break;
	case GameState::dealerPause:
		DealerPauseUpdate(3);
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
