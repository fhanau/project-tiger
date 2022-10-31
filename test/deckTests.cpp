#include <set>
#include <utility>

#include "gtest/gtest.h"
#include "deck.h"


TEST(deckTest1, testConstructor) {
    Deck deck(false);
    ASSERT_EQ(deck.getNumRemainingCards(), 52);

    // draw all cards
    auto cards = deck.dealCards(52);
    // check no duplicate card exists
    std::set<std::pair<int, int>> card_set;
    for (auto& card: cards) {
        auto p = std::make_pair<int, int>(card->getSuit(), card->getFace());
        EXPECT_EQ(card_set.find(p), card_set.end());
        card_set.insert(p);
    }
}

TEST(deckTest2, testDealArbitraryCards) {
    Deck deck(false);
    auto cards = deck.dealCards(3);
    EXPECT_EQ(cards.size(), 3);
    EXPECT_EQ(deck.getNumRemainingCards(), 52 - 3);
    cards = deck.dealCards(10);
    EXPECT_EQ(cards.size(), 10);
    EXPECT_EQ(deck.getNumRemainingCards(), 52 - 13);
}

TEST(deckTest3, testDealGivenCards) {
    Deck deck(false);
    auto card = deck.dealGivenCards(Card::club, Card::ace);
    EXPECT_EQ(deck.getNumRemainingCards(), 51);
    EXPECT_EQ(*card, Card(Card::club, Card::ace));

    // deal a card which is already gone returns nullptr
    card = deck.dealGivenCards(Card::club, Card::ace);
    EXPECT_TRUE(nullptr == card);
    EXPECT_EQ(deck.getNumRemainingCards(), 51);
    
    // deal multiple given cards
    auto cards = deck.dealGivenCards({
        {Card::heart, Card::ace},
        {Card::diamond, Card::ace},
        {Card::spade, Card::ace}
    });
    EXPECT_EQ(deck.getNumRemainingCards(), 48);
    ASSERT_EQ(cards.size(), 3);
    for (auto card: cards) {
        EXPECT_EQ(card->getFace(), Card::ace);
    }
}