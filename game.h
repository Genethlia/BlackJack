#pragma once
#include "Card.h"
#include "deck.h"
#include "button.h"
#include <vector>
#include <iostream>
constexpr int player_Y = 650;
constexpr int dealer_Y = 50;
constexpr int cardSpacing = 160;
constexpr int screenWidth = 1200;
constexpr int screenHeight = 950;
using namespace std;

enum class GameState{
	betting,
	dealing,
	playerTurn,
	dealerTurn,
	roundEnd,
};

class Game {
public:

	Game();

	void Draw();//All the drawing functions
	void Update();//All the update functions
	
private:

	bool HasEnoughTimePassed(double& lastUpdateTime, double TimePassed);
	bool roundOver;

	void UpdateDealing(double TimePassed); //Adds cards to player and dealer hands over time
	void DrawCards();//Draws all cards in a vector
	void DrawScore();//Draws the scores of player and dealer
	void DrawButtons();//Draws the buttons
	void DrawBackground();//Draws the table background
	void DrawUpsideCard();
	void HitPlayer();//Adds a card to the player's hand
	void UpdateButtons();//Updates button states
	void cpuGet17();//Dealer draws cards until reaching at least 17
	void UpdateResults();//Calculates and displays the results of the round
	void DrawResultText();//Draws the result text
	void ResetRound();//Resets the game state for a new round

	double LastUpdateTime;
	double dealerLastUpdateTime;

	int cardsDealtCount;
	int money;
	int GetScore(const vector<valRank>& hand);//Calculates the score of a hand

	vector<valRank> playerHand;
	vector<valRank> cpuHand;
	vector<Card> playerCards;
	vector<Card> cpuCards;

	Button hit=Button(100,"HIT");
	Button stand = Button(250, "STAND");
	Button Double = Button(400, "DOUBLE");
	Button Split = Button(550, "SPLIT");

	Images gameImage;
	Images suitImages[4];    // shared textures for suits

	Font mainFont;            // shared font

	valRank cpuHiddenCard;

	GameState state = GameState::betting;

	string resultText;

	Deck deck;
};
