#include <gtest/gtest.h>

#include "bowling.h"

using namespace std;

class BowlingTest : public ::testing::Test {
public:
    BowlingTest()
        : bowling_first_strike(2),
          bowling_first_spare(2),
          bowling_first_miss(2) {
        bowling_first_strike.frame_for(detail::strike());
        bowling_first_spare.frame_for(detail::spare(5, 5));
        bowling_first_miss.frame_for(detail::miss(2, 7));
    }
    ~BowlingTest() = default;

protected:
    bowling_t bowling_first_strike;
    bowling_t bowling_first_spare;
    bowling_t bowling_first_miss;
};

TEST_F(BowlingTest, StartBowlingGame) {
    using namespace detail;
    EXPECT_TRUE(
        std::holds_alternative<strike>(bowling_first_strike.get_state()));
    EXPECT_EQ(bowling_first_strike.get_score(), 10);
    EXPECT_EQ(bowling_first_strike.get_frameID(), 1);
    EXPECT_EQ(bowling_first_strike.get_bonus(), bonus_t::two);

    EXPECT_TRUE(std::holds_alternative<spare>(bowling_first_spare.get_state()));
    EXPECT_EQ(bowling_first_spare.get_score(), 10);
    EXPECT_EQ(bowling_first_spare.get_frameID(), 1);
    EXPECT_EQ(bowling_first_spare.get_bonus(), bonus_t::one);

    EXPECT_TRUE(std::holds_alternative<miss>(bowling_first_miss.get_state()));
    EXPECT_EQ(bowling_first_miss.get_score(), 9);
    EXPECT_EQ(bowling_first_miss.get_frameID(), 1);
    EXPECT_EQ(bowling_first_miss.get_bonus(), bonus_t::zero);
}

TEST_F(BowlingTest, StrikeAndStrikeTest) {
    using namespace detail;
    // strike + strike
    auto state_1 = bowling_first_strike.frame_for(strike());
    ASSERT_TRUE(std::holds_alternative<clear>(state_1));
    EXPECT_EQ(bowling_first_strike.get_score(), 30);
    EXPECT_EQ(bowling_first_strike.get_bonus(), bonus_t::two);

    // strike + strike + strike + strike 的奖励得分情况:
    auto double_strike_bonus = bowling_first_strike;
    auto double_strike_bonus_state = double_strike_bonus.frame_for(strike());
    EXPECT_EQ(double_strike_bonus.get_score(), 50);
    EXPECT_EQ(double_strike_bonus.get_frameID(), 3);
    ASSERT_TRUE(std::holds_alternative<clear>(double_strike_bonus_state));
    double_strike_bonus_state = double_strike_bonus.frame_for(strike());
    EXPECT_EQ(double_strike_bonus.get_score(), 60);
    EXPECT_EQ(double_strike_bonus.get_frameID(), 4);
    ASSERT_TRUE(std::holds_alternative<game_over>(double_strike_bonus_state));

    // strike + strike + spare 的情况
    auto strike_spare_bonus = bowling_first_strike;
    auto strike_spare_bonus_state = strike_spare_bonus.frame_for(spare(5, 5));
    EXPECT_EQ(strike_spare_bonus.get_score(), 45);
    EXPECT_EQ(double_strike_bonus.get_frameID(), 4);

    // strike + strike + miss 的情况
    auto strike_miss_bonus = bowling_first_strike;
    auto strike_miss_bonus_state = strike_miss_bonus.frame_for(miss(3, 5));
    EXPECT_TRUE(std::holds_alternative<game_over>(strike_miss_bonus_state));
    EXPECT_EQ(strike_miss_bonus.get_score(), 41);
    EXPECT_EQ(double_strike_bonus.get_frameID(), 4);
}

TEST_F(BowlingTest, SpareAndSpareTest) {
    using namespace detail;
    // spare + spare
    auto state_1 = bowling_first_spare.frame_for(spare(2, 8));
    ASSERT_TRUE(std::holds_alternative<clear>(state_1));
    EXPECT_EQ(bowling_first_spare.get_score(), 22);

    // spare + spare + strike
    auto single_strike_bonus = bowling_first_spare;
    ASSERT_TRUE(std::holds_alternative<clear>(single_strike_bonus.get_state()));
    auto single_strike_bonus_state_2 = single_strike_bonus.frame_for(strike());
    ASSERT_TRUE(std::holds_alternative<game_over>(single_strike_bonus_state_2));
    EXPECT_EQ(single_strike_bonus.get_score(), 32);

    // spare + spare + spare
    auto double_spare_bonus = bowling_first_spare;
    auto double_spare_bonus_state_3 = double_spare_bonus.frame_for(spare(3, 7));
    ASSERT_TRUE(std::holds_alternative<game_over>(double_spare_bonus_state_3));
    EXPECT_EQ(double_spare_bonus.get_score(), 25);

    // spare + spare + miss
    auto spare_miss_bonus = bowling_first_spare;
    auto spare_miss_bonus_state_3 = spare_miss_bonus.frame_for(miss(2, 6));
    ASSERT_TRUE(std::holds_alternative<game_over>(spare_miss_bonus_state_3));
    EXPECT_EQ(spare_miss_bonus.get_score(), 24);
}

TEST_F(BowlingTest, MissAndMissTest) {
    using namespace detail;
    // miss + miss
    auto state_1 = bowling_first_miss.frame_for(miss(3, 5));
    EXPECT_TRUE(std::holds_alternative<game_over>(state_1));
    EXPECT_EQ(bowling_first_miss.get_score(), 17);
}

TEST_F(BowlingTest, SpareAndStrikeTest) {
    using namespace detail;
    // spare + strike
    auto state_2 = bowling_first_spare.frame_for(strike());
    ASSERT_TRUE(std::holds_alternative<clear>(state_2));
    EXPECT_EQ(bowling_first_spare.get_score(), 30);

    // spare + strike + strike + strike
    auto double_strike_bonus = bowling_first_spare;
    auto double_strike_bonus_state = double_strike_bonus.frame_for(strike());
    ASSERT_TRUE(std::holds_alternative<clear>(double_strike_bonus_state));
    EXPECT_EQ(double_strike_bonus.get_score(), 40);
    EXPECT_EQ(double_strike_bonus.get_frameID(), 3);
    double_strike_bonus_state = double_strike_bonus.frame_for(strike());
    ASSERT_TRUE(std::holds_alternative<game_over>(double_strike_bonus_state));
    EXPECT_EQ(double_strike_bonus.get_score(), 50);
    EXPECT_EQ(double_strike_bonus.get_frameID(), 4);

    // spare + strike + spare
    auto strike_spare_bonus = bowling_first_spare;
    auto strike_spare_bonus_state = strike_spare_bonus.frame_for(spare(5, 5));
    ASSERT_TRUE(std::holds_alternative<game_over>(strike_spare_bonus_state));
    EXPECT_EQ(strike_spare_bonus.get_score(), 40);

    // spare + strike + miss
    auto strike_miss_bonus = bowling_first_spare;
    auto strike_miss_bonus_state = strike_miss_bonus.frame_for(miss(2, 3));
    ASSERT_TRUE(std::holds_alternative<game_over>(strike_miss_bonus_state));
    EXPECT_EQ(strike_miss_bonus.get_score(), 35);
}

TEST_F(BowlingTest, SpareAndMissTest) {
    using namespace detail;
    // spare + miss
    auto state_1 = bowling_first_spare.frame_for(miss(2, 4));
    ASSERT_TRUE(std::holds_alternative<game_over>(state_1));
    EXPECT_EQ(bowling_first_spare.get_score(), 18);
}

TEST_F(BowlingTest, MissAndStrikeTest) {
    using namespace detail;
    ASSERT_TRUE(std::holds_alternative<miss>(bowling_first_miss.get_state()));
    auto state_3 = bowling_first_miss.frame_for(strike());
    ASSERT_TRUE(std::holds_alternative<clear>(state_3));
    EXPECT_EQ(bowling_first_miss.get_score(), 19);

    // miss + strike + strike + strike
    auto double_strike_bonus = bowling_first_miss;
    auto double_strike_bonus_state = double_strike_bonus.frame_for(strike());
    ASSERT_TRUE(std::holds_alternative<clear>(double_strike_bonus_state));
    EXPECT_EQ(double_strike_bonus.get_score(), 29);
    EXPECT_EQ(double_strike_bonus.get_frameID(), 3);
    double_strike_bonus_state = double_strike_bonus.frame_for(strike());
    ASSERT_TRUE(std::holds_alternative<game_over>(double_strike_bonus_state));
    EXPECT_EQ(double_strike_bonus.get_score(), 39);
    EXPECT_EQ(double_strike_bonus.get_frameID(), 4);

    // miss + strike + spare
    auto strike_spare_bonus = bowling_first_miss;
    auto strike_spare_bonus_state = strike_spare_bonus.frame_for(spare(5, 5));
    ASSERT_TRUE(std::holds_alternative<game_over>(strike_spare_bonus_state));
    EXPECT_EQ(strike_spare_bonus.get_score(), 29);

    // miss + strike + miss
    auto strike_miss_bonus = bowling_first_miss;
    auto strike_miss_bonus_state = strike_miss_bonus.frame_for(miss(2, 3));
    ASSERT_TRUE(std::holds_alternative<game_over>(strike_miss_bonus_state));
    EXPECT_EQ(strike_miss_bonus.get_score(), 24);
}

TEST_F(BowlingTest, MissAndSpareTest) {
    using namespace detail;
    auto state_1 = bowling_first_miss.frame_for(spare(5, 5));
    ASSERT_TRUE(std::holds_alternative<clear>(state_1));
    EXPECT_EQ(bowling_first_miss.get_score(), 19);

    // miss + spare + strike
    auto spare_strike_bonus = bowling_first_miss;
    auto spare_strike_bonus_state_2 = spare_strike_bonus.frame_for(strike{});
    ASSERT_TRUE(std::holds_alternative<game_over>(spare_strike_bonus_state_2));
    EXPECT_EQ(spare_strike_bonus.get_score(), 29);

    // miss + spare + spare
    auto spare_spare_bonus = bowling_first_miss;
    auto spare_spare_bonus_sate_3 = spare_spare_bonus.frame_for(spare(4, 6));
    ASSERT_TRUE(std::holds_alternative<game_over>(spare_spare_bonus_sate_3));
    EXPECT_EQ(spare_spare_bonus.get_score(), 23);

    // miss + spare + miss
    auto spare_miss_bonus = bowling_first_miss;
    auto spare_miss_bonus_sate_3 = spare_spare_bonus.frame_for(miss(0, 1));
    ASSERT_TRUE(std::holds_alternative<game_over>(spare_miss_bonus_sate_3));
    EXPECT_EQ(spare_miss_bonus.get_score(), 19);
}

TEST_F(BowlingTest, StrikeAndSpareTest) {
    using namespace detail;
    auto state_1 = bowling_first_strike.frame_for(spare(5, 5));
    ASSERT_TRUE(std::holds_alternative<clear>(state_1));
    EXPECT_EQ(bowling_first_strike.get_score(), 30);

    // strike + spare + strike
    auto spare_strike_bonus = bowling_first_strike;
    auto spare_strike_bonus_state_1 = spare_strike_bonus.frame_for(strike{});
    ASSERT_TRUE(std::holds_alternative<game_over>(spare_strike_bonus_state_1));
    EXPECT_EQ(spare_strike_bonus.get_score(), 40);

    // strike + spare + spare
    auto spare_spare_bonus = bowling_first_strike;
    auto spare_spare_bonus_state_1 = spare_spare_bonus.frame_for(spare{9, 1});
    ASSERT_TRUE(std::holds_alternative<game_over>(spare_spare_bonus_state_1));
    EXPECT_EQ(spare_spare_bonus.get_score(), 39);

    // strike + spare + miss
    auto spare_miss_bonus = bowling_first_strike;
    auto spare_miss_bonus_state_1 = spare_miss_bonus.frame_for(miss{5, 1});
    ASSERT_TRUE(std::holds_alternative<game_over>(spare_miss_bonus_state_1));
    EXPECT_EQ(spare_miss_bonus.get_score(), 35);
}

TEST_F(BowlingTest, StrikeAndMissTest) {
    using namespace detail;
    auto state_1 = bowling_first_strike.frame_for(miss(3, 3));
    ASSERT_TRUE(std::holds_alternative<game_over>(state_1));
    EXPECT_EQ(bowling_first_strike.get_score(), 22);
}

TEST(SpecialCaseTest, OnlyOneFramBowling) {
    using namespace detail;
    bowling_t one_frame(1);
    ASSERT_TRUE(std::holds_alternative<game_start>(one_frame.get_state()));
    EXPECT_EQ(one_frame.get_score(), 0);

    auto one_frame_miss = one_frame;
    auto one_frame_miss_state = one_frame_miss.frame_for(miss{3, 4});
    ASSERT_TRUE(std::holds_alternative<game_over>(one_frame_miss_state));
    EXPECT_EQ(one_frame_miss.get_score(), 7);

    auto one_frame_spare = one_frame;
    auto one_frame_spare_state = one_frame_spare.frame_for(spare{7, 3});
    ASSERT_TRUE(std::holds_alternative<clear>(one_frame_spare_state));
    EXPECT_EQ(one_frame_spare.get_score(), 10);

    auto one_frame_spare_miss = one_frame_spare;
    auto one_frame_spare_miss_state =
        one_frame_spare_miss.frame_for(miss{4, 3});
    ASSERT_TRUE(std::holds_alternative<game_over>(one_frame_spare_miss_state));
    EXPECT_EQ(one_frame_spare_miss.get_score(), 14);

    auto one_frame_spare_spare = one_frame_spare;
    auto one_frame_spare_spare_state =
        one_frame_spare_spare.frame_for(spare{7, 3});
    ASSERT_TRUE(std::holds_alternative<game_over>(one_frame_spare_spare_state));
    EXPECT_EQ(one_frame_spare_spare.get_score(), 17);

    auto one_frame_spare_strike = one_frame_spare;
    auto one_frame_spare_strike_state =
        one_frame_spare_strike.frame_for(strike{});
    ASSERT_TRUE(
        std::holds_alternative<game_over>(one_frame_spare_strike_state));
    EXPECT_EQ(one_frame_spare_strike.get_score(), 20);

    auto one_frame_strike = one_frame;
    auto one_frame_strike_state = one_frame_strike.frame_for(strike{});
    ASSERT_TRUE(std::holds_alternative<clear>(one_frame_strike_state));
    EXPECT_EQ(one_frame_strike.get_score(), 10);

    auto one_frame_strike_strike = one_frame_strike;
    auto one_frame_strike_strike_state =
        one_frame_strike_strike.frame_for(strike{});
    ASSERT_TRUE(std::holds_alternative<clear>(one_frame_strike_strike_state));
    EXPECT_EQ(one_frame_strike_strike.get_score(), 20);

    auto one_frame_strike_strike_strike = one_frame_strike_strike;
    auto one_frame_strike_strike_strike_state =
        one_frame_strike_strike_strike.frame_for(strike{});
    ASSERT_TRUE(std::holds_alternative<game_over>(
        one_frame_strike_strike_strike_state));
    EXPECT_EQ(one_frame_strike_strike_strike.get_score(), 30);

    auto one_frame_strike_strike_spare = one_frame_strike_strike;
    auto one_frame_strike_strike_spare_state =
        one_frame_strike_strike_spare.frame_for(spare{8, 2});
    ASSERT_TRUE(
        std::holds_alternative<game_over>(one_frame_strike_strike_spare_state));
    EXPECT_EQ(one_frame_strike_strike_spare.get_score(), 28);

    auto one_frame_strike_strike_miss = one_frame_strike_strike;
    auto one_frame_strike_strike_miss_state =
        one_frame_strike_strike_miss.frame_for(miss{1, 1});
    ASSERT_TRUE(
        std::holds_alternative<game_over>(one_frame_strike_strike_miss_state));
    EXPECT_EQ(one_frame_strike_strike_miss.get_score(), 21);

    auto one_frame_strike_miss = one_frame_strike;
    auto one_frame_strike_miss_state =
        one_frame_strike_miss.frame_for(miss{1, 1});
    ASSERT_TRUE(
        std::holds_alternative<game_over>(one_frame_strike_strike_miss_state));
    EXPECT_EQ(one_frame_strike_miss.get_score(), 12);

    auto one_frame_strike_spare = one_frame_strike;
    auto one_frame_strike_spare_state =
        one_frame_strike_spare.frame_for(miss{1, 1});
    ASSERT_TRUE(
        std::holds_alternative<game_over>(one_frame_strike_spare_state));
    EXPECT_EQ(one_frame_strike_spare.get_score(), 12);
}