#pragma once
#include <vector>
using namespace std;

struct valRank {
	int value;
	int rank;
};

class Deck {
private:
	vector<valRank> cards;//The deck of cards
	int deckSize;//52 cards
public:
	Deck();
	vector<valRank> CreateDeck();
	vector<valRank> RandomizeDeck();//Shuffles the deck
	void Draw();//Just for testing
	valRank DrawCard();//Draws a card from the deck
};
