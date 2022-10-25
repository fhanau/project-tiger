#ifndef SRC_GAME_CARD_H_
#define SRC_GAME_CARD_H_

#include <string>

// Class representing a card for card games
class Card {
 public:
    enum Suit {
        heart = 0,
        diamond = 1,
        spade = 2,
        club = 3,
        nil
    };

    enum Face {
        ace = 1,
        two = 2,
        three = 3,
        four = 4,
        five = 5,
        six = 6,
        seven = 7,
        eight = 8,
        nine = 9,
        ten = 10,
        jack = 11,
        queen = 12,
        king = 13,
        black_joker = 14,
        red_joker = 15
    };

 private:
    Suit suit;
    Face face;
    // the player index who owns the card, "" means not owned by anyone
    std::string owner;

 public:
    // Create card
    Card(Suit suit, Face face);
    // Create card with a given owner
    Card(Suit suit, Face face, const std::string& owner);
    Card(const Card& card);
    ~Card();
    Suit getSuit() const;
    Face getFace() const;
    bool isOwned() const;
    void setOwner(std::string owner);
    std::string getOwner() const;

    // Print representation of the card to standard output.
    void display() const;
    // Compare the value of two cards
    bool comparator(const Card& card1, const Card& card2) const;
    //Operator to test if cards are identical.
    bool operator==(const Card& rhs) const;
};

#endif  // SRC_GAME_CARD_H_
