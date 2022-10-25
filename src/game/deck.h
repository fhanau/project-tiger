#ifndef SRC_GAME_DECK_H_
#define SRC_GAME_DECK_H_

#include <random>
#include <algorithm>
#include <utility>
#include <vector>

#include "card.h"

// Deck class, to implement card games based on the Card class.
class Deck {
 private:
    bool includeJokers;
    int cardCount;
    std::vector<Card*> remainingCards;
    std::vector<Card*> dealedCards;

 public:
    // Create a deck.
    Deck(bool includeJokers = false);
    ~Deck();

    int getNumRemainingCards() const;
    void shuffle();
    void reset();

    // deal single/multiple cards
    Card* dealCards();

    // Deal a certain number of cards.
    std::vector<Card*> dealCards(int amount);

    // Deal a specific card, if available.
    Card* dealGivenCards(Card::Suit suit, Card::Face face);

    // Deal the following cards as available.
    std::vector<Card*> dealGivenCards(const std::vector<std::pair<Card::Suit,
        Card::Face>>& cards);
};

#endif  // SRC_GAME_DECK_H_
