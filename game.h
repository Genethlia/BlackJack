#pragma once
#include "Source.h"
#include "Card.h"
#include "mainMenu.h"
#include "settings.h"
constexpr int player_Y = 650;
constexpr int dealer_Y = 50;
constexpr int cardSpacing = 160;
inline float screenWidth = 1200;
inline float screenHeight = 950;
using namespace std;

enum class ResultStates {
	None,
	Win,
	Lose,
	Push,
	Blackjack
};

enum class GameState{
	MainMenu,
	settings,
	stats,
	betting,
	dealing,
	playerTurn,
	dealerTurn,
	dealerPause,
	roundEnd,
};

struct PendingDeal {
	vector<Card>* cards;
	vector<valRank>* hand;
	vector<valRank>* results;
	int y;
};

struct popUpMessage {
	string message = " ";
	Color color = WHITE;
	double displayTime = 0.0;
	double startTime = 0.0;
	bool active = false;
};

struct Timers {
	double cardDealStart=0.0;//For dealing cards over time
	double animationStart=0.0;//For dealing cards over time
	double dealerTurnStart=0.0;//For dealer drawing cards over time
	double resultPauseStart=0.0;//For pausing before showing results

	void ResetTimers();
	void SetAllTimersToNow();
};

struct Hand {
	vector<valRank> cards;
	vector<valRank> results;
	vector<Card> visual;
	int bet = 0;
	void ClearAll();
};

class Game {
public:

	Game();

	void Draw();//All the drawing functions
	void Update();//All the update functions
	void SaveGame();//Saves the player's money to a file
	
private:

	bool roundOver;
	bool splitHand;//Is split pressed in this round
	bool dealToSplitHand;//For dealing to split hand
	bool surrendered;//Has the player surrendered this round
	bool CardsAreMoving();

	void UpdateDealing(double TimePassed); //Adds cards to player and dealer hands over time
	void DrawCards();//Draws all cards in a vector
	void DrawScore();//Draws the scores of player and dealer
	void DrawButtons();//Draws the buttons
	void DrawBackground();//Draws the table background
	void DrawDeck();
	void DrawBetButtons();//Draws the bet buttons
	void DrawMoneyBets();//Draws the player's money and current bet
	void UpdateButtons(Hand& player,int y);//Updates button states
	void cpuGet17();//Dealer draws cards until reaching at least 17
	void UpdateResults();//Calculates and displays the results of the round
	void UpdateBettingButtons();//Updates bets based on button presses
	void DrawResultText();//Draws the result text
	void ResetRound();//Resets the game state for a new round
	void StartRound();//Starts new round
	void SplitFunc();//Splits the player's hand into two hands
	void QueueHit(Hand& hand, int y);
	void ProcessDealQueue(double delay);//Processes the pending deal queue
	void ShowPopUp(string text,Color color,double duration);//Shows messages like "Not enough money"
	void DrawPopUpMessage();//Draws the popup message if active
	void DealerPauseUpdate(double duration);//Updates the dealer pause state
	void LoadLastGame();//Loads the player's money from a file
	void DrawHomeButton();//Draws the home button
	void UpdateHomeButton();//Updates the home button(Should be called in every frame when not in mainMenu)
	void UpdateCards();
	void RevealHiddenCard();

	Timers timers;

	int cardsDealtCount;
	int money;
	int GetScore(const vector<valRank>& hand);//Calculates the score of a hand
	int GetScoreOfCard(int cardValue);//Calculates the score of a single card,do not use for more than two cards with GetScore
	int YOfSplitCards = player_Y - 200 - 20;//Y position of split hand cards

	Hand playerMain;
	Hand playerSplit;
	Hand cpu;

	Button hit=Button(60,"HIT");
	Button stand = Button(210, "STAND");
	Button Double = Button(510, "DOUBLE");
	Button Split = Button(660, "SPLIT");
	Button Surrender = Button(360, "GIVE UP");

	BetButton betButtons[4] = { BetButton(150,10),BetButton(350,50),BetButton(550,100),BetButton(750,200) };
	AllInButton allInButton = AllInButton();

	UndoConfirmButton undoButton = UndoConfirmButton(550,"UNDO");
	UndoConfirmButton confirmButton = UndoConfirmButton(700,"CONFIRM BET");

	Images gameImage;
	Images suitImages[4];    // shared textures for suits

	Font mainFont;            // shared font

	GameState state = GameState::MainMenu;

	Color resultColor;
	Color GetresultColor(ResultStates r);//Returns color based on result state

	const char* resultText;
	const char* GetresultText(ResultStates r);//Returns text based on result state

	stack<int> lastBet;

	Deck deck;

	queue<PendingDeal> dealQueue;

	ResultStates ResolveHand(const Hand& hand);//Resolves a single hand against the dealer's hand
	ResultStates mainResult = ResultStates::None;
	ResultStates splitResult = ResultStates::None;

	popUpMessage popUp;

	MainMenu mainMenu;

	HomeButton homeButton = HomeButton();

	Settings settings = Settings();
};