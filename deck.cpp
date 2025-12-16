#include "deck.h"
#include <random>
#include <iostream>

Deck::Deck(){
    srand(static_cast<unsigned>(time(nullptr)));
    cards=RandomizeDeck();
    deckSize = 52;
}

vector<valRank> Deck::CreateDeck(){
    vector<valRank> tempDeck;
    int rank, value;
    for (int i = 0; i < 4; i++) {
        rank = i;
        for (int j = 1; j <= 13; j++) {
            value = j;
            tempDeck.push_back({ value,rank });
        }
    }
    return tempDeck;
}

vector<valRank> Deck::RandomizeDeck(){
    vector<valRank> tempDeck;
    tempDeck = CreateDeck();
    for (int i = tempDeck.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(tempDeck[i], tempDeck[j]);
    }
    return tempDeck;
}

void Deck::Draw() {
    for (int i = 0; i < deckSize; i++) {
        cout << cards[i].value << " " << cards[i].rank << endl;
    }
}

valRank Deck::DrawCard(){
    if (cards.empty()) {
        cards = RandomizeDeck();
        deckSize = 52;
    }
    valRank temp = cards[deckSize - 1];
    cards.pop_back();
    deckSize--;
    return temp;
}
