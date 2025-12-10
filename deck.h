#pragma once
#include <vector>
using namespace std;

struct valRank {
	int value;
	int rank;
};

class Deck {
private:
	vector<valRank> cards;
	int deckSize;
public:
	Deck();
	vector<valRank> CreateDeck();
	vector<valRank> RandomizeDeck();
	void Draw();
	valRank DrawCard();
};
