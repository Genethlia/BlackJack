#include "game.h"

Game::Game(){
	InitAudioDevice();
	gameAudio.LoadAll();


	deck = Deck();
	cardsDealtCount = 0;
	money = 1000;
	resultColor = RED;
	splitHand = false;
	dealToSplitHand = false;
	surrendered = false;

	for (int i = 0; i < 4; i++) suitImages[i].LoadSuit(i);
	gameImage.LoadMatHiddenCardAndHome();
	mainFont = LoadFontEx("Fonts/ComSuns.ttf", 64, 0, 0);

	roundOver = false;

	resultText = "";

	UpdateScale();

	SetTransform();
}

Game::~Game(){
	CloseAudioDevice();
}

void Game::Draw() {
	
	if (state == GameState::MainMenu) {
		mainMenu.Draw();
	}
	else if (state == GameState::settings) {
		DrawHomeButton();
		settings.Draw();
	}
	else {
		DrawBackground();
		DrawHomeButton();
		if (state == GameState::betting) DrawBetButtons();
		else { DrawButtons(); };
		if (state != GameState::roundEnd && state != GameState::betting) DrawCards();
		DrawScore();
		if (state == GameState::roundEnd)	DrawResultText();
		DrawMoneyBets();
		DrawPopUpMessage();
	}
}

bool Game::CardsAreMoving(){
	for (auto& c : playerMain.visual) if (c.IsMoving()) return true;
	for (auto& c : playerSplit.visual) if (c.IsMoving()) return true;
	for (auto& c : cpu.visual) if (c.IsMoving()) return true;
	return false;
}

void Game::UpdateDealing(double TimePassed){
	if (CardsAreMoving()) return;

	if (HasEnoughTimePassed(timers.animationStart, TimePassed)) {
		valRank xard = deck.DrawCard();
		//valRank xard = { 1,1 };//testing split
		switch (cardsDealtCount) {
		case 0:
			playerMain.visual.emplace_back(50, player_Y, xard, &suitImages[xard.rank], &mainFont, &gameImage, &playerMain.results, &gameAudio, &settings.IsSoundEffectsOn);
			playerMain.cards.push_back(xard);
			break;
		case 1:
			cpu.visual.emplace_back(50, dealer_Y, xard, &suitImages[xard.rank], &mainFont, &gameImage, &cpu.results, &gameAudio, &settings.IsSoundEffectsOn);
			cpu.cards.push_back(xard);
			break;
		case 2:
			playerMain.visual.emplace_back(210, player_Y, xard, &suitImages[xard.rank], &mainFont, &gameImage, &playerMain.results, &gameAudio, &settings.IsSoundEffectsOn);
			playerMain.cards.push_back(xard);
			break;
		case 3:
			cpu.visual.emplace_back(210, dealer_Y, xard, &suitImages[xard.rank], &mainFont, &gameImage, &cpu.results, &gameAudio, &settings.IsSoundEffectsOn);
			cpu.visual.back().SetFaceDown(true);
			cpu.cards.push_back(xard);
			if (GetScore(cpu.cards) == 21) {
				timers.resultPauseStart = GetTime();
				ShowPopUp("DEALER HIT A BLACKJACK", RED, 3);
				cpu.visual.back().SetFaceDown(false);
				PlaySoundEffect(gameAudio.dealerBlackjack);
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
	for (int i = 0; i < static_cast<int>(playerMain.visual.size()); ++i) {
		playerMain.visual[i].Draw();
	}
	for (int i = 0; i < static_cast<int>(cpu.visual.size()); ++i) {
		cpu.visual[i].Draw();
	}
	for (int i = 0; i < static_cast<int>(playerSplit.visual.size()); ++i) {
		playerSplit.visual[i].Draw();
	}
}

void Game::DrawScore(){
	string Text;
	int size = 32;
	int YofScore=865;
	if (!splitHand) {
		Text = "Player Score: ";
		YofScore = 880;
	}
	else {
		size = 24;
		Text = "First Hand Score: ";
		int splitScore = GetScore(playerSplit.results);
		string splitScoreText = "Second Hand Score: " + to_string(splitScore);
		DrawText(splitScoreText.c_str(), 915, 910, size, WHITE);
	}
	int playerScore = GetScore(playerMain.results);
	int cpuScore = GetScore(cpu.results);
	string playerScoreText = Text + to_string(playerScore);
	string cpuScoreText = "Dealer Score: " + to_string(cpuScore);
	DrawText(playerScoreText.c_str(), 915, YofScore, size, WHITE);
	DrawText(cpuScoreText.c_str(), 915, 810, 32, WHITE);
}

void Game::DrawButtons(){
		hit.Draw();
		stand.Draw();
		Surrender.Draw();
		if (playerMain.bet <= money && state == GameState::playerTurn) {
			Double.Draw();
			if (playerMain.cards.size() == 2 && GetScoreOfCard(playerMain.cards[0].value) == GetScoreOfCard(playerMain.cards[1].value)) {
				Split.Draw();
			}
		}
}

void Game::DrawBackground() {
	DrawTexture(gameImage.matTexture, 0, 0, WHITE);
	if (state != GameState::betting && state != GameState::roundEnd) DrawDeck();
	DrawRectangle(VIRTUAL_WIDTH - 295, 0, 295, 950, lightGreen);
	DrawRectangle(VIRTUAL_WIDTH - 300, 0, 5, 950, BLACK);
	DrawRectangleLinesEx({ 0,0,VIRTUAL_WIDTH,VIRTUAL_HEIGHT }, 5, BLACK);
}

void Game::DrawDeck(){
	DrawTexture(gameImage.hiddenCardTexture, 700, 300, WHITE);
}


void Game::DrawBetButtons(){
	for (int i = 0; i < 4; i++) {
		betButtons[i].Draw();
	}
	undoButton.Draw();
	confirmButton.Draw();
	allInButton.Draw();
}

void Game::DrawMoneyBets() {
	string mon = (to_string(money) + "$");
	float spacing = MeasureText(mon.c_str(), 32);
	DrawTextEx(mainFont, mon.c_str(), { VIRTUAL_WIDTH - spacing,5 }, 32, 2, WHITE);
	string be = ("Bet: " + to_string(playerMain.bet) + "$");
	spacing = 290;
	DrawTextEx(mainFont, be.c_str(), { VIRTUAL_WIDTH - spacing,5 }, 32, 2, WHITE);
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


void Game::UpdateButtons(Hand &player,int y){
	if (!dealQueue.empty()||CardsAreMoving()) return;

	if (state==GameState::playerTurn) {
		if (hit.IsButtonPressed() && GetScore(player.cards) < 21) {
			QueueHit(player,y);
		}
		else if ((stand.IsButtonPressed()||GetScore(player.cards)>=21)&&player.cards.size()>=2) {
			if (!splitHand || dealToSplitHand) {
				state = GameState::dealerTurn;
				RevealHiddenCard();
				timers.dealerTurnStart = GetTime();
			}
			else {
				ShowPopUp("DEALING TO SPLIT HAND", WHITE, 3);
				dealToSplitHand = true;
			}
		}
		else if (stand.IsButtonPressed() && player.cards.size() < 2) {
			ShowPopUp("YOU MUST HAVE AT LEAST 2 CARDS TO STAND", RED, 3);
		}
		else if (Surrender.IsButtonPressed()) {
			if (player.cards.size() != 2) {
				ShowPopUp("YOU CAN ONLY SURRENDER WITH 2 CARDS", RED, 3);
			}
			else if (splitHand) {
				ShowPopUp("YOU CANNOT SURRENDER AFTER SPLITTING", RED, 3);
			}
			else {
				surrendered = true;
				money += playerMain.bet / 2;
				ShowPopUp("YOU SURRENDERED, HALF YOUR BET HAS BEEN RETURNED", WHITE, 3);
				state = GameState::dealerPause;
				timers.resultPauseStart = GetTime();
				RevealHiddenCard();
				PlaySoundEffect(gameAudio.playerSurrender);
			}
		}
		else if (Double.IsButtonPressed()&&playerMain.bet<=money) {
			money -= playerMain.bet;
			playerMain.bet *= 2;
			QueueHit(player,y);
			if (!splitHand || dealToSplitHand) {
				state = GameState::dealerTurn;
				RevealHiddenCard();
				timers.dealerTurnStart = GetTime();
			}
			else {
				ShowPopUp("DEALING TO SPLIT HAND", WHITE, 3);
				dealToSplitHand = true;
			}
		}
		else if (Split.IsButtonPressed() && player.cards.size() == 2 && GetScoreOfCard(player.cards[0].value)==GetScoreOfCard(player.cards[1].value) && !splitHand && playerMain.bet <= money) {
			splitHand = true;
			SplitFunc();
		}
	}
}

void Game::cpuGet17(){
	if (CardsAreMoving()) return;

	int Stop = settings.ShouldDealerHitOn17 ? 18 : 17;

	bool ShouldTheDealerStand;
	if (!splitHand) {
		ShouldTheDealerStand = GetScore(cpu.cards) >= Stop || GetScore(playerMain.cards) > 21 || (GetScore(playerMain.cards) == 21 && playerMain.cards.size() == 2);
	}
	else {
		ShouldTheDealerStand = GetScore(cpu.cards) >= Stop || ((GetScore(playerMain.cards) > 21 || GetScore(playerMain.cards) == 21 && playerMain.cards.size() == 2) && (GetScore(playerSplit.cards) > 21 || GetScore(playerSplit.cards) == 21 && playerSplit.cards.size() == 2));
		}
	if (ShouldTheDealerStand) {
		if (GetScore(cpu.cards) <= 21) {
			ShowPopUp("DEALER STANDS", WHITE, 3);
		}
		else {
			ShowPopUp("DEALER BUSTED", WHITE, 3);
		}
		timers.resultPauseStart = GetTime();
		state = GameState::dealerPause;
		return;
	}
	if (HasEnoughTimePassed(timers.dealerTurnStart, 0.8)) {
		valRank card = deck.DrawCard();
		cpu.cards.push_back(card);
		cpu.visual.emplace_back(50 + cpu.visual.size() * cardSpacing, dealer_Y, card, &suitImages[card.rank], &mainFont, &gameImage, &cpu.results, &gameAudio, &settings.IsSoundEffectsOn);
	}
}

void Game::UpdateResults(){
	if (roundOver) return;
	if (!surrendered) {
		mainResult = ResolveHand(playerMain);
		if (splitHand) {
			splitResult = ResolveHand(playerSplit);
			resultText = "SPLIT RESULTS";
			resultColor = WHITE;
		}
		else {
			resultText = GetresultText(mainResult);
			resultColor = GetresultColor(mainResult);
		}
	}
	else {
		resultText = "YOU GAVE UP";
		resultColor = RED;
	}
	roundOver = true;
}

void Game::UpdateBettingButtons()
{
	for (int i = 0; i < 4; i++) {
		if (betButtons[i].IsButtonPressed()) {
			int betAmount = betButtons[i].betAmount;
			if (money >= betAmount) {
				playerMain.bet += betAmount;
				money -= betAmount;
				lastBet.push(betAmount);
			}
			else if(money!=0){
				playerMain.bet += money;
				lastBet.push(money);
				money = 0;
			}
		}
	}
	if (undoButton.IsButtonPressed()) {
		if (!lastBet.empty()) {
			playerMain.bet -= lastBet.top();
			money += lastBet.top();
			lastBet.pop();
		}
	}
	if (confirmButton.IsButtonPressed()) {
		if (playerMain.bet != 0) {
			state = GameState::dealing;
			timers.animationStart = GetTime();
		}
	}
	if (allInButton.IsButtonPressed()) {
		if (money != 0) {
			playerMain.bet += money;
			lastBet.push(money);
			money = 0;
		}
	}
	UpdateHomeButton();
}

void Game::DrawResultText(){
		const char* PlayAgain = "Press any key to play again";
		int x = (VIRTUAL_WIDTH-300) / 2;
		int y = (VIRTUAL_HEIGHT-350) / 2;
		int fontSize = 116;
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
		state = GameState::betting;
		playerMain.bet = 0;
		playerSplit.bet = 0;
		while (!lastBet.empty()) {
			lastBet.pop();
		}
		timers.ResetTimers();
		cardsDealtCount = 0;
		roundOver = false;
		splitHand = false;
		dealToSplitHand = false;
		surrendered = false;
		resultText = "";
		cpu.ClearAll();
		playerMain.ClearAll();
		playerSplit.ClearAll();
		ResultStates mainResult = ResultStates::None;
		ResultStates splitResult = ResultStates::None;
		mainMenu.placeholder = -1;
}

void Game::StartRound(){
	ResetRound();
	money = 1000;
}

void Game::SplitFunc(){
	playerSplit.cards.push_back(playerMain.cards[1]);
	playerSplit.visual.emplace_back(50, YOfSplitCards, playerSplit.cards[0], &suitImages[playerMain.visual[1].card.rank], &mainFont, &gameImage, &playerSplit.results, &gameAudio, &settings.IsSoundEffectsOn);
	playerMain.visual.pop_back();
	playerMain.cards.pop_back();
	playerMain.results.pop_back();

	playerSplit.bet = playerMain.bet;
	money -= playerSplit.bet;

	timers.cardDealStart = GetTime();
}

void Game::QueueHit(Hand& hand, int y){
	dealQueue.push({ &hand.visual,&hand.cards,&hand.results,y });
}

void Game::ProcessDealQueue(double delay){
	if (dealQueue.empty()) return;

	if (HasEnoughTimePassed(timers.cardDealStart, delay)) {
		auto& d = dealQueue.front();
		valRank v = deck.DrawCard();

		d.cards->emplace_back(50 + d.cards->size() * cardSpacing, d.y, v, &suitImages[v.rank], &mainFont, &gameImage, d.results, &gameAudio, &settings.IsSoundEffectsOn);
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

	int fontSize = 24;
	int padding = 20;
	int textWidth = MeasureText(popUp.message.c_str(), fontSize);
	int textHeight = fontSize;

	int x = (VIRTUAL_WIDTH-300) / 2 - (textWidth + 2 * padding) / 2;
	int y = VIRTUAL_HEIGHT / 2 - (textHeight + 2 * padding) / 2;

	float alpha = 1.0f - (GetTime() - popUp.startTime) / popUp.displayTime;
	DrawRectangle(x, y,textWidth + 2 * padding,textHeight + 2 * padding,Fade(BLACK, 0.6f*alpha));
	DrawRectangleLines(x, y, textWidth + 2 * padding, textHeight + 2 * padding, Fade(WHITE, alpha));
	DrawText(popUp.message.c_str(), x + padding, y + padding, fontSize, Fade(popUp.color, alpha));
}

void Game::DealerPauseUpdate(double duration){
	if (GetTime() - timers.resultPauseStart > duration) {
		state = GameState::roundEnd;
	}
	return;
}

void Game::SaveGame() {
	fs::path filename("saves/save.txt");
	fs::path folder = filename.parent_path();
	if (!folder.empty() && !fs::exists(folder)) {
		fs::create_directories(folder);
	}
	settings.SaveSettings();
	if (state == GameState::settings || state == GameState::stats) return;

	deck.SaveDeck();
	ofstream fout("saves/save.txt");
	if (fout.is_open()) {
		fout << "Money: " << money << endl;

		fout << "State: ";
		if (state != GameState::MainMenu && state != GameState::roundEnd) {
			fout << int(state);
		}
		else {
			fout << int(GameState::betting);
		}
		fout << endl;
		fout << "MainBet: " << playerMain.bet << endl;

		vector<int> tempBets;
		stack<int> copyStack = lastBet;
		while (!copyStack.empty()) {
			tempBets.push_back(copyStack.top());
			copyStack.pop();
		}
		fout << "LastBetSize: " << tempBets.size() << endl;
		for (int i = tempBets.size() - 1; i >= 0; i--) {
			fout << "Bet" << i + 1 << ": " << tempBets[i] << endl;
		}

		fout << "PlayerHandSize: " << playerMain.cards.size() << endl << "PlayerHand: ";
		for (auto& card : playerMain.cards) fout << endl << card.value << " " << card.rank;
		fout << endl;
		fout << "DealerHandSize: " << cpu.cards.size() << endl << "DealerHand: ";
		for (auto& card : cpu.visual) fout << endl << card.card.value << " " << card.card.rank << " " << card.secret;
		fout << endl;
		fout << endl << "CardsDealt: " << cardsDealtCount << endl;
		fout << "IsRoundOver: " << roundOver << endl;
		fout << "IsSplitEnabledThisRound? " << splitHand << endl;
		fout << "SplitHandSize: " << playerSplit.cards.size() << endl << "SplitHand: ";
		for (auto& card : playerSplit.cards) fout << endl << card.value << " " << card.rank;
		fout << endl;
		fout << "SplitBet: " << playerSplit.bet << endl;
		fout << "ShouldYouDealToSplitHand: " << dealToSplitHand << endl;
		fout.close();
	}
	else {
		ShowPopUp("Failed To Save Game", RED, 3);
	}
}

Vector2 Game::offset()
{
	return Vector2(offsetX, offsetY);
}

void Game::LoadLastGame(){
	ifstream fin("saves/save.txt");
	if (fin.is_open()) {
		string label;
		fin >> label >> money;
		
		int Gamestate;
		fin >> label >> Gamestate;
		if (Gamestate <= (int)GameState::MainMenu || Gamestate > (int)GameState::roundEnd) {
			state = GameState::betting;
		}
		else {
			state = GameState(Gamestate);
		}

		int bet;
		fin >>label>> bet;
		playerMain.bet = bet;

		int lastBetsize;
		fin >> label >> lastBetsize;
		vector<int> lastBetUpside;
		for (int i = 0; i < lastBetsize; i++) {
			int temp;
			fin >> label >> temp;
			lastBet.push(temp);
		}

		if (money == 0 && state == GameState::betting&&bet==0) {
			money = 1000;
			ShowPopUp("COULDN'T LOAD GAME", RED, 2);
		}
		else if (state != GameState::betting) {
			int playerHandSize;
			fin >> label >> playerHandSize >> label;
			playerMain.ClearAll();
			for (int i = 0; i < playerHandSize; i++) {
				valRank card;
				fin >> card.value >> card.rank;
				playerMain.cards.push_back(card);
				playerMain.visual.emplace_back(50 + i * cardSpacing, player_Y, card, &suitImages[card.rank], &mainFont, &gameImage, &playerMain.results, &gameAudio, &settings.IsSoundEffectsOn);
				playerMain.visual[i].GoImmediatelyToTarget();
			}
			int cpuHandSize;
			fin >> label >> cpuHandSize >> label;
			cpu.ClearAll();
			for (int i = 0; i < cpuHandSize; i++) {
				valRank card;
				bool faced;
				fin >> card.value >> card.rank>>faced;
				cpu.cards.push_back(card);
				cpu.visual.emplace_back(50 + i * cardSpacing, dealer_Y, card, &suitImages[card.rank], &mainFont, &gameImage, &cpu.results, &gameAudio, &settings.IsSoundEffectsOn);
				cpu.visual[i].GoImmediatelyToTarget();
				cpu.visual[i].SetFaceDown(faced);
			}

			fin >> label >> cardsDealtCount;


			fin >> label >> roundOver;

			bool IsSplitActive;
			fin >> label >> IsSplitActive;
			splitHand = IsSplitActive;

			if (splitHand) {
				int playerHandSplitSize;
				fin >> label >> playerHandSplitSize >> label;
				playerSplit.ClearAll();
				for (int i = 0; i < playerHandSplitSize; i++) {
					valRank card;
					fin >> card.value >> card.rank;
					playerSplit.cards.push_back(card);
					playerSplit.visual.emplace_back(50 + i * cardSpacing, YOfSplitCards, card, &suitImages[card.rank], &mainFont, &gameImage, &playerSplit.results, &gameAudio, &settings.IsSoundEffectsOn);
					playerSplit.visual[i].GoImmediatelyToTarget();
				}
				fin >> label >> bet;
				playerSplit.bet = bet;
				fin >> label >> dealToSplitHand;
			}
			deck.LoadDeck();
			ShowPopUp("GAME LOADED", WHITE, 2);
		}
		fin.close();
	}
}

void Game::DrawHomeButton(){
	homeButton.Draw();
	DrawTexture(gameImage.mainMenuhomeTexture, homeButton.x , 5, WHITE);
}

void Game::UpdateHomeButton(){
	if (homeButton.IsButtonPressed()) {
		SaveGame();
		mainMenu.placeholder = -1;
		state = GameState::MainMenu;
	}
	return;
}

void Game::UpdateCards(){
	for (auto& c : playerMain.visual) c.Update();
	for (auto& c : playerSplit.visual) c.Update();
	for (auto& c : cpu.visual)c.Update();
}

void Game::RevealHiddenCard(){
	cpu.visual[1].SetFaceDown(false);
}

void Game::PlaySoundEffect(Sound sound){
	if (settings.IsSoundEffectsOn) {
		PlaySound(sound);
	}
}

void Game::UpdateScale(){
	screenW = GetScreenWidth();
	screenH = GetScreenHeight();

	scaleX = (float)screenW / VIRTUAL_WIDTH;
	scaleY = (float)screenH / VIRTUAL_HEIGHT;

	scale = (scaleX < scaleY) ? scaleX : scaleY;

	offsetX = (screenW - (VIRTUAL_WIDTH * scale)) / 2;
	offsetY = (screenH - (VIRTUAL_HEIGHT * scale)) / 2;
}

void Game::UpdateEveryFrame(){
	gameAudio.UpdateMusic(settings.IsMusicOn);
	UpdateScale();
}

void Game::SetTransform(){
	hit.SetTransform(&scale, &offsetX, &offsetY);
	stand.SetTransform(&scale, &offsetX, &offsetY);
	Double.SetTransform(&scale, &offsetX, &offsetY);
	Split.SetTransform(&scale, &offsetX, &offsetY);
	Surrender.SetTransform(&scale, &offsetX, &offsetY);

	for (int i = 0; i < 4; i++) {
		betButtons[i].SetTransform(&scale, &offsetX, &offsetY);
	}

	undoButton.SetTransform(&scale, &offsetX, &offsetY);
	confirmButton.SetTransform(&scale, &offsetX, &offsetY);
	allInButton.SetTransform(&scale, &offsetX, &offsetY);
	homeButton.SetTransform(&scale, &offsetX, &offsetY);

	settings.SetTransform(&scale, &offsetX, &offsetY);
	mainMenu.SetTransform(&scale, &offsetX, &offsetY);
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

ResultStates Game::ResolveHand(const Hand& hand){
	int playerScore = GetScore(hand.cards);
	int dealerScore = GetScore(cpu.cards);
	if (playerScore > 21) return ResultStates::Lose;
	if (dealerScore > 21) {
		money += hand.bet * 2;
		PlaySoundEffect(gameAudio.playerWin);
		return ResultStates::Win;
	}
	else if (!splitHand &&playerScore == 21 && hand.cards.size() == 2) {
		money += hand.bet * 5 / 2;
		PlaySoundEffect(gameAudio.playerBlackjack);
		return ResultStates::Blackjack;
	}
	else if (playerScore > dealerScore && playerScore <= 21) {
		money += hand.bet * 2;
		PlaySoundEffect(gameAudio.playerWin);
		return ResultStates::Win;
	}
	else if (playerScore == dealerScore) {
		money += hand.bet;
		PlaySoundEffect(gameAudio.playerPush);
		return ResultStates::Push;
	}
	PlaySoundEffect(gameAudio.playerLose);
	return ResultStates::Lose;
}


void Game::Update() {
	UpdateEveryFrame();
	switch (state) {
	case GameState::MainMenu:
		mainMenu.Update();
		homeButton.FindX(mainMenu.placeholder);
		if (mainMenu.placeholder == -1) {
			break;
		}
		else if (mainMenu.placeholder == 0) {
			StartRound();
		}
		else if (mainMenu.placeholder == 1) {
			StartRound();
			LoadLastGame();
			timers.SetAllTimersToNow();
		}
		else if (mainMenu.placeholder == 2) {
			state = GameState::settings;
		}
		else if (mainMenu.placeholder == 4) {
			ShouldWindowClose = true;
		}
		break;
	case GameState::settings:
		UpdateHomeButton();
		settings.Update();
		break;
	case GameState::betting:
		UpdateBettingButtons();
		break;
	case GameState::dealing:
		UpdateDealing(0.8);
		UpdateHomeButton();
		UpdateCards();
		break;
	case GameState::playerTurn:
		if (!dealToSplitHand) {
			UpdateButtons(playerMain, player_Y);
		}
		else {
			UpdateButtons(playerSplit, YOfSplitCards);
		}
		ProcessDealQueue(0.8);
		UpdateHomeButton();
		UpdateCards();
		break;
	case GameState::dealerTurn:
		cpuGet17();
		UpdateHomeButton();
		UpdateCards();
		break;
	case GameState::dealerPause:
		UpdateCards();
		DealerPauseUpdate(3);
		break;
	case GameState::roundEnd:
		UpdateResults();
		if (GetKeyPressed()) {
			ResetRound();
		}
		break;
	default:
		break;
	}
}

void Timers::ResetTimers(){
	cardDealStart = 0.0;
	animationStart = 0.0;
	dealerTurnStart = 0.0;
	resultPauseStart = 0.0;
}

void Timers::SetAllTimersToNow(){
	cardDealStart = GetTime();
	animationStart = GetTime();
	dealerTurnStart = GetTime();
	resultPauseStart = GetTime();
}

void Hand::ClearAll(){
	visual.clear();
	results.clear();
	cards.clear();
}
