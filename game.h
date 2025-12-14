#pragma once
#include "Card.h"
#include "deck.h"
#include "button.h"
#include <vector>
#include <iostream>
using namespace std;


class Game {
public:
	Game();
	Deck deck;
	void Draw();//All the drawing functions
	void DrawStartingCards(double TimePassed); //Adds cards to player and dealer hands over time
	double LastUpdateTime;
	int cardsDrown;
	bool Drown;//Are the starting cards drawn
	int GetScore(vector<valRank> hand);//Calculates the score of a hand
	vector<valRank> playerHand;
private:
	vector<valRank> cpuHand;
	vector<Card> playerCards;
	vector<Card> cpuCards;
	Button hit=Button(100,"HIT");
	Button stand = Button(250, "STAND");
	Button Double = Button(400, "DOUBLE");
	Button Split = Button(550, "SPLIT");
	Images matImage = Images(4);
	int money;
	void DrawCards();//Draws all cards in a vector
	void DrawScore();//Draws the scores of player and dealer
	void DrawButtons();//Draws the buttons
	void DrawBackground();//Draws the table background
};
