#include <gtest/gtest.h>

#include "leapyear.h"

using namespace std;

TEST(LeapYearUnitTest, LeapYearCanBeDividedBy400) {
    ASSERT_TRUE(is_leap_year(400));
    ASSERT_TRUE(is_leap_year(800));
    ASSERT_TRUE(is_leap_year(1200));
    ASSERT_FALSE(is_leap_year(100));
    ASSERT_FALSE(is_leap_year(300));
    ASSERT_FALSE(is_leap_year(500));
}

TEST(LeapYearUnitTest, LeapYearCanBeDividedBy4ButNot100) {
    ASSERT_TRUE(is_leap_year(2008));
    ASSERT_TRUE(is_leap_year(2012));
    ASSERT_TRUE(is_leap_year(2016));
    ASSERT_FALSE(is_leap_year(2100));
    ASSERT_FALSE(is_leap_year(2017));
    ASSERT_FALSE(is_leap_year(2018));
    ASSERT_FALSE(is_leap_year(1700));
}
