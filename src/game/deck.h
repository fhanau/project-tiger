#ifndef SRC_GAME_DECK_H_
#define SRC_GAME_DECK_H_

#include <random>
#include <algorithm>
#include <utility>
#include <vector>

#include "./card.h"

class Deck {
 private:
    bool includeJokers;
    int cardCount;
    std::vector<Card*> remainingCards;
    std::vector<Card*> dealedCards;

 public:
    Deck(bool includeJokers=false);
    ~Deck();
	int getNumReaminingCards();
    void shuffle();
    void reset();
    // deal single/multiple cards
    Card* dealCards();
    std::vector<Card*> dealCards(int amount);
    Card* dealGivenCards(Card::Suit suit, Card::Face face);
    std::vector<Card*> dealGivenCards(const std::vector<std::pair<Card::Suit,
        Card::Face>>& cards);
};

#endif  // SRC_GAME_DECK_H_
