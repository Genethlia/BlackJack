#pragma once
#include "Card.h"
#include "deck.h"
#include "button.h"
#include <vector>
using namespace std;


class Game {
public:
	Game();
	Deck deck;
	void Draw();
	void Update(double TimePassed);
	double LastUpdateTime;
	int cardsDrown;
	bool Drown;
private:
	vector<valRank> playerHand;
	vector<valRank> cpuHand;
	vector<Card> playerCards;
	vector<Card> cpuCards;
	Button hit=Button(100,"HIT");
	Button stand = Button(250, "STAND");
	int money;
	void DrawCards(vector<Card> cards);
};
