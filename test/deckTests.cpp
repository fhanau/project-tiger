#include "gtest/gtest.h"

#include "deck.h"


TEST(deckTest1, testConstructor) {
    Deck deck(0);
    ASSERT_EQ(deck.getNumReaminingCards(), 52);
}