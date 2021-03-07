#include <gtest/gtest.h>

#include "bowling.h"

using namespace std;


TEST(BowlingUnitTest, StartBowlingAndTheFirstFrameTest){
    using namespace detail;
    bowling_t bowling_1, bowling_2, bowling_3;
    bowling_1.frame_for(strike{});
    ASSERT_EQ(bowling_1.get_score(), 10);
    ASSERT_EQ(bowling_1.get_bonus(), bonus_t::two);
    ASSERT_EQ(bowling_1.get_frameID(), 1);

    bowling_2.frame_for(spare{});
    ASSERT_EQ(bowling_2.get_score(), 10);
    ASSERT_EQ(bowling_2.get_bonus(), bonus_t::one);
    ASSERT_EQ(bowling_2.get_frameID(), 1);

    bowling_3.frame_for(miss{2, 7});
    ASSERT_EQ(bowling_3.get_score(), 9);
    ASSERT_EQ(bowling_3.get_bonus(), bonus_t::zero);
    ASSERT_EQ(bowling_3.get_frameID(), 1);
}



