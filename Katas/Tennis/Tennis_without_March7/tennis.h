#pragma once

#include <variant>
namespace tennis {
namespace detail {

// 基本的分数情况，对于有特殊意义的得分，分离为 forty 的情况
enum class points {
    love,
    fifteen,
    thirty,
};

enum class player {
    player_1,
    player_2,
};

struct normal_scoring {
    points player_1_points;
    points player_2_points;
};

struct forty_scoring {
    player leading_player;
    points other_player_points;
};

struct deuce {};

struct advantage {
    player player_with_advantage;
};

}  // namespace detail

class tennis_t {
public:
    tennis_t()
        : m_state(detail::normal_scoring{detail::points::love,
                                         detail::points::love}) {}
    auto get_state() const { return m_state; }

private:
    std::variant<detail::normal_scoring, detail::forty_scoring, detail::deuce,
                 detail::advantage>
        m_state;
};

}  // namespace tennis