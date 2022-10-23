#include "gtest/gtest.h"

// TODO: change settings then we can include card directly
#include "../src/game/card.h"
#include "../src/game/card.cpp"


// class CardTest:public::testing::Test {
//  protected:
// 	CardTest() {}
// 	virtual ~CardTest() {}
// 	virtual void setUp() {}
// };

TEST(cardTest1, testGetter) {
	Card card(Card::heart, Card::king);
	EXPECT_EQ(card.getFace(), Card::king);
	EXPECT_EQ(card.getSuit(), Card::heart);
}

TEST(cardTest2, testOwner) {
	Card card(Card::heart, Card::king);
	EXPECT_FALSE(card.isOwned());
	card.setOwner("tiger1");
	EXPECT_TRUE(card.isOwned());
}