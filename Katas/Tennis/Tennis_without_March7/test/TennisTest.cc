#include <gtest/gtest.h>

#include <variant>

#include "tennis.h"

using namespace std;

TEST(TennisInitialStateTestCase, InitialStateShouldBeZeroToZero) {
    using namespace tennis::detail;
    tennis::tennis_t test_1;
    auto test_1_state = test_1.get_state();
    EXPECT_EQ(std::holds_alternative<normal_scoring>(test_1_state), true);
    auto* test_1_points = std::get_if<normal_scoring>(&test_1_state);
    EXPECT_EQ(test_1_points->player_1_points, points::love);
    EXPECT_EQ(test_1_points->player_2_points, points::love);
}