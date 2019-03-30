#include "Utilities.h"
#include "gtest/gtest.h"

TEST(UtilitiesTest, TestThatCollidingRectanglesAreColliding) {
	pp::Rectangle rec1(0, 0, 4, 4);
	pp::Rectangle rec2(1, 1, 4, 4);
	pp::Rectangle rec3(-1, -1, 4, 4);
	pp::Rectangle rec4(1, 1, 1, 1);

	EXPECT_TRUE(pp::AreColliding(rec1, rec2));
	EXPECT_TRUE(pp::AreColliding(rec1, rec3));
	EXPECT_TRUE(pp::AreColliding(rec1, rec4));
}
