#include <gtest/gtest.h>

#include <variant>

#include "tennis.h"

using namespace std;
using namespace tennis::detail;

class TennisTest : public ::testing::Test {
public:
    TennisTest()
        : love_love{points::love, points::love},
          fifteen_fifteen{points::fifteen, points::fifteen},
          thirty_thirty{points::thirty, points::thirty},
          love_fifteen{points::love, points::fifteen},
          fifteen_love{points::fifteen, points::love},
          love_thirty{points::love, points::thirty},
          thirty_love{points::thirty, points::love},
          fifteen_thirty{points::fifteen, points::thirty},
          thirty_fifteen{points::thirty, points::fifteen},
          forty_love{player::player_1, points::love},
          forty_fifteen{player::player_1, points::fifteen},
          forty_thirty{player::player_1, points::thirty},
          love_forty{player::player_2, points::love},
          fifteen_forty{player::player_2, points::fifteen},
          thirty_forty{player::player_2, points::thirty},
          player_1_advantage{player::player_1},
          player_2_advantage{player::player_2} {}

protected:
    normal_scoring love_love;
    normal_scoring fifteen_fifteen;
    normal_scoring thirty_thirty;
    normal_scoring love_fifteen;
    normal_scoring fifteen_love;
    normal_scoring love_thirty;
    normal_scoring thirty_love;
    normal_scoring fifteen_thirty;
    normal_scoring thirty_fifteen;

    forty_scoring forty_love;
    forty_scoring forty_fifteen;
    forty_scoring forty_thirty;
    forty_scoring love_forty;
    forty_scoring fifteen_forty;
    forty_scoring thirty_forty;

    advantage player_1_advantage;
    advantage player_2_advantage;
};

TEST_F(TennisTest, InitialStateShouldBeZeroToZero) {
    tennis::tennis_t test_1;
    auto test_1_state = test_1.get_state();
    EXPECT_EQ(std::holds_alternative<normal_scoring>(test_1_state), true);
    auto test_1_points = *std::get_if<normal_scoring>(&test_1_state);
    EXPECT_EQ(test_1_points, love_love);
}

TEST_F(TennisTest, OneWinPointsAndUpdateHisStates) {
    tennis::tennis_t test_2;
    tennis::tennis_t test_3;

    auto test_2_state = test_2.get_state();
    auto test_3_state = test_3.get_state();

    auto test_2_scoring_state = *std::get_if<normal_scoring>(&test_2_state);
    auto test_3_scoring_state = *std::get_if<normal_scoring>(&test_3_state);
    ASSERT_EQ(test_2_scoring_state, love_love);
    ASSERT_EQ(test_3_scoring_state, love_love);

    test_2_state = test_2.point_for(player::player_1);
    EXPECT_TRUE(std::holds_alternative<normal_scoring>(test_2_state));
    EXPECT_EQ(*(std::get_if<normal_scoring>(&test_2_state)), fifteen_love);

    test_2_state = test_2.point_for(player::player_1);
    EXPECT_EQ(*(std::get_if<normal_scoring>(&test_2_state)), thirty_love);

    test_2_state = test_2.point_for(player::player_1);
    EXPECT_TRUE(std::holds_alternative<forty_scoring>(test_2_state));
    EXPECT_EQ(*(std::get_if<forty_scoring>(&test_2_state)), forty_love);

    test_3_state = test_3.point_for(player::player_2);
    EXPECT_TRUE(std::holds_alternative<normal_scoring>(test_3_state));
    EXPECT_EQ(*(std::get_if<normal_scoring>(&test_3_state)), love_fifteen);

    test_3_state = test_3.point_for(player::player_2);
    EXPECT_EQ(*(std::get_if<normal_scoring>(&test_3_state)), love_thirty);

    test_3_state = test_3.point_for(player::player_2);
    EXPECT_TRUE(std::holds_alternative<forty_scoring>(test_3_state));
    EXPECT_EQ(*(std::get_if<forty_scoring>(&test_3_state)), love_forty);
}
