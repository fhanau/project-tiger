#ifndef SRC_GAME_CARD_H_
#define SRC_GAME_CARD_H_

#include <string>

class Card {
 public:
    enum Suit {
        heart = 0,
        diamond = 1,
        spade = 2,
        club = 3,
        nil
    };

    // TODO(Songheng): not sure if needed
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
    Card(Face face, Suit suit);
    Card(Face face, Suit suit, std::string& owner);
    Card(const Card& card);
    ~Card();
    Suit getSuit() const;
    Face getFace() const;
    bool isOwned() const;
    std::string getOwner() const;
    void display() const;
    bool comparator(const Card& card1, const Card& card2) const;
};

bool operator==(const Card& lhs, const Card& rhs) {
    return typeid(lhs) == typeid(rhs) 
        && lhs.getFace() == rhs.getFace()
        && lhs.getSuit() == rhs.getSuit();
}

#endif  // SRC_GAME_CARD_H_
