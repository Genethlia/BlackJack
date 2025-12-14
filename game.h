#pragma once
#include "Card.h"
#include "deck.h"
#include "button.h"
#include <vector>
#include <iostream>
constexpr int player_Y = 650;
constexpr int dealer_Y = 50;
constexpr int cardSpacing = 160;
using namespace std;

enum class GameState{
	dealing,
	playerTurn,
	dealerTurn,
	roundEnd,
};

class Game {
public:
	Game();
	Deck deck;
	void Draw();//All the drawing functions
	void UpdateDealing(double TimePassed); //Adds cards to player and dealer hands over time
	double LastUpdateTime;
	double dealerLastUpdateTime;
	int cardsDealtCount;
	int GetScore(const vector<valRank>& hand);//Calculates the score of a hand
	vector<valRank> playerHand;
	void Update();
	bool HasEnoughTimePassed(double& lastUpdateTime,double TimePassed);
private:
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


	int money;
	void DrawCards();//Draws all cards in a vector
	void DrawScore();//Draws the scores of player and dealer
	void DrawButtons();//Draws the buttons
	void DrawBackground();//Draws the table background
	void DrawUpsideCard();
	void HitPlayer();//Adds a card to the player's hand
	void UpdateButtons();//Updates button states
	void cpuGet17();//Dealer draws cards until reaching at least 17

	valRank cpuHiddenCard;

	GameState state = GameState::dealing;
};
